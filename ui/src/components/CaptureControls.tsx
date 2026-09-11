import { CaptureState } from "../types"

interface Props {
  captureState: CaptureState
  packetCount: number
  onStart: () => void
  onPause: () => void
  onStop: () => void
  onSave: () => void
}

export function CaptureControls({
  captureState,
  packetCount,
  onStart,
  onPause,
  onStop,
  onSave,
}: Props) {
  const running = captureState === "running"
  const idle = captureState === "idle"

  return (
    <div className="flex items-center gap-1">
      {/* Start */}
      <button
        onClick={onStart}
        disabled={running}
        className="flex items-center gap-1.5 font-mono text-[11px] px-3 h-7 rounded transition-colors"
        style={{
          background: running ? "transparent" : "#3D997022",
          border: `1px solid ${running ? "rgba(44,49,56,0.4)" : "#3D997055"}`,
          color: running ? "#8A9099" : "#3D9970",
          cursor: running ? "not-allowed" : "pointer",
        }}
      >
        <svg width="9" height="10" viewBox="0 0 9 10" fill="currentColor">
          <polygon points="0,0 9,5 0,10" />
        </svg>
        Start
      </button>

      {/* Pause */}
      <button
        onClick={onPause}
        disabled={!running}
        className="flex items-center gap-1.5 font-mono text-[11px] px-3 h-7 rounded transition-colors"
        style={{
          background: captureState === "paused" ? "#C9A83C22" : "transparent",
          border: `1px solid ${running ? "#C9A83C55" : "rgba(44,49,56,0.4)"}`,
          color: running ? "#C9A83C" : "#8A9099",
          cursor: !running ? "not-allowed" : "pointer",
        }}
      >
        <svg width="9" height="10" viewBox="0 0 9 10" fill="currentColor">
          <rect x="0" y="0" width="3" height="10" />
          <rect x="6" y="0" width="3" height="10" />
        </svg>
        Pause
      </button>

      {/* Stop */}
      <button
        onClick={onStop}
        disabled={idle}
        className="flex items-center gap-1.5 font-mono text-[11px] px-3 h-7 rounded transition-colors"
        style={{
          background: "transparent",
          border: `1px solid ${!idle ? "#C0392B66" : "rgba(44,49,56,0.4)"}`,
          color: !idle ? "#C0392B" : "#8A9099",
          cursor: idle ? "not-allowed" : "pointer",
        }}
      >
        <svg width="9" height="9" viewBox="0 0 9 9" fill="currentColor">
          <rect x="0" y="0" width="9" height="9" />
        </svg>
        Stop
      </button>

      {/* Save */}
      <button
        onClick={onSave}
        disabled={idle && packetCount === 0}
        className="flex items-center gap-1.5 font-mono text-[11px] px-3 h-7 rounded transition-colors"
        style={{
          background: "transparent",
          border: `1px solid ${
            packetCount > 0 ? "rgba(44,49,56,0.6)" : "rgba(44,49,56,0.3)"
          }`,
          color: packetCount > 0 ? "#8A9099" : "#4A5058",
          cursor: packetCount === 0 ? "not-allowed" : "pointer",
        }}
      >
        <svg width="10" height="10" viewBox="0 0 10 10" fill="currentColor">
          <path d="M0 0h7l3 3v7H0V0zm1 1v8h8V3.4L6.6 1H1zm2 0h3v3H3V1zm4 5H3v3h4V6z" />
        </svg>
        Save
      </button>
    </div>
  )
}
