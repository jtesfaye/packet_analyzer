import { Protocol } from "../data/mockPackets"
import { PROTOCOLS } from "../constants"
import { PROTO_COLOR } from "../theme"

export function StatusBar({ counts }: { counts: Record<Protocol, number> }) {
  return (
    <div
      className="flex items-center gap-4 px-4 shrink-0"
      style={{
        height: 24,
        background: "#13151A",
        borderTop: "1px solid rgba(44,49,56,0.4)",
        boxShadow: "0 -1px 6px rgba(0,0,0,0.2)",
      }}
    >
      {PROTOCOLS.filter((p) => counts[p] > 0).map((p) => (
        <span key={p} className="font-mono text-[10px] flex items-center gap-1">
          <span style={{ color: PROTO_COLOR[p] }}>{p}</span>
          <span style={{ color: "#8A9099" }}>{counts[p]}</span>
        </span>
      ))}
      <span
        className="ml-auto font-mono text-[10px]"
        style={{ color: "#8A9099" }}
      >
        eth0 · promiscuous
      </span>
    </div>
  )
}
