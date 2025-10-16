
#include <capture/Offline.h>
#include <sys/_types/_u_int8_t.h>
#include <iostream>

Offline::Offline
(
  pcap_t* handle,
  const std::shared_ptr<PcapFile>& file
)
: PacketCapture(handle)
, m_file(file) {

  set_data_link(pcap_datalink(handle));
}

void Offline::capture_func() {

  PacketParse parser(get_datalink(), 0xff);
  std::shared_ptr<PacketRefBuffer> buffer = get_buffer();
  std::shared_ptr<PacketObserver> observer = get_observer();

  size_t amt = m_file->get_packet_count();
  std::cout << amt << std::endl;

  for (int i = 0; i < amt; i++) {

    auto data = m_file->read(i);
    auto pkt_ref = parser.start_extract(data, i);
    buffer->add(i, std::move(pkt_ref));
    observer->notify_if_next(i);

  }
}

void Offline::stop_func() {

}






