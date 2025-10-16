
#ifndef ONLINE_H
#define ONLINE_H

#include <capture/PacketCapture.h>
#include <cstring>
#include <sys/_types/_u_int8_t.h>

using namespace packet::frame;

class Online final : public PacketCapture {
public:

  Online() = delete;
  Online(const Online&) = delete;
  Online& operator= (const Online&) = delete;

  Online(
    pcap_t* handle,
    int packet_count,
    size_t layer_flags,
    const std::shared_ptr<PcapFile> &file
    );

  ~Online() override;

private:

  static void pcap_loop_callback(u_char* data, const pcap_pkthdr* header, const u_char* packet);

  void capture_func() override;

  void stop_func() override;

  struct capture_objects {
    std::shared_ptr<PacketParse> parser;
    std::shared_ptr<PacketRefBuffer> buffer;
    std::shared_ptr<ThreadPool> tpool;
    std::shared_ptr<PacketObserver> pkt_observer;
    std::shared_ptr<PcapFile> file;

  };

  int m_packets_to_capture;
  u_int8_t m_flags;
  std::shared_ptr<ThreadPool> m_pool;
  std::shared_ptr<PcapFile> m_file;

};





#endif