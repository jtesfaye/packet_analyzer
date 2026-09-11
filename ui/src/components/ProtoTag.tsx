import { Protocol } from "../data/mockPackets"
import { PROTO_COLOR } from "../theme"

export function ProtoTag({ proto }: { proto: Protocol }) {
  return (
    <span
      className="inline-block px-1.5 rounded font-mono text-[10px] font-medium leading-5"
      style={{
        color: PROTO_COLOR[proto],
        background: PROTO_COLOR[proto] + "1A",
        border: `1px solid ${PROTO_COLOR[proto]}33`,
      }}
    >
      {proto}
    </span>
  )
}
