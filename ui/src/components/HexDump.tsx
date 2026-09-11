export function HexDump({ raw }: { raw: string }) {
  return (
    <pre
      className="font-mono text-[11px] leading-5 overflow-auto p-3 rounded"
      style={{ background: "#13151A", color: "#8A9099", maxHeight: 160 }}
    >
      {raw.split("\n").map((line, i) => (
        <span key={i} className="block">
          <span style={{ color: "#3FA3A0", userSelect: "none" }}>
            {String(i * 16).padStart(4, "0")}{" "}
          </span>
          <span style={{ color: "#7EC8E3" }}>{line}</span>
        </span>
      ))}
    </pre>
  )
}
