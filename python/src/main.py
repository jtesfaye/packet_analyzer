"""
pywebview entry point + the SnifferAPI js_api object exposed to React as
window.pywebview.api.

Threading model (why this file looks the way it does)
-------------------------------------------------------
pywebview runs *every* js_api call on its own short-lived thread [1]. That
means start(), pause(), stop(), save(), get_details() and drain() can all be
invoked concurrently with each other -- including two overlapping calls to
start() if the user double-clicks, or stop() racing a start() that hasn't
finished spinning up yet. All state that more than one of those methods
touches (`_state`, `_session`, `_worker`, the outbox) is protected by a
single `RLock`; nothing reaches into `_session` without holding it.

The actual packet capture lives on one dedicated worker thread per capture
("epoch"). js_api methods never call `session.poll()` themselves -- they only
start/stop that worker and send commands into the (already thread-safe)
CaptureSession. This keeps "am I allowed to touch the session right now" a
single-writer question instead of something every method has to reason about
independently.

An `_epoch` counter is bumped on every start(). The worker captures its own
epoch and checks it before publishing anything, so a slow-to-die worker from
a previous session (e.g. stop() timed out waiting on a wedged C++ thread)
can never leak stale packets into a new one.

[1] https://pywebview.flowrl.com/guide/api.html#security -- "each js_api
    call spawns a new thread". Do not call evaluate_js from the *main*
    thread on macOS (WKWebView requires it be dispatched there itself) --
    only ever call it from a worker thread, which is what _wake_ui() does.

See ../../ui/src/lib/snifferBridge.ts for the JS side of the data plane.
"""

from __future__ import annotations

import atexit
import os
import sys
import threading
import time
import traceback
from collections import deque
from dataclasses import dataclass
from enum import Enum
from typing import Optional

import webview

BUILD_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "build")
sys.path.append(os.path.abspath(BUILD_DIR))
import sniffer_py  # noqa: E402  (needs sys.path set up first)

_EMPTY = sniffer_py.EventMessageType.Empty
_PACKET = sniffer_py.EventMessageType.Packet
_DETAIL = sniffer_py.EventMessageType.Detail
_STAT = sniffer_py.EventMessageType.Stat

# CaptureConfig.settings bit flags (mirrors capture::* in CaptureConfig.h)
PROMISC = 0x20
IMMEDIATE = 0x10
MONITOR = 0x08
PRECISION = 0x04


class State(str, Enum):
    IDLE = "idle"
    RUNNING = "running"
    PAUSED = "paused"
    STOPPING = "stopping"


@dataclass
class _PendingDetail:
    packet_id: int
    requested_at: float


class SnifferAPI:
    """Exposed to React as window.pywebview.api."""

    OUTBOX_MAX = 20_000        # packets buffered for the UI before we drop
    POLL_IDLE_SLEEP = 0.002    # s -- backoff while the C++ queue is empty
    WAKE_MIN_INTERVAL = 0.05   # s -- min gap between evaluate_js UI nudges
    STOP_JOIN_TIMEOUT = 5.0    # s -- how long stop() waits for the worker

    def __init__(self) -> None:
        self._lock = threading.RLock()
        self._window: Optional[webview.Window] = None

        self._state = State.IDLE
        self._epoch = 0                    # bumped on every start()
        self._worker: Optional[threading.Thread] = None
        self._session: Optional[sniffer_py.CaptureSession] = None
        self._active_cfg = None            # last CaptureConfig, for resume()

        self._stop_evt = threading.Event()
        self._last_error: Optional[str] = None

        # data plane
        self._outbox: deque[dict] = deque(maxlen=self.OUTBOX_MAX)
        self._dropped = 0
        self._last_wake = 0.0

        # FIFO correlation for get_details() -- the C++ side doesn't echo
        # the packet id back, it just emits a bare Detail message, so we
        # have to assume replies arrive in request order.
        self._pending_details: deque[_PendingDetail] = deque()

    # =====================================================================
    # wiring
    # =====================================================================
    def attach_window(self, window: webview.Window) -> None:
        with self._lock:
            self._window = window

    # =====================================================================
    # js_api: control plane
    # =====================================================================
    def start(self, config: dict) -> dict:
        with self._lock:
            if self._state in (State.RUNNING, State.PAUSED):
                return self._status_locked(error="capture already running")
            if self._state is State.STOPPING:
                return self._status_locked(error="still stopping previous capture")

            try:
                cfg = self._build_config(config)
            except (KeyError, TypeError, ValueError) as exc:
                return self._status_locked(error=f"bad config: {exc}")

            self._reap_worker_locked()

            self._epoch += 1
            epoch = self._epoch
            self._stop_evt = threading.Event()
            self._last_error = None
            self._outbox.clear()
            self._dropped = 0
            self._pending_details.clear()
            self._active_cfg = cfg

            # Constructed here (not in the worker) so that by the time this
            # call returns, self._session is never None while state says
            # RUNNING -- pause()/save()/get_details() would otherwise have
            # a window where they see the right state but a null session.
            session = sniffer_py.CaptureSession()
            self._session = session

            self._state = State.RUNNING
            self._worker = threading.Thread(
                target=self._run,
                args=(epoch, session, cfg, self._stop_evt),
                name=f"sniffer-worker-{epoch}",
                daemon=True,
            )
            self._worker.start()
            return self._status_locked()

    def pause(self) -> dict:
        with self._lock:
            if self._state is not State.RUNNING:
                return self._status_locked(error="not running")
            self._session.send_command(sniffer_py.SessionCommand.stop())
            self._state = State.PAUSED
            return self._status_locked()

    def resume(self) -> dict:
        with self._lock:
            if self._state is not State.PAUSED:
                return self._status_locked(error="not paused")
            if self._session is None:
                return self._status_locked(error="session gone; start a new capture")
            # NOTE: relies on CaptureSession being restartable after Stop --
            # true as of CaptureEngine::stop()/shutdown() joining cleanly,
            # but re-verify if that ever changes.
            self._session.send_command(sniffer_py.SessionCommand.start(self._active_cfg))
            self._state = State.RUNNING
            return self._status_locked()

    def stop(self) -> dict:
        with self._lock:
            if self._state is State.IDLE:
                return self._status_locked()
            self._state = State.STOPPING
            self._stop_evt.set()
            worker = self._worker

        # Join outside the lock: the worker's finally-block needs self._lock
        # on its way out to clear self._session, so holding it here would
        # deadlock the two threads against each other.
        if worker is not None:
            worker.join(timeout=self.STOP_JOIN_TIMEOUT)
            if worker.is_alive():
                sys.stderr.write(
                    "SnifferAPI.stop: worker did not exit within "
                    f"{self.STOP_JOIN_TIMEOUT}s; abandoning it\n"
                )

        with self._lock:
            self._reap_worker_locked()
            self._state = State.IDLE
            return self._status_locked()

    def save(self, path: str) -> dict:
        with self._lock:
            if self._session is None:
                return self._status_locked(error="no session to save")
            self._session.send_command(sniffer_py.SessionCommand.save(path))
            return self._status_locked()

    def get_details(self, packet_id: int) -> dict:
        with self._lock:
            if self._session is None:
                return {"ok": False, "error": "no active session"}
            packet_id = int(packet_id)
            self._pending_details.append(_PendingDetail(packet_id, time.monotonic()))
            self._session.send_command(sniffer_py.SessionCommand.get_details(packet_id))
            return {"ok": True, "packet_id": packet_id}

    def status(self) -> dict:
        with self._lock:
            return self._status_locked()

    # =====================================================================
    # js_api: data plane -- the UI pulls this on its own cadence instead of
    # us pushing per-packet through evaluate_js. See snifferBridge.ts.
    # =====================================================================
    def drain(self, max_items: int = 5000) -> dict:
        with self._lock:
            n = min(max_items, len(self._outbox))
            batch = [self._outbox.popleft() for _ in range(n)]
            dropped, self._dropped = self._dropped, 0
            return {
                "packets": batch,
                "dropped": dropped,
                "more": len(self._outbox) > 0,
                "epoch": self._epoch,
                "state": self._state.value,
            }

    # =====================================================================
    # worker thread -- one per capture (epoch)
    # =====================================================================
    def _run(self, epoch: int, session, cfg, stop_evt: threading.Event) -> None:
        try:
            session.send_command(sniffer_py.SessionCommand.start(cfg))
            self._pump(epoch, session, stop_evt)
        except Exception:  # noqa: BLE001 -- report to the UI, don't die silently
            err = traceback.format_exc()
            with self._lock:
                if epoch == self._epoch:
                    self._last_error = err
            sys.stderr.write(err)
            self._wake_ui(force=True)
        finally:
            try:
                session.send_command(sniffer_py.SessionCommand.stop())
                # CaptureSession's worker thread is detached and only exits
                # once it sees End -- skip this and it leaks forever.
                session.send_command(sniffer_py.SessionCommand.end())
            except Exception:
                pass
            with self._lock:
                if epoch == self._epoch:
                    self._session = None
                    if self._state is not State.IDLE:
                        self._state = State.IDLE
            self._wake_ui(force=True)

    def _pump(self, epoch: int, session, stop_evt: threading.Event) -> None:
        while not stop_evt.is_set():
            msg = session.poll()

            if msg.type == _EMPTY:
                time.sleep(self.POLL_IDLE_SLEEP)
                continue

            # A newer start() has already moved on; stop feeding a stream
            # nobody is listening for anymore.
            if epoch != self._epoch:
                return

            if msg.type == _PACKET:
                self._enqueue({"kind": "packet", "packetId": None, "data": msg.message})
            elif msg.type == _DETAIL:
                self._enqueue({
                    "kind": "detail",
                    "packetId": self._match_pending_detail(),
                    "data": msg.message,
                })
            elif msg.type == _STAT:
                self._enqueue({"kind": "stat", "packetId": None, "data": msg.message})

    # =====================================================================
    # helpers
    # =====================================================================
    def _enqueue(self, item: dict) -> None:
        with self._lock:
            if len(self._outbox) == self._outbox.maxlen:
                self._dropped += 1  # deque silently evicts the oldest entry
            self._outbox.append(item)
        self._wake_ui()

    def _match_pending_detail(self) -> Optional[int]:
        with self._lock:
            if self._pending_details:
                return self._pending_details.popleft().packet_id
        return None

    def _wake_ui(self, force: bool = False) -> None:
        """Nudge the UI that drain() has something new, without pushing the
        payload itself through evaluate_js (see snifferBridge.ts for why)."""
        win = self._window
        if win is None:
            return
        now = time.monotonic()
        if not force and now - self._last_wake < self.WAKE_MIN_INTERVAL:
            return
        self._last_wake = now
        try:
            run_js = getattr(win, "run_js", None)  # fire-and-forget, pywebview >=4.4
            js = "window.__sniffer && window.__sniffer._wake()"
            (run_js or win.evaluate_js)(js)
        except Exception:
            pass  # window may be mid-teardown; a missed nudge just costs one poll tick

    def _reap_worker_locked(self) -> None:
        if self._worker is not None and not self._worker.is_alive():
            self._worker = None

    def _status_locked(self, error: Optional[str] = None) -> dict:
        return {
            "ok": error is None,
            "state": self._state.value,
            "epoch": self._epoch,
            "error": error or self._last_error,
            "buffered": len(self._outbox),
            "dropped": self._dropped,
        }

    @staticmethod
    def _build_config(c: dict) -> sniffer_py.CaptureConfig:
        cfg = sniffer_py.CaptureConfig()
        cfg.source = str(c["interface"])
        cfg.capture_size = int(c.get("snaplen") or 0)
        cfg.filter = c.get("bpfFilter") or ""
        cfg.packet_count = int(c.get("maxPackets") or 0)
        cfg.flags = 0xFF
        cfg.mode = sniffer_py.CaptureMode.Online

        settings = 0
        if c.get("promiscuous"):
            settings |= PROMISC
        if c.get("immediateMode"):
            settings |= IMMEDIATE
        if c.get("monitorMode"):
            settings |= MONITOR
        if c.get("highPrecisionTs"):
            settings |= PRECISION
        cfg.settings = settings
        return cfg


def main() -> None:
    api = SnifferAPI()

    dev_server = os.environ.get("SNIFFER_UI_URL")  # e.g. http://localhost:8443 for `vite dev`
    ui_entry = dev_server or os.path.join(
        os.path.dirname(__file__), "..", "..", "ui", "dist", "index.html"
    )

    window = webview.create_window(
        "Packet Analyzer",
        url=ui_entry,
        js_api=api,
        width=1280,
        height=800,
    )
    api.attach_window(window)

    # Best-effort: make sure the C++ capture is torn down if the window
    # closes while a capture is running, instead of leaking the detached
    # CaptureSession worker thread.
    atexit.register(api.stop)

    webview.start(debug=bool(dev_server))


if __name__ == "__main__":
    main()
