export type Protocol = "TCP" | "UDP" | "HTTP" | "HTTPS" | "DNS" | "ARP" | "ICMP" | "TLS"

export interface Packet {
  id: number
  timestamp: string
  src: string
  dst: string
  srcPort: number | null
  dstPort: number | null
  protocol: Protocol
  length: number
  info: string
  ttl: number
  flags: string[]
  payload: string
  headers: Record<string, string>
}

const protocols: Protocol[] = [
  "TCP",
  "UDP",
  "HTTP",
  "HTTPS",
  "DNS",
  "ARP",
  "ICMP",
  "TLS",
]

const ips = [
  "192.168.1.104",
  "192.168.1.1",
  "10.0.0.23",
  "172.16.0.5",
  "8.8.8.8",
  "1.1.1.1",
  "93.184.216.34",
  "151.101.1.140",
  "216.58.214.46",
  "104.18.25.95",
]

const infos: Record<Protocol, string[]> = {
  TCP: ["SYN", "SYN-ACK", "ACK", "FIN, ACK", "RST, ACK", "PSH, ACK"],
  UDP: ["Source port: 5353", "Source port: 68", "QUIC INITIAL"],
  HTTP: [
    "GET /index.html HTTP/1.1",
    "POST /api/v1/data HTTP/1.1",
    "HTTP/1.1 200 OK",
    "HTTP/1.1 404 Not Found",
  ],
  HTTPS: ["Application Data", "Client Hello", "Server Hello", "Certificate"],
  DNS: [
    "Standard query A google.com",
    "Standard query response A 142.250.64.46",
    "Standard query AAAA cloudflare.com",
  ],
  ARP: [
    "Who has 192.168.1.1? Tell 192.168.1.104",
    "Reply 192.168.1.1 is at dc:a6:32:4b:11:22",
  ],
  ICMP: [
    "Echo (ping) request id=0x0001",
    "Echo (ping) reply id=0x0001",
    "Destination unreachable",
  ],
  TLS: [
    "TLSv1.3 Record Layer: Handshake",
    "Change Cipher Spec",
    "Encrypted Alert",
  ],
}

function rnd<T>(arr: T[]): T {
  return arr[Math.floor(Math.random() * arr.length)]
}

function randPort(proto: Protocol): number | null {
  if (proto === "ARP" || proto === "ICMP") return null
  const well: Record<string, number> = { HTTP: 80, HTTPS: 443, DNS: 53 }
  return well[proto] ?? Math.floor(Math.random() * 60000 + 1024)
}

let seq = 1
let baseMs = Date.now() - 60000

function makePacket(): Packet {
  const proto = rnd(protocols)
  const src = rnd(ips)
  const dst = rnd(ips.filter((ip) => ip !== src))
  const srcPort = randPort(proto)
  const dstPort = randPort(proto)
  baseMs += Math.floor(Math.random() * 120 + 5)

  const d = new Date(baseMs)
  const ts = `${String(d.getHours()).padStart(2, "0")}:${String(d.getMinutes()).padStart(2, "0")}:${String(d.getSeconds()).padStart(2, "0")}.${String(d.getMilliseconds()).padStart(3, "0")}`

  const headers: Record<string, string> = {
    "Frame Length": `${Math.floor(Math.random() * 1400 + 60)} bytes`,
    "Ethernet II": `Src: dc:a6:32:4b:11:22, Dst: 00:1a:2b:3c:4d:5e`,
    "IP Version": "4",
    TTL: String(Math.floor(Math.random() * 128 + 30)),
    Checksum: `0x${Math.floor(Math.random() * 0xffff)
      .toString(16)
      .padStart(4, "0")}`,
  }

  if (srcPort) headers["Source Port"] = String(srcPort)
  if (dstPort) headers["Destination Port"] = String(dstPort)
  if (proto === "HTTP" || proto === "HTTPS") {
    headers["Host"] = dst
    headers["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)"
    headers["Accept"] = "text/html,application/xhtml+xml"
  }

  const flags =
    proto === "TCP"
      ? rnd([["SYN"], ["ACK"], ["SYN", "ACK"], ["FIN", "ACK"], ["PSH", "ACK"]])
      : []

  const payload =
    `45 00 00 3c ${Math.floor(Math.random() * 0xff)
      .toString(16)
      .padStart(2, "0")} ${Math.floor(Math.random() * 0xff)
      .toString(16)
      .padStart(2, "0")} 40 00 40 06\n` +
    `${Array.from({ length: 16 }, () =>
      Math.floor(Math.random() * 0xff)
        .toString(16)
        .padStart(2, "0"),
    ).join(" ")}\n` +
    `${Array.from({ length: 16 }, () =>
      Math.floor(Math.random() * 0xff)
        .toString(16)
        .padStart(2, "0"),
    ).join(" ")}`

  return {
    id: seq++,
    timestamp: ts,
    src,
    dst,
    srcPort,
    dstPort,
    protocol: proto,
    length: Math.floor(Math.random() * 1400 + 60),
    info: rnd(infos[proto]),
    ttl: Math.floor(Math.random() * 128 + 30),
    flags,
    payload,
    headers,
  }
}

export const MOCK_PACKETS: Packet[] = Array.from({ length: 120 }, makePacket)
