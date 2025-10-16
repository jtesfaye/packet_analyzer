
#include <capture/PacketCapture.h>
#include <capture/Online.h>
#include <capture/Offline.h>
#include <iostream>
#include <packet/PcapFile.h>


PacketCapture::PacketCapture(pcap_t* handle)
: _handle(handle)
, m_data_link() {}

std::unique_ptr<PacketCapture>
PacketCapture::createOnlineCapture(
  pcap_t* handle,
  int packet_count,
  size_t layer_flags,
  const std::shared_ptr<PcapFile> &file) {

  auto cap = std::make_unique<Online>(handle, packet_count, layer_flags, file);

  cap->set_buffer(std::make_shared<PacketRefBuffer>(packet_count));

  auto obs = cap->get_buffer();
  cap->set_observer(std::make_shared<PacketObserver>(*obs));

  return cap;

}

std::unique_ptr<PacketCapture>
PacketCapture::createOfflineCapture(
  pcap_t* handle,
  const std::shared_ptr<PcapFile>& file) {

  auto cap = std::make_unique<Offline>(handle, file);
  cap->set_buffer(std::make_shared<PacketRefBuffer>(20));

  auto obs = cap->get_buffer();
  cap->set_observer(std::make_shared<PacketObserver>(*obs));

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

void PacketCapture::set_data_link(int dlt) {

  m_data_link = dlt;
}

int PacketCapture::get_datalink() const {

  return m_data_link;

}

void PacketCapture::set_buffer(const std::shared_ptr<PacketRefBuffer> &buf) {

  _buffer = buf;
}

std::shared_ptr<PacketRefBuffer> PacketCapture::get_buffer() {

  return _buffer;
}

void PacketCapture::set_observer(const std::shared_ptr<PacketObserver> & ob) {

  observer = ob;
}

std::shared_ptr<PacketObserver> PacketCapture::get_observer() {

  return observer;
}


PacketCapture::~PacketCapture() = default;


