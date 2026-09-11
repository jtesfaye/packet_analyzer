import { Packet, Protocol } from "../data/mockPackets"
import { PROTOCOLS } from "../constants"

// IANA protocol numbers, as reported by protocol::ProtocolKeys (see
// include/layerx/ProtocolKeys.h) and echoed into packet_data.protocol by
// packet::to_json. Anything not in this table falls back to the raw
// number as a string rather than guessing at a Protocol label.
const CORE_PROTOCOL_NUMBERS: Record<number, Protocol> = {
  1: "ICMP",
  6: "TCP",
  17: "UDP",
}

/**
 * Raw shape of one packet-summary message coming off the wire (see
 * packet::to_json in src/util/PacketUtil.cpp). This is the *summary*
 * stream only -- it has no raw bytes and no per-layer field breakdown,
 * so `payload`, `headers`, `ttl` and `flags` below are placeholders until
 * a get_details() response for this packet id comes back separately (see
 * useSniffer.ts). Don't read anything into those fields being empty.
 */
interface CorePacketJson {
  id: number
  time_sec: number | null
  time_usec: number | null
  wire_length: number | null
  payload_length: number | null
  src: string | null
  dest: string | null
  protocol: number | null
  desc: string | null
}

export function protocolFromCoreNumber(n: number | null): Protocol {
  if (n == null) return "TCP" // Protocol has no "unknown" bucket today
  return CORE_PROTOCOL_NUMBERS[n] ?? `PROTO-${n}` as unknown as Protocol
}

/** Adapts one summary-stream JSON string into the table's Packet shape. */
export function fromCoreJson(json: string): Packet {
  const raw: CorePacketJson = JSON.parse(json)
  const ts = raw.time_sec != null ? new Date(raw.time_sec * 1000) : null

  return {
    id: raw.id,
    timestamp: ts
      ? `${String(ts.getHours()).padStart(2, "0")}:${String(ts.getMinutes()).padStart(2, "0")}:${String(ts.getSeconds()).padStart(2, "0")}.${String(Math.floor((raw.time_usec ?? 0) / 1000)).padStart(3, "0")}`
      : "N/A",
    src: raw.src ?? "",
    dst: raw.dest ?? "",
    srcPort: null, // not broken out by packet::to_json today -- it's baked into `src`/`desc`
    dstPort: null,
    protocol: protocolFromCoreNumber(raw.protocol),
    length: raw.wire_length ?? 0,
    info: raw.desc ?? "",
    ttl: 0,
    flags: [],
    payload: "",
    headers: {},
  }
}

export function filterPackets(packets: Packet[], query: string): Packet[] {
  const q = query.toLowerCase()
  if (!q) return packets
  return packets.filter(
    (p) =>
      p.src.includes(q) ||
      p.dst.includes(q) ||
      p.info.toLowerCase().includes(q) ||
      p.protocol.toLowerCase().includes(q),
  )
}

export function protocolCounts(packets: Packet[]): Record<Protocol, number> {
  return PROTOCOLS.reduce(
    (acc, proto) => {
      acc[proto] = packets.filter((p) => p.protocol === proto).length
      return acc
    },
    {} as Record<Protocol, number>,
  )
}
