
#include <capture/PacketCapture.h>
#include <capture/Online.h>
#include <capture/Offline.h>
#include <iostream>
#include <packet/PcapFile.h>


PacketCapture::PacketCapture(pcap_t *h, int dlt, const std::shared_ptr<PcapFile> &file, const std::shared_ptr<ThreadPool> &pool)
: _handle(h)
, m_data_link(dlt)
, file(file)
, pool(pool)
{
}

std::unique_ptr<PacketCapture>
PacketCapture::createOnlineCapture(
  pcap_t* handle,
  int dlt,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ThreadPool> &pool) {

  auto cap = std::make_unique<Online>(handle, dlt, packet_count, layer_flags, file, pool);

  return cap;

}

std::unique_ptr<PacketCapture>
PacketCapture::createOfflineCapture(
  pcap_t* handle,
  int dlt,
  const std::shared_ptr<PcapFile>& file,
  const std::shared_ptr<ThreadPool> &pool) {

  auto cap = std::make_unique<Offline>(handle, dlt, file, pool);

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

int PacketCapture::get_datalink() const {

  return m_data_link;

}

PacketCapture::~PacketCapture() = default;


