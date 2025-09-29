
#include <capture/PacketCapture.h>
#include <capture/Online.h>
#include <capture/Offline.h>
#include <iostream>
#include <span>
#include <packet/PcapFile.h>


PacketCapture::PacketCapture()
: _handle(nullptr, close_handle)
, _device_list(nullptr, free_devices)
, _packets_to_capture(0)
, m_data_link(-1) {
}

std::unique_ptr<PacketCapture>
PacketCapture::createOnlineCapture(
  std::string& device_name,
  int count,
  int capture_size,
  u_int8_t settings,
  u_int8_t flags) {

  auto cap = std::make_unique<Online>(
    std::move(device_name)
    , count
    , capture_size
    , settings
    , flags);

  cap->set_buffer(std::make_shared<PacketRefBuffer>(count));

  auto obs = cap->get_buffer();
  cap->set_observer(std::make_shared<PacketObserver>(*obs));

  return cap;

}

std::unique_ptr<PacketCapture>
PacketCapture::createOfflineCapture(
  std::string &path_name,
  int count) {

  auto cap = std::make_unique<Offline>(
    std::move(path_name)
    , count);

  cap->set_buffer(std::make_shared<PacketRefBuffer>(count));

  return cap;

}

void PacketCapture::capture_func() {

}

void PacketCapture::start_capture() {
  this->capture_func();
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

void PacketCapture::free_devices (pcap_if_t* dev_list) {
  if (!dev_list) {
    return;
  }

  pcap_freealldevs(dev_list);

}

void PacketCapture::close_handle(pcap_t* handle) {
  if (!handle)
    return;

  pcap_close(handle);

}


pcap_t* PacketCapture::handle() const {

  return _handle.get();
}

pcap_if_t* PacketCapture::devices() const {

  return _device_list.get();
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


