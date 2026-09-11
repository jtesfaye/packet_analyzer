import { useRef, useState, useEffect } from "react"
import { INTERFACES, SNAPLEN_OPTIONS } from "../constants"
import { DEFAULT_CONFIG, SessionConfig } from "../types"
import { useClickOutside } from "../hooks/useClickOutside"
import { Field } from "./form/Field"
import { Toggle } from "./form/Toggle"
import { inputStyle, selectStyle } from "./form/inputStyle"

const TOGGLES = [
  ["promiscuous", "Promiscuous mode", "capture all frames on the wire"],
  [
    "resolveNames",
    "Resolve hostnames",
    "reverse DNS lookup on source/dest IPs",
  ],
] as const

export function NewSessionModal({ onStart, onCancel }: { onStart: (cfg: SessionConfig) => void; onCancel: () => void }) {
  const [cfg, setCfg] = useState<SessionConfig>(DEFAULT_CONFIG);
  const ref = useRef<HTMLDivElement>(null);

  const set = <K extends keyof SessionConfig>(k: K, v: SessionConfig[K]) =>
    setCfg(prev => ({ ...prev, [k]: v }));

  useEffect(() => {
    const handler = (e: MouseEvent) => {
      if (ref.current && !ref.current.contains(e.target as Node)) onCancel();
    };
    document.addEventListener('mousedown', handler);
    return () => document.removeEventListener('mousedown', handler);
  }, [onCancel]);

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center" style={{ background: 'rgba(0,0,0,0.6)' }}>
      <div
        ref={ref}
        className="flex flex-col rounded overflow-hidden"
        style={{ width: 520, background: '#1E2126', border: '1px solid rgba(44,49,56,0.5)', boxShadow: '0 16px 48px rgba(0,0,0,0.55)' }}
      >
        {/* Header */}
        <div className="flex items-center justify-between px-5 py-3" style={{ background: '#16181D', borderBottom: '1px solid rgba(44,49,56,0.5)' }}>
          <div>
            <div className="font-mono text-xs font-medium" style={{ color: '#E6E8EC' }}>New Capture Session</div>
            <div className="font-mono text-[10px] mt-0.5" style={{ color: '#8A9099' }}>Configure capture parameters before starting</div>
          </div>
          <button onClick={onCancel} style={{ color: '#8A9099', fontSize: 18, lineHeight: 1 }}
            onMouseEnter={e => (e.currentTarget.style.color = '#E6E8EC')}
            onMouseLeave={e => (e.currentTarget.style.color = '#8A9099')}>×</button>
        </div>

        <div className="flex flex-col gap-4 px-5 py-4">
          {/* Label */}
          <Field label="Session Label" hint="optional">
            <input
              style={inputStyle}
              placeholder="e.g. prod-ingress-debug"
              value={cfg.label}
              onChange={e => set('label', e.target.value)}
            />
          </Field>

          {/* Interface + BPF side by side */}
          <div className="grid gap-4" style={{ gridTemplateColumns: '1fr 2fr' }}>
            <Field label="Interface">
              <select
                style={{ ...inputStyle, appearance: 'none', cursor: 'pointer' }}
                value={cfg.interface}
                onChange={e => set('interface', e.target.value)}
              >
                {INTERFACES.map(i => <option key={i} value={i}>{i}</option>)}
              </select>
            </Field>
            <Field label="BPF Filter" hint="e.g. tcp port 443">
              <input
                style={inputStyle}
                placeholder="tcp and host 192.168.1.1"
                value={cfg.bpfFilter}
                onChange={e => set('bpfFilter', e.target.value)}
              />
            </Field>
          </div>

          {/* Snaplen + Max packets */}
          <div className="grid gap-4" style={{ gridTemplateColumns: '1fr 1fr' }}>
            <Field label="Snap Length" hint="bytes">
              <select
                style={{ ...inputStyle, appearance: 'none', cursor: 'pointer' }}
                value={cfg.snaplen}
                onChange={e => set('snaplen', e.target.value)}
              >
                {SNAPLEN_OPTIONS.map(s => <option key={s} value={s}>{s}</option>)}
              </select>
            </Field>
            <Field label="Max Packets" hint="0 = unlimited">
              <input
                style={inputStyle}
                placeholder="0"
                type="number"
                min={0}
                value={cfg.maxPackets}
                onChange={e => set('maxPackets', e.target.value)}
              />
            </Field>
          </div>

          {/* Toggles */}
          <div className="flex flex-col gap-2 pt-1" style={{ borderTop: '1px solid rgba(44,49,56,0.4)' }}>
            {([
              ['promiscuous',    'Promiscuous mode',          'capture all frames on the wire'],
              ['immediateMode',  'Immediate mode',            'deliver packets as they arrive, no buffering'],
              ['monitorMode',    'Monitor mode',              '802.11 RFMON — capture raw 802.11 frames'],
              ['highPrecisionTs','High precision timestamps', 'nanosecond resolution via hardware clock'],
            ] as const).map(([key, label, hint]) => (
              <div key={key} className="flex items-center justify-between">
                <div>
                  <span className="font-mono text-xs" style={{ color: '#E6E8EC' }}>{label}</span>
                  <span className="font-mono text-[10px] ml-2" style={{ color: '#4A5058' }}>{hint}</span>
                </div>
                <Toggle checked={cfg[key]} onChange={v => set(key, v)} />
              </div>
            ))}
          </div>
        </div>

        {/* Footer */}
        <div className="flex items-center justify-end gap-2 px-5 py-3" style={{ borderTop: '1px solid rgba(44,49,56,0.4)', background: '#191C21' }}>
          <button
            onClick={onCancel}
            className="font-mono text-xs px-4 h-7 rounded transition-colors"
            style={{ background: 'transparent', border: '1px solid rgba(44,49,56,0.5)', color: '#8A9099' }}
            onMouseEnter={e => (e.currentTarget.style.color = '#E6E8EC')}
            onMouseLeave={e => (e.currentTarget.style.color = '#8A9099')}
          >
            Cancel
          </button>
          <button
            onClick={() => onStart(cfg)}
            className="flex items-center gap-2 font-mono text-xs px-4 h-7 rounded transition-colors"
            style={{ background: '#3D997022', border: '1px solid #3D997055', color: '#3D9970' }}
            onMouseEnter={e => { e.currentTarget.style.background = '#3D997033'; }}
            onMouseLeave={e => { e.currentTarget.style.background = '#3D997022'; }}
          >
            <svg width="8" height="9" viewBox="0 0 8 9" fill="currentColor"><polygon points="0,0 8,4.5 0,9"/></svg>
            Start Capture
          </button>
        </div>
      </div>
    </div>
  );
}

