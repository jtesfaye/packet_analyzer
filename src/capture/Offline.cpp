
#include <capture/Offline.h>
#include <sys/_types/_u_int8_t.h>
#include <iostream>

Offline::Offline
(
  pcap_t* handle,
  const int dlt,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ThreadPool>& pool
)
: PacketCapture(handle, dlt ,file, pool)
{
}

void Offline::capture_func() {

  InitialParser parser(get_datalink(), 0xff);
  std::shared_ptr<IContainerType<packet_ref>> buffer = get_buffer();
  std::shared_ptr<PacketObserver> observer = get_observer();

  size_t amt = file->get_packet_count();
  std::cout << amt << std::endl;

  for (int i = 0; i < amt; i++) {

    auto data = file->read(i);
    auto pkt_ref = parser.start_extract(data, i);
    buffer->add(i, std::move(pkt_ref));
    observer->notify_if_next(i);

  }
}

void Offline::stop_func() {

}






