
#include <capture/Offline.h>
#include <sys/_types/_u_int8_t.h>
#include <iostream>

Offline::Offline
(
  pcap_t* handle,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ParsingEngine>& pool,
  raw_pkt_queue& queue
)
: PacketCapture(handle,file, pool, queue)
{
}

void Offline::capture_func() {

  size_t num_of_packets = file->get_packet_count();

  for (int i = 0; i < num_of_packets; i++) {
    RawPacket data{};
    data.index = i;
    auto raw_data = file->read(i);
    std::memcpy(data.packet, raw_data.data(), raw_data.size());
    queue.push(data);
  }

  pool->notify_all();

}

void Offline::stop_func() {

}






