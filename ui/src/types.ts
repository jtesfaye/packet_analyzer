export type CaptureState = "idle" | "running" | "paused"

export interface ChatMessage {
  role: "user" | "assistant"
  content: string
}

export interface SessionConfig {
  interface: string;
  bpfFilter: string;
  snaplen: string;
  promiscuous: boolean;
  immediateMode: boolean;
  monitorMode: boolean;
  highPrecisionTs: boolean;
  maxPackets: string;
  label: string;
}

export const DEFAULT_CONFIG: SessionConfig = {
  interface: 'eth0',
  bpfFilter: '',
  snaplen: '65535',
  promiscuous: true,
  immediateMode: false,
  monitorMode: false,
  highPrecisionTs: false,
  maxPackets: '',
  label: '',
};

