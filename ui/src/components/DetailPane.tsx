import { useRef } from "react"
import { Packet } from "../data/mockPackets"
import { useClickOutside } from "../hooks/useClickOutside"
import { ProtoTag } from "./ProtoTag"
import { HexDump } from "./HexDump"

const SECTION_LABEL: React.CSSProperties = {
  color: "#8A9099",
  letterSpacing: "0.06em",
  textTransform: "uppercase",
}

export function DetailPane({
  packet,
  onClose,
}: {
  packet: Packet
  onClose: () => void
}) {
  const ref = useRef<HTMLDivElement>(null)
  useClickOutside(ref, onClose)

  return (
    <div
      className="fixed inset-0 z-50 flex items-center justify-center"
      style={{ background: "rgba(0,0,0,0.55)" }}
    >
      <div
        ref={ref}
        className="flex flex-col rounded overflow-hidden shadow-2xl"
        style={{
          width: 680,
          maxHeight: "88vh",
          background: "#1E2126",
          border: "1px solid rgba(44,49,56,0.5)",
        }}
      >
        {/* Header */}
        <div
          className="flex items-center justify-between px-4 py-2 shrink-0"
          style={{
            background: "#16181D",
            borderBottom: "1px solid rgba(44,49,56,0.5)",
            boxShadow: "0 1px 4px rgba(0,0,0,0.2)",
          }}
        >
          <div className="flex items-center gap-3">
            <ProtoTag proto={packet.protocol} />
            <span className="font-mono text-xs" style={{ color: "#8A9099" }}>
              #{packet.id}
            </span>
            <span className="font-mono text-xs" style={{ color: "#8A9099" }}>
              {packet.timestamp}
            </span>
          </div>
          <button
            onClick={onClose}
            className="text-lg leading-none transition-colors"
            style={{ color: "#8A9099" }}
            onMouseEnter={(e) => (e.currentTarget.style.color = "#E6E8EC")}
            onMouseLeave={(e) => (e.currentTarget.style.color = "#8A9099")}
          >
            ×
          </button>
        </div>

        <div className="overflow-y-auto">
          {/* Route row */}
          <div
            className="flex items-center gap-2 px-4 py-3"
            style={{ borderBottom: "1px solid rgba(44,49,56,0.45)" }}
          >
            <span className="font-mono text-xs" style={{ color: "#7EC8E3" }}>
              {packet.src}
              {packet.srcPort ? `:${packet.srcPort}` : ""}
            </span>
            <span style={{ color: "#3FA3A0" }}>→</span>
            <span className="font-mono text-xs" style={{ color: "#A3C9A8" }}>
              {packet.dst}
              {packet.dstPort ? `:${packet.dstPort}` : ""}
            </span>
            <span
              className="ml-auto font-mono text-xs"
              style={{ color: "#8A9099" }}
            >
              {packet.length} bytes · TTL {packet.ttl}
            </span>
          </div>

          {/* Info */}
          <div
            className="px-4 py-2"
            style={{ borderBottom: "1px solid rgba(44,49,56,0.45)" }}
          >
            <div className="text-xs mb-1 font-medium" style={SECTION_LABEL}>
              Info
            </div>
            <div className="font-mono text-xs" style={{ color: "#E6E8EC" }}>
              {packet.info}
            </div>
            {packet.flags.length > 0 && (
              <div className="flex gap-1 mt-2">
                {packet.flags.map((f) => (
                  <span
                    key={f}
                    className="font-mono text-[10px] px-1.5 rounded"
                    style={{
                      background: "#3FA3A01A",
                      color: "#3FA3A0",
                      border: "1px solid #3FA3A033",
                    }}
                  >
                    {f}
                  </span>
                ))}
              </div>
            )}
          </div>

          {/* Headers */}
          <div
            className="px-4 py-2"
            style={{ borderBottom: "1px solid rgba(44,49,56,0.45)" }}
          >
            <div className="text-xs mb-2 font-medium" style={SECTION_LABEL}>
              Headers
            </div>
            <table className="w-full text-xs">
              <tbody>
                {Object.entries(packet.headers).map(([k, v]) => (
                  <tr
                    key={k}
                    style={{ borderBottom: "1px solid rgba(35,39,45,0.55)" }}
                  >
                    <td
                      className="py-1 pr-4 font-mono"
                      style={{
                        color: "#7EC8E3",
                        width: 180,
                        verticalAlign: "top",
                      }}
                    >
                      {k}
                    </td>
                    <td
                      className="py-1 font-mono"
                      style={{ color: "#A3C9A8", wordBreak: "break-all" }}
                    >
                      {v}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>

          {/* Hex dump */}
          <div className="px-4 py-2">
            <div className="text-xs mb-2 font-medium" style={SECTION_LABEL}>
              Raw Payload
            </div>
            <HexDump raw={packet.payload} />
          </div>
        </div>
      </div>
    </div>
  )
}
