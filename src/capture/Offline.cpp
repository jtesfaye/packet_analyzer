
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

}

void Offline::stop_func() {

}






