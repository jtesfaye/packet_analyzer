export function Toggle({
  checked,
  onChange,
}: {
  checked: boolean
  onChange: (v: boolean) => void
}) {
  return (
    <button
      type="button"
      onClick={() => onChange(!checked)}
      className="relative shrink-0 rounded-full transition-colors"
      style={{
        width: 32,
        height: 16,
        background: checked ? "#3FA3A0" : "#2C3138",
        border: `1px solid ${checked ? "#3FA3A055" : "rgba(44,49,56,0.5)"}`,
      }}
    >
      <span
        className="absolute top-0.5 rounded-full transition-transform"
        style={{
          width: 12,
          height: 12,
          background: checked ? "#E6E8EC" : "#8A9099",
          left: 1,
          transform: checked ? "translateX(16px)" : "translateX(0)",
        }}
      />
    </button>
  )
}
