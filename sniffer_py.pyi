# sniffer_py.pyi

from typing import Any

class EventMessage:
    type: int
    message: str

class CaptureMode:
    Online: int
    Offline: int

class EventMessageType:
    Packet: int
    Detail: int
    Stat: int

class CaptureConfig:
    mode: int
    source: str
    packet_count: int
    capture_size: int
    settings: int
    flags: int
    filter: str

    def __init__(self) -> None: ...


class CommandType:
    Start: int
    Stop: int
    Save: int
    End: int


class SessionCommand:
    type: int
    arg: Any

    @staticmethod
    def start(config: CaptureConfig) -> "SessionCommand": ...

    @staticmethod
    def stop() -> "SessionCommand": ...

    @staticmethod
    def save(path: str) -> "SessionCommand": ...

    @staticmethod
    def end() -> "SessionCommand": ...

    @staticmethod
    def get_details(id: int) -> "SessionCommand": ...


class CaptureSession:
    def __init__(self) -> None: ...

    def send_command(self, cmd: SessionCommand) -> None: ...

    def poll(self) -> EventMessage: ...