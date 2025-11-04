
#ifndef ONLINE_H
#define ONLINE_H

#include <capture/PacketCapture.h>
#include <cstring>
#include <sys/_types/_u_int8_t.h>

class Online final : public PacketCapture {
public:

  Online() = delete;
  Online(const Online&) = delete;
  Online& operator= (const Online&) = delete;

  Online(
    pcap_t* handle,
    int packet_count,
    size_t layer_flags,
    const std::shared_ptr<PcapFile> &file,
    const std::shared_ptr<ThreadPool> &pool,
    raw_pkt_queue& queue
    );

  ~Online() override;

private:

  static void pcap_loop_callback(u_char* data, const pcap_pkthdr* header, const u_char* packet);

  void capture_func() override;

  void stop_func() override;

  struct capture_objects {
    std::shared_ptr<ThreadPool> tpool;
    std::shared_ptr<PcapFile> file;
    raw_pkt_queue& queue;
  };

  int m_packets_to_capture;
  u_int8_t m_flags;

};





#endif