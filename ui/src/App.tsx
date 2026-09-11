import { useCallback, useMemo, useState } from "react"
import { MOCK_PACKETS, Packet } from "./data/mockPackets"
import { CaptureState, SessionConfig } from "./types"
import { filterPackets, protocolCounts } from "./lib/packets"
import { useElapsedTimer } from "./hooks/useElapsedTimer"
import { TopBar } from "./components/TopBar"
import { PacketTable } from "./components/PacketTable"
import { StatusBar } from "./components/StatusBar"
import { ChatPanel } from "./components/ChatPanel"
import { DetailPane } from "./components/DetailPane"
import { NewSessionModal } from "./components/NewSessionModal"
import { control } from "./lib/snifferBridge"

export default function App() {
  const [selected, setSelected] = useState<Packet | null>(null)
  const [filter, setFilter] = useState("")
  const [captureState, setCaptureState] = useState<CaptureState>("idle")
  const [showNewSession, setShowNewSession] = useState(false)
  const [sessionConfig, setSessionConfig] = useState<SessionConfig | null>(null)

  const { elapsed, reset: resetElapsed } = useElapsedTimer(
    captureState === "running",
  )

  const handleStart = () => {
    if (captureState === "idle") {
      setShowNewSession(true)
      return
    }
    setCaptureState("running")
  }
  const handleSessionStart = (cfg: SessionConfig) => {
    setSessionConfig(cfg)
    // Fire-and-forget from the UI's perspective: start() resolves once the
    // capture thread is spun up, with { ok, error, epoch, ... }. Errors
    // (bad interface, capture already running, ...) aren't surfaced here
    // yet -- see useSniffer() for a version that feeds status/errors back
    // into state and wires up the packet stream via packetStream.begin().
    control
      .start(cfg)
      .catch((err) => console.error("failed to start capture", err))
    setShowNewSession(false)
    setCaptureState("running")
    resetElapsed()
  }
  const handlePause = () => setCaptureState("paused")
  const handleStop = () => {
    setCaptureState("idle")
    resetElapsed()
    setSessionConfig(null)
  }
  const handleSave = () => {}

  const closeDetail = useCallback(() => setSelected(null), [])

  const packets = useMemo(() => filterPackets(MOCK_PACKETS, filter), [filter])
  const counts = useMemo(() => protocolCounts(MOCK_PACKETS), [])

  return (
    <div
      className="flex flex-col"
      style={{ height: "100vh", background: "#16181D", color: "#E6E8EC" }}
    >
      <TopBar
        sessionConfig={sessionConfig}
        captureState={captureState}
        elapsed={elapsed}
        filter={filter}
        onFilterChange={setFilter}
        packetCount={packets.length}
        onStart={handleStart}
        onPause={handlePause}
        onStop={handleStop}
        onSave={handleSave}
      />

      <PacketTable packets={packets} onSelect={setSelected} />

      <ChatPanel />

      <StatusBar counts={counts} />

      {selected && <DetailPane packet={selected} onClose={closeDetail} />}
      {showNewSession && (
        <NewSessionModal
          onStart={handleSessionStart}
          onCancel={() => setShowNewSession(false)}
        />
      )}
    </div>
  )
}
