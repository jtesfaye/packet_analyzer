import { Protocol } from "./data/mockPackets"

export const PROTOCOLS: Protocol[] = [
  "TCP",
  "UDP",
  "HTTP",
  "HTTPS",
  "DNS",
  "ARP",
  "ICMP",
  "TLS",
]

export const INTERFACES = ["eth0", "eth1", "wlan0", "lo", "any"]

export const SNAPLEN_OPTIONS = ["65535", "1500", "1024", "512"]
