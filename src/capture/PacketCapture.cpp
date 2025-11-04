
#include <capture/PacketCapture.h>
#include <capture/Online.h>
#include <capture/Offline.h>
#include <iostream>
#include <packet/PcapFile.h>


PacketCapture::PacketCapture(
  pcap_t *h,
  const std::shared_ptr<PcapFile> &file,
  const std::shared_ptr<ThreadPool> &pool,
  raw_pkt_queue& queue
  )
: _handle(h)
, file(file)
, pool(pool)
, queue(queue)
{
}

std::unique_ptr<PacketCapture>
PacketCapture::createOnlineCapture(
  pcap_t* handle,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ThreadPool> &pool,
  raw_pkt_queue& queue) {

  auto cap = std::make_unique<Online>(handle, packet_count, layer_flags, file, pool, queue);

  return cap;

}

std::unique_ptr<PacketCapture>
PacketCapture::createOfflineCapture(
  pcap_t* handle,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ThreadPool> &pool,
  raw_pkt_queue& queue) {

  auto cap = std::make_unique<Offline>(handle, file, pool, queue);

  return cap;

}

void PacketCapture::start_capture() {
  capture_func();
}

void PacketCapture::stop_capture() {
  stop_func();
}


std::vector<std::string> PacketCapture::get_devices() {

  pcap_if_t* list = nullptr;

  std::vector<std::string> names;

  char errbuf[256];

  if (pcap_findalldevs(&list, errbuf) == 0) {

    //fix
  } else {
    std::cerr << "Error: " << errbuf << std::endl;
    return {};
  }

  for (pcap_if_t* it = list; it != nullptr; it = it->next) {

    names.emplace_back(it->name);

  }

  return names;
}

pcap_t* PacketCapture::handle() const {

  return _handle;
}

PacketCapture::~PacketCapture() = default;


