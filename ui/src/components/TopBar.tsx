import { CaptureState, SessionConfig } from "../types"
import { formatElapsed } from "../lib/format"
import { CaptureControls } from "./CaptureControls"

interface Props {
  sessionConfig: SessionConfig | null
  captureState: CaptureState
  elapsed: number
  filter: string
  onFilterChange: (v: string) => void
  packetCount: number
  onStart: () => void
  onPause: () => void
  onStop: () => void
  onSave: () => void
}

const STATUS_COLOR: Record<CaptureState, string> = {
  running: "#3D9970",
  paused: "#C9A83C",
  idle: "#4A5058",
}

const STATUS_LABEL: Record<CaptureState, string> = {
  running: "capturing",
  paused: "paused",
  idle: "idle",
}

export function TopBar({
  sessionConfig,
  captureState,
  elapsed,
  filter,
  onFilterChange,
  packetCount,
  onStart,
  onPause,
  onStop,
  onSave,
}: Props) {
  return (
    <div
      className="flex items-center gap-4 px-4 shrink-0"
      style={{
        height: 40,
        background: "#13151A",
        borderBottom: "1px solid rgba(44,49,56,0.45)",
        boxShadow: "0 1px 0 rgba(0,0,0,0.18)",
      }}
    >
      <span
        className="font-mono text-xs font-medium tracking-widest uppercase"
        style={{ color: "#3FA3A0" }}
      >
        pkt/analyze
      </span>
      {sessionConfig?.label && (
        <span className="font-mono text-xs" style={{ color: "#8A9099" }}>
          {sessionConfig.label}
        </span>
      )}
      {sessionConfig && (
        <span className="font-mono text-[10px]" style={{ color: "#4A5058" }}>
          {sessionConfig.interface}
          {sessionConfig.bpfFilter ? ` · ${sessionConfig.bpfFilter}` : ""}
        </span>
      )}
      <div style={{ width: 1, height: 16, background: "rgba(44,49,56,0.6)" }} />

      <CaptureControls
        captureState={captureState}
        packetCount={packetCount}
        onStart={onStart}
        onPause={onPause}
        onStop={onStop}
        onSave={onSave}
      />

      <div style={{ width: 1, height: 16, background: "rgba(44,49,56,0.5)" }} />

      {/* Filter */}
      <div className="relative flex items-center">
        <span
          className="absolute left-2 font-mono text-xs"
          style={{ color: "#8A9099" }}
        >
          ⌕
        </span>
        <input
          type="text"
          placeholder="filter packets…"
          value={filter}
          onChange={(e) => onFilterChange(e.target.value)}
          className="font-mono text-xs pl-6 pr-3 rounded outline-none h-7"
          style={{
            background: "#1E2126",
            border: "1px solid rgba(44,49,56,0.5)",
            color: "#E6E8EC",
            width: 200,
          }}
        />
      </div>

      <div className="ml-auto flex items-center gap-3">
        {/* Elapsed timer */}
        <span
          className="font-mono text-xs tabular-nums"
          style={{ color: captureState === "running" ? "#3FA3A0" : "#8A9099" }}
        >
          {formatElapsed(elapsed)}
        </span>
        {/* Status dot */}
        <div className="flex items-center gap-1.5">
          <span
            className="inline-block rounded-full"
            style={{
              width: 7,
              height: 7,
              background: STATUS_COLOR[captureState],
              boxShadow:
                captureState === "running" ? "0 0 0 2px #3D997033" : "none",
            }}
          />
          <span
            className="font-mono text-[10px] uppercase tracking-wider"
            style={{ color: STATUS_COLOR[captureState] }}
          >
            {STATUS_LABEL[captureState]}
          </span>
        </div>
        <span className="font-mono text-xs" style={{ color: "#8A9099" }}>
          {packetCount} pkts
        </span>
      </div>
    </div>
  )
}
