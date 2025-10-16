
#include <capture/Online.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <print>
#include <filesystem>

Online::Online
(
  pcap_t* handle,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile> &file
)
: PacketCapture(handle)
, m_flags(layer_flags)
, m_pool(std::make_shared<ThreadPool>(5))
, m_file(file) {

  m_packets_to_capture = packet_count;
  set_data_link(pcap_datalink(handle));
}

Online::~Online() = default;

void Online::capture_func() {

  auto parser = std::make_shared<PacketParse>(m_data_link, m_flags);

  capture_objects ref {
    parser,
    get_buffer(),
    m_pool,
    get_observer(),
    m_file
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

  std::println("Here");

  std::memcpy(packet_copy.data(), header, sizeof(pcaprec_hdr_t));
  std::memcpy(packet_copy.data() + sizeof(pcaprec_hdr_t), packet, header->caplen);

  obj->tpool->submit([obj,index, packet_copy]{

    auto parser = obj->parser;
    packet_ref pkt_ref = parser->start_extract(packet_copy, index);

    obj->buffer->add(index, std::move(pkt_ref));

    obj->pkt_observer->notify_if_next(index);

  });


}

void Online::stop_func() {

  pcap_breakloop(handle());

}






