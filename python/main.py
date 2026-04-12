import sys
sys.path.append("/Users/jt/projects/workspace/personal_projects/sniffer/build")

import sniffer_py
print(dir(sniffer_py))

config = sniffer_py.CaptureConfig()
config.capture_size = 250
config.flags = 1
config.filter = ""
config.mode = sniffer_py.CaptureMode.Online
config.settings = 0xff
config.packet_count = 25
config.source = "en0"

pkt_cap = sniffer_py.CaptureSession()
pkt_cap.send_command(sniffer_py.SessionCommand.start(config))

packet_count = 0

while packet_count < 25:
    msg = pkt_cap.poll()

    if msg.type == sniffer_py.EventMessageType.Packet:
        print("packet")
        packet_count += 1
        print(msg.message)


pkt_cap.send_command(sniffer_py.SessionCommand.stop())
pkt_cap.send_command(sniffer_py.SessionCommand.end())


