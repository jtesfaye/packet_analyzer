import type { ReactNode } from "react"

export function Field({
  label,
  hint,
  children,
}: {
  label: string
  hint?: string
  children: ReactNode
}) {
  return (
    <div className="flex flex-col gap-1">
      <div className="flex items-baseline gap-2">
        <label
          className="font-mono text-[10px] uppercase tracking-wider"
          style={{ color: "#8A9099" }}
        >
          {label}
        </label>
        {hint && (
          <span className="font-mono text-[10px]" style={{ color: "#4A5058" }}>
            {hint}
          </span>
        )}
      </div>
      {children}
    </div>
  )
}
