
#ifndef ONLINE_H
#define ONLINE_H

#include <capture/PacketCapture.h>
#include <cstring>
#include <sys/_types/_u_int8_t.h>

class OnlineCapture final : public PacketCapture {
public:
  OnlineCapture() = delete;
  OnlineCapture(const OnlineCapture&) = delete;
  OnlineCapture& operator= (const OnlineCapture&) = delete;
  OnlineCapture(int packet_count, size_t layer_flags, CaptureInit init);
  ~OnlineCapture() override;

private:
  static void pcap_loop_callback(u_char* data, const pcap_pkthdr* header, const u_char* packet);
  void capture_func() override;
  void stop_func() override;

  struct capture_objects {
    std::shared_ptr<PcapFile> file;
    raw_packet_queue& queue;
    onRawPkt func;
  };

  int m_packets_to_capture;
  u_int8_t m_flags;
};





#endif