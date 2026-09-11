import { useEffect, useRef, useState } from "react"
import {
  anthropic,
  ANTHROPIC_API_KEY,
  CHAT_MODEL,
  CHAT_SYSTEM_PROMPT,
} from "../lib/anthropic"
import { ChatMessage } from "../types"

export function ChatPanel() {
  const [messages, setMessages] = useState<ChatMessage[]>([])
  const [input, setInput] = useState("")
  const [loading, setLoading] = useState(false)
  const [streamingText, setStreamingText] = useState("")
  const scrollRef = useRef<HTMLDivElement>(null)

  useEffect(() => {
    if (scrollRef.current)
      scrollRef.current.scrollTop = scrollRef.current.scrollHeight
  }, [messages, streamingText])

  const send = async () => {
    const text = input.trim()
    if (!text || loading) return
    setInput("")
    const userMsg: ChatMessage = { role: "user", content: text }
    const history = [...messages, userMsg]
    setMessages(history)
    setLoading(true)
    setStreamingText("")

    try {
      if (!ANTHROPIC_API_KEY) {
        // Stub response when no API key
        await new Promise((r) => setTimeout(r, 600))
        const stub = `I'm a packet analysis assistant. You asked: "${text}" — connect an Anthropic API key via VITE_ANTHROPIC_API_KEY to get real responses.`
        setMessages([...history, { role: "assistant", content: stub }])
        setLoading(false)
        return
      }

      const stream = await anthropic.messages.stream({
        model: CHAT_MODEL,
        max_tokens: 1024,
        thinking: { type: "adaptive" },
        system: CHAT_SYSTEM_PROMPT,
        messages: history.map((m) => ({ role: m.role, content: m.content })),
      })

      let full = ""
      for await (const event of stream) {
        if (
          event.type === "content_block_delta" &&
          event.delta.type === "text_delta"
        ) {
          full += event.delta.text
          setStreamingText(full)
        }
      }
      setMessages([...history, { role: "assistant", content: full }])
      setStreamingText("")
    } catch (err) {
      const errMsg = err instanceof Error ? err.message : "Unknown error"
      setMessages([
        ...history,
        { role: "assistant", content: `Error: ${errMsg}` },
      ])
      setStreamingText("")
    }
    setLoading(false)
  }

  return (
    <div
      className="shrink-0 flex flex-col"
      style={{
        height: 200,
        background: "#1A1D22",
        borderTop: "1px solid rgba(44,49,56,0.45)",
        boxShadow: "0 -1px 0 rgba(0,0,0,0.18)",
      }}
    >
      {/* Chat header */}
      <div
        className="flex items-center gap-2 px-4 shrink-0"
        style={{
          height: 28,
          background: "#13151A",
          borderBottom: "1px solid rgba(44,49,56,0.35)",
        }}
      >
        <span
          className="font-mono text-[10px] uppercase tracking-wider"
          style={{ color: "#3FA3A0" }}
        >
          ai assistant
        </span>
        {loading && (
          <span className="font-mono text-[10px]" style={{ color: "#8A9099" }}>
            thinking…
          </span>
        )}
      </div>

      {/* Message history */}
      <div
        ref={scrollRef}
        className="flex-1 overflow-y-auto px-4 py-2 flex flex-col gap-1.5"
      >
        {messages.length === 0 && !streamingText && (
          <span
            className="font-mono text-[11px] self-center mt-auto mb-auto"
            style={{ color: "#4A5058" }}
          >
            Ask a question about your packet capture…
          </span>
        )}
        {messages.map((m, i) => (
          <div
            key={i}
            className={`flex ${
              m.role === "user" ? "justify-end" : "justify-start"
            }`}
          >
            <div
              className="font-mono text-[11px] leading-5 px-2.5 py-1 rounded max-w-[80%]"
              style={
                m.role === "user"
                  ? {
                      background: "#3FA3A022",
                      border: "1px solid #3FA3A033",
                      color: "#A8D8D7",
                    }
                  : {
                      background: "#1E2126",
                      border: "1px solid rgba(44,49,56,0.5)",
                      color: "#E6E8EC",
                    }
              }
            >
              {m.content}
            </div>
          </div>
        ))}
        {streamingText && (
          <div className="flex justify-start">
            <div
              className="font-mono text-[11px] leading-5 px-2.5 py-1 rounded max-w-[80%]"
              style={{
                background: "#1E2126",
                border: "1px solid rgba(44,49,56,0.5)",
                color: "#E6E8EC",
              }}
            >
              {streamingText}
              <span
                className="inline-block w-1.5 h-3 ml-0.5 align-middle"
                style={{
                  background: "#3FA3A0",
                  animation: "blink 1s step-end infinite",
                }}
              />
            </div>
          </div>
        )}
      </div>

      {/* Input row */}
      <div
        className="flex items-center gap-2 px-3 py-2 shrink-0"
        style={{ borderTop: "1px solid rgba(44,49,56,0.35)" }}
      >
        <input
          type="text"
          placeholder="Ask about protocols, addresses, traffic patterns…"
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyDown={(e) => {
            if (e.key === "Enter" && !e.shiftKey) {
              e.preventDefault()
              send()
            }
          }}
          disabled={loading}
          className="flex-1 font-mono text-xs rounded outline-none px-3 h-7"
          style={{
            background: "#13151A",
            border: "1px solid rgba(44,49,56,0.6)",
            color: "#E6E8EC",
          }}
        />
        <button
          onClick={send}
          disabled={loading || !input.trim()}
          className="font-mono text-[11px] px-3 h-7 rounded transition-colors flex items-center gap-1.5"
          style={{
            background: loading || !input.trim() ? "transparent" : "#3FA3A022",
            border: `1px solid ${
              loading || !input.trim() ? "rgba(44,49,56,0.4)" : "#3FA3A055"
            }`,
            color: loading || !input.trim() ? "#4A5058" : "#3FA3A0",
            cursor: loading || !input.trim() ? "not-allowed" : "pointer",
          }}
        >
          Send
        </button>
      </div>
    </div>
  )
}
