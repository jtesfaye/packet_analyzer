import { useCallback, useEffect, useRef, useState } from "react"
import { Packet } from "../data/mockPackets"
import { SessionConfig } from "../types"
import { fromCoreJson } from "../lib/packets"
import {
  control,
  packetStream,
  RawEvent,
  SnifferStatus,
} from "../lib/snifferBridge"

const MAX_BUFFERED_PACKETS = 50_000

const IDLE_STATUS: SnifferStatus = {
  ok: true,
  state: "idle",
  epoch: 0,
  error: null,
  buffered: 0,
  dropped: 0,
}

/** One layer of a get_details() response, matching packet::to_json(ProtocolDetails). */
export interface CoreDetailLayer {
  name: string
  fields: string[]
}

/**
 * Owns the pywebview control calls and the pulled packet stream, and
 * adapts both into shapes the existing components can render. Detail
 * lookups (by packet id) are kept separate from the summary `packets`
 * list -- see the note below on why DetailPane can't be wired to this
 * as-is yet.
 */
export function useSniffer() {
  const [status, setStatus] = useState<SnifferStatus>(IDLE_STATUS)
  const [packets, setPackets] = useState<Packet[]>([])
  const [details, setDetails] = useState<Record<number, CoreDetailLayer[]>>({})
  const pendingDetailIds = useRef<number[]>([])

  useEffect(() => {
    return packetStream.subscribe((events: RawEvent[]) => {
      const newPackets: Packet[] = []
      const newDetails: Array<[number, CoreDetailLayer[]]> = []

      for (const ev of events) {
        try {
          if (ev.kind === "packet") {
            newPackets.push(fromCoreJson(ev.data))
          } else if (ev.kind === "detail" && ev.packetId != null) {
            newDetails.push([ev.packetId, JSON.parse(ev.data)])
          }
        } catch (err) {
          console.error(`sniffer: malformed ${ev.kind} payload`, err, ev.data)
        }
      }

      if (newPackets.length) {
        setPackets((prev) => {
          const merged = prev.length ? prev.concat(newPackets) : newPackets
          return merged.length > MAX_BUFFERED_PACKETS
            ? merged.slice(merged.length - MAX_BUFFERED_PACKETS)
            : merged
        })
      }
      if (newDetails.length) {
        setDetails((prev) => {
          const next = { ...prev }
          for (const [id, layers] of newDetails) next[id] = layers
          return next
        })
      }
    })
  }, [])

  const start = useCallback(async (cfg: SessionConfig) => {
    const res = await control.start(cfg)
    setStatus(res)
    if (res.ok) {
      setPackets([])
      setDetails({})
      packetStream.begin(res.epoch)
    }
    return res
  }, [])

  const pause = useCallback(async () => {
    const res = await control.pause()
    setStatus(res)
    return res
  }, [])

  const resume = useCallback(async () => {
    const res = await control.resume()
    setStatus(res)
    return res
  }, [])

  const stop = useCallback(async () => {
    packetStream.end()
    const res = await control.stop()
    setStatus(res)
    return res
  }, [])

  const save = useCallback((path: string) => control.save(path), [])

  // FIFO-correlated with the C++ Detail queue -- see the note in
  // main.py::get_details about why this only works if requests aren't
  // fired faster than the responses come back.
  const requestDetails = useCallback((packetId: number) => {
    pendingDetailIds.current.push(packetId)
    return control.getDetails(packetId)
  }, [])

  return {
    status,
    packets,
    details,
    start,
    pause,
    resume,
    stop,
    save,
    requestDetails,
  }
}

/*
 * Heads up: DetailPane currently renders `packet.headers` (a flat
 * Record<string,string>) and `packet.payload` (a hex dump string), but
 * what get_details() actually returns is a list of {name, fields:string[]}
 * layers (see CoreDetailLayer above / gui.py's QTreeWidget, which renders
 * that shape directly) and the C++ core doesn't emit raw packet bytes
 * anywhere yet. Wiring real capture data into DetailPane needs one of:
 *   - a DetailPane redesign to show the layer/field tree it's actually
 *     given (closer to gui.py's tree widget), or
 *   - extending ProtocolDetails/packet::to_json on the C++ side to
 *     include a hex/base64 payload if you want to keep the hex-dump UI.
 * Left alone here rather than force-fitting `details` into `headers`,
 * which would just mislabel the data.
 */
