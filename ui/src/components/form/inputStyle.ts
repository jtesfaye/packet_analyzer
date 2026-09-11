import type { CSSProperties } from "react"

export const inputStyle: CSSProperties = {
  background: "#13151A",
  border: "1px solid rgba(44,49,56,0.7)",
  color: "#E6E8EC",
  borderRadius: 4,
  fontFamily: "var(--font-mono, monospace)",
  fontSize: 12,
  padding: "5px 8px",
  outline: "none",
  width: "100%",
}

export const selectStyle: CSSProperties = {
  ...inputStyle,
  appearance: "none",
  cursor: "pointer",
}
