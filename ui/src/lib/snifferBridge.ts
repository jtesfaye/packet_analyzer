/**
 * Front-end half of the pywebview bridge.
 *
 * Two separate channels, on purpose:
 *
 *  - control plane: window.pywebview.api.{start,pause,resume,stop,save,
 *    get_details,status} -- a handful of calls a second at most, each a
 *    real request/response over pywebview's Promise-returning js_api.
 *
 *  - data plane: packets. Python does NOT push these into evaluate_js
 *    per-packet (see the note in main.py / below). Instead we pull them
 *    with api.drain(), on an animation-frame cadence, and Python "wakes"
 *    us via a tiny evaluate_js nudge when a batch is ready so we don't
 *    have to busy-poll while idle. See the README section at the bottom
 *    of this file for the alternatives that were considered.
 */
import type { SessionConfig } from "../types"

export type SnifferState = "idle" | "running" | "paused" | "stopping"

export interface SnifferStatus {
  ok: boolean
  state: SnifferState
  epoch: number
  error: string | null
  buffered: number
  dropped: number
}

export interface RawEvent {
  kind: "packet" | "detail" | "stat"
  packetId: number | null
  /** JSON string produced by the C++ core -- JSON.parse it yourself. */
  data: string
}

export interface DrainResult {
  packets: RawEvent[]
  dropped: number
  more: boolean
  epoch: number
  state: SnifferState
}

interface PyApi {
  start(config: SessionConfig): Promise<SnifferStatus>
  pause(): Promise<SnifferStatus>
  resume(): Promise<SnifferStatus>
  stop(): Promise<SnifferStatus>
  save(path: string): Promise<SnifferStatus>
  get_details(packetId: number): Promise<{
    ok: boolean
    packet_id?: number
    error?: string
  }>
  status(): Promise<SnifferStatus>
  drain(maxItems?: number): Promise<DrainResult>
}

declare global {
  interface Window {
    pywebview?: { api: PyApi }
    __sniffer?: { _wake(): void }
  }
}

const READY_TIMEOUT_MS = 10_000

/** Resolves once window.pywebview.api exists. Call before touching `api`. */
export function pywebviewReady(): Promise<PyApi> {
  if (window.pywebview?.api) return Promise.resolve(window.pywebview.api)
  return new Promise((resolve, reject) => {
    const timer = window.setTimeout(
      () => reject(new Error("pywebview did not become ready in time")),
      READY_TIMEOUT_MS,
    )
    window.addEventListener(
      "pywebviewready",
      () => {
        window.clearTimeout(timer)
        resolve(window.pywebview!.api)
      },
      { once: true },
    )
  })
}

function api(): PyApi {
  if (!window.pywebview?.api) {
    throw new Error("pywebview API not ready -- await pywebviewReady() first")
  }
  return window.pywebview.api
}

export const control = {
  start: (cfg: SessionConfig) => api().start(cfg),
  pause: () => api().pause(),
  resume: () => api().resume(),
  stop: () => api().stop(),
  save: (path: string) => api().save(path),
  getDetails: (packetId: number) => api().get_details(packetId),
  status: () => api().status(),
}

type Listener = (events: RawEvent[]) => void

/**
 * Pulls drain() on rAF cadence, plus a slow safety-net interval in case a
 * wake nudge is ever missed (window minimized, a dropped evaluate_js call,
 * etc.) -- correctness of the stream should never depend on the nudge
 * arriving, only its *latency* should.
 */
class PacketStream {
  private listeners = new Set<Listener>()
  private running = false
  private drainInFlight = false
  private frameScheduled = false
  private safetyTimer: number | null = null
  private epoch = 0

  constructor() {
    window.__sniffer = { _wake: () => this.scheduleDrain() }
  }

  subscribe(fn: Listener): () => void {
    this.listeners.add(fn)
    return () => this.listeners.delete(fn)
  }

  /** Call once a capture has started; `epoch` comes from the start() result. */
  begin(epoch: number) {
    this.epoch = epoch
    this.running = true
    this.scheduleDrain()
    if (this.safetyTimer == null) {
      this.safetyTimer = window.setInterval(() => this.scheduleDrain(), 250)
    }
  }

  end() {
    this.running = false
    if (this.safetyTimer != null) {
      window.clearInterval(this.safetyTimer)
      this.safetyTimer = null
    }
  }

  private scheduleDrain() {
    if (!this.running || this.frameScheduled) return
    this.frameScheduled = true
    requestAnimationFrame(() => {
      this.frameScheduled = false
      void this.pump()
    })
  }

  private async pump() {
    if (!this.running || this.drainInFlight || !window.pywebview?.api) return
    this.drainInFlight = true
    try {
      const res = await api().drain(5000)
      if (res.epoch !== this.epoch) return // stale capture, ignore
      if (res.packets.length) {
        for (const fn of this.listeners) fn(res.packets)
      }
      if (res.dropped) {
        console.warn(`sniffer: UI fell behind, dropped ${res.dropped} events`)
      }
      if (res.more) this.scheduleDrain()
    } catch (err) {
      console.error("sniffer: drain() failed", err)
    } finally {
      this.drainInFlight = false
    }
  }
}

export const packetStream = new PacketStream()

/*
 * ---------------------------------------------------------------------
 * Why pull-based drain() instead of Python pushing straight into JS?
 * ---------------------------------------------------------------------
 * The instinct ("when a packet arrives, call a TS callback") maps to
 * `window.evaluate_js("someGlobalFn(...)")` from Python -- pywebview has
 * no first-class callback handle, evaluate_js is the only direction Python
 * can talk without JS asking first. That works but has three problems at
 * packet-firehose volume:
 *
 *   1. Embedding data means turning it into a JS source string. JSON is
 *      *mostly* valid as a JS literal but not quite (U+2028/U+2029 differ),
 *      so it needs real escaping or a base64 hop either way.
 *   2. On macOS, evaluate_js is marshalled onto the Cocoa main thread and
 *      blocks the calling thread until the JS finishes running. Calling it
 *      once per packet serialises your capture-draining thread behind
 *      whatever the render thread is doing.
 *   3. It's a push with no backpressure: if React can't keep up, Python
 *      just keeps calling evaluate_js and the queue backs up somewhere
 *      (or you silently pile up huge JS call overhead).
 *
 * Pulling with drain() sidesteps all three: pywebview serialises the
 * *return value* itself (no hand-rolled escaping), the UI decides its own
 * cadence (rAF-aligned, so we never do more work than the browser can
 * paint), and a full outbox just means older buffered events get dropped
 * with a visible counter instead of growing unbounded. Python still pokes
 * the UI via evaluate_js, but only a tiny fixed nudge ("something's ready"),
 * throttled to 20/s -- not the payload.
 *
 * If packet volume ever outgrows this (very high pps, or you want the
 * option of testing the UI against a plain `vite dev` browser tab instead
 * of only inside the pywebview shell), the next step up is a small local
 * WebSocket/SSE server in the Python process that React connects to
 * directly -- same pull-vs-push tradeoffs don't apply since the browser's
 * WebSocket buffering and backpressure are already built for this, and it
 * decouples the data plane from pywebview entirely (control plane can stay
 * on js_api). Worth doing straight away if you want packets to keep
 * flowing while devtools/the window is unfocused, since some platforms
 * throttle rAF (and evaluate_js nudges) in background tabs/windows.
 * ---------------------------------------------------------------------
 */
