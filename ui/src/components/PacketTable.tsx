import { useState } from "react"
import { Packet } from "../data/mockPackets"
import { ProtoTag } from "./ProtoTag"

const GRID_COLUMNS = "52px 110px 160px 160px 70px 70px 1fr"

const COLUMNS = ["#", "Time", "Source", "Destination", "Proto", "Len", "Info"]

export function PacketTable({
  packets,
  onSelect,
}: {
  packets: Packet[]
  onSelect: (p: Packet) => void
}) {
  const [hoveredRow, setHoveredRow] = useState<number | null>(null)

  return (
    <>
      {/* Column headers */}
      <div
        className="grid shrink-0 font-mono text-[10px] uppercase tracking-wider px-3"
        style={{
          gridTemplateColumns: GRID_COLUMNS,
          height: 28,
          background: "#13151A",
          borderBottom: "1px solid rgba(44,49,56,0.35)",
          boxShadow: "0 2px 6px rgba(0,0,0,0.22)",
          color: "#8A9099",
          alignItems: "center",
          gap: 8,
        }}
      >
        {COLUMNS.map((c) => (
          <span key={c}>{c}</span>
        ))}
      </div>

      {/* Packet list */}
      <div className="flex-1 overflow-y-auto">
        {packets.map((pkt, i) => {
          const isEven = i % 2 === 0
          const isHovered = hoveredRow === pkt.id
          return (
            <div
              key={pkt.id}
              onClick={() => onSelect(pkt)}
              onMouseEnter={() => setHoveredRow(pkt.id)}
              onMouseLeave={() => setHoveredRow(null)}
              className="grid cursor-pointer font-mono text-xs px-3"
              style={{
                gridTemplateColumns: GRID_COLUMNS,
                height: 26,
                alignItems: "center",
                gap: 8,
                background: isHovered
                  ? "#252A31"
                  : isEven
                    ? "#16181D"
                    : "#191C21",
                borderBottom: "1px solid rgba(30,33,38,0.7)",
                transition: "background 0.07s",
              }}
            >
              <span style={{ color: "#8A9099" }}>{pkt.id}</span>
              <span style={{ color: "#8A9099" }}>{pkt.timestamp}</span>
              <span style={{ color: "#7EC8E3" }} className="truncate">
                {pkt.src}
                {pkt.srcPort ? `:${pkt.srcPort}` : ""}
              </span>
              <span style={{ color: "#A3C9A8" }} className="truncate">
                {pkt.dst}
                {pkt.dstPort ? `:${pkt.dstPort}` : ""}
              </span>
              <span>
                <ProtoTag proto={pkt.protocol} />
              </span>
              <span style={{ color: "#8A9099" }}>{pkt.length}</span>
              <span style={{ color: "#E6E8EC" }} className="truncate">
                {pkt.info}
              </span>
            </div>
          )
        })}
      </div>
    </>
  )
}
