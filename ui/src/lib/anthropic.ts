import Anthropic from "@anthropic-ai/sdk"

export const ANTHROPIC_API_KEY: string =
  import.meta.env.VITE_ANTHROPIC_API_KEY ?? ""

export const anthropic = new Anthropic({
  apiKey: ANTHROPIC_API_KEY,
  dangerouslyAllowBrowser: true,
})

export const CHAT_MODEL = "claude-opus-5"

export const CHAT_SYSTEM_PROMPT =
  "You are a network analysis assistant embedded in a packet analyzer tool. Help the user understand " +
  "network traffic, protocols, and packet data. Be concise and technical."
