
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
  const std::shared_ptr<ThreadPool> &pool,
  raw_pkt_queue& queue
)
: PacketCapture(handle, dlt, file, pool, queue)
, m_flags(layer_flags)
{
  m_packets_to_capture = packet_count;
}

Online::~Online() = default;

void Online::capture_func() {

  capture_objects ref {
    pool,
    file,
    queue
  };

  pcap_loop(handle(), m_packets_to_capture, pcap_loop_callback, reinterpret_cast<u_char*>(&ref));

}

void Online::pcap_loop_callback(
  u_char *data,
  const pcap_pkthdr *header,
  const u_char *packet) {

  const auto obj = reinterpret_cast<capture_objects*>(data);

  RawPacket pkt{};

  size_t index = obj->file->write(header, packet);
  pkt.index = index;

  std::memcpy(pkt.packet, header, sizeof(pcaprec_hdr_t));

  size_t len = header->caplen + sizeof(pcaprec_hdr_t) > 1500 ? 1500 - 16 : header->caplen;
  std::memcpy(pkt.packet + sizeof(pcaprec_hdr_t), packet, len);

  obj->queue.push(pkt);
  obj->tpool->notify_all();

}

void Online::stop_func() {

  pcap_breakloop(handle());

}






