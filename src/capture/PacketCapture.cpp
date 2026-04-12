
#include <capture/PacketCapture.h>
#include <capture/OnlineCapture.h>
#include <capture/OfflineCapture.h>
#include <iostream>
#include <packet/PcapFile.h>

PacketCapture::PacketCapture(CaptureInit init)
: _handle(init.handle)
, file(init.file)
, queue(init.queue)
{
}

std::unique_ptr<PacketCapture>
PacketCapture::createOnlineCapture(int packet_count, size_t layer_flags, CaptureInit init) {
  auto cap = std::make_unique<OnlineCapture>(packet_count, layer_flags, init);
  return cap;
}

void PacketCapture::set_callback(onRawPkt &&func) {
  on_pkt_fn = std::move(func);
}


std::unique_ptr<PacketCapture> PacketCapture::createOfflineCapture(CaptureInit init) {
  auto cap = std::make_unique<OfflineCapture>(init);
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

raw_packet_queue& PacketCapture::get_queue() const {
  return queue;
}


PacketCapture::~PacketCapture() = default;


