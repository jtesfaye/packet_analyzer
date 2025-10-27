
#include <capture/Online.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <print>
#include <filesystem>

Online::Online
(
  pcap_t* handle,
  int dlt,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile> &file,
  const std::shared_ptr<ThreadPool> &pool
)
: PacketCapture(handle, dlt, file, pool)
, m_flags(layer_flags)
{
  m_packets_to_capture = packet_count;
}

Online::~Online() = default;

void Online::capture_func() {

  capture_objects ref {
    pool,
    file
  };

  pcap_loop(handle(), m_packets_to_capture, pcap_loop_callback, reinterpret_cast<u_char*>(&ref));

  ref.tpool->shutdown();
}

void Online::pcap_loop_callback(
  u_char *data,
  const pcap_pkthdr *header,
  const u_char *packet) {

  const auto obj = reinterpret_cast<capture_objects*>(data);

  size_t index = obj->file->write(header, packet);
  size_t pkt_size = sizeof(pcaprec_hdr_t) + header->caplen;
  std::vector<std::byte> packet_copy;
  packet_copy.resize(pkt_size);

  std::memcpy(packet_copy.data(), header, sizeof(pcaprec_hdr_t));
  std::memcpy(packet_copy.data() + sizeof(pcaprec_hdr_t), packet, header->caplen);

  obj->tpool->add(index, ParseJob::InitialParse);

}

void Online::stop_func() {

  pcap_breakloop(handle());

}






