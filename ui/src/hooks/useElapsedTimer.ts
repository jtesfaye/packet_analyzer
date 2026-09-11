import { useCallback, useEffect, useState } from "react"

interface ElapsedTimer {
  elapsed: number
  reset: () => void
}

/**
 * Counts up one second at a time while `running` is true. Pausing (running
 * flips to false) freezes the count; `reset` zeroes it.
 */
export function useElapsedTimer(running: boolean): ElapsedTimer {
  const [elapsed, setElapsed] = useState(0)

  useEffect(() => {
    if (!running) return
    const id = setInterval(() => setElapsed((t) => t + 1), 1000)
    return () => clearInterval(id)
  }, [running])

  const reset = useCallback(() => setElapsed(0), [])

  return { elapsed, reset }
}
