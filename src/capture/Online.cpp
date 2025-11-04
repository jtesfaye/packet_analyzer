
#include <capture/Online.h>
#include <print>
#include <algorithm>

Online::Online
(
  pcap_t* handle,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile> &file,
  const std::shared_ptr<ThreadPool> &pool,
  raw_pkt_queue& queue
)
: PacketCapture(handle, file, pool, queue)
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

  pkt.index = obj->file->write(header, packet);

  pcaprec_hdr_t rec_header{};
  rec_header.ts_sec  = header->ts.tv_sec;
  rec_header.ts_usec = header->ts.tv_usec;
  rec_header.incl_len = header->caplen;
  rec_header.orig_len = header->len;

  std::memcpy(pkt.packet, &rec_header, sizeof(rec_header));

  constexpr size_t header_size = sizeof(rec_header);
  constexpr size_t max_payload = sizeof(pkt.packet) - header_size;
  const size_t payload_len = std::min(static_cast<size_t>(header->caplen), max_payload);

  std::memcpy(pkt.packet + header_size, packet, payload_len);

  obj->queue.push(pkt);
  obj->tpool->notify_all();

}

void Online::stop_func() {

  pcap_breakloop(handle());

}






