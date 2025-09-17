
#include <capture/PacketCapture.h>
#include <capture/Online.h>
#include <capture/Offline.h>
#include <iostream>
#include <span>
#include <packet/PcapFile.h>

namespace capture {

  PacketCapture::PacketCapture()
  : _packets_to_capture(0)
  , m_data_link(-1)
  , _handle(nullptr, close_handle)
  , _device_list(nullptr, free_devices) {
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

    cap->set_buffer(std::make_shared<PacketRefBuffer>(count, nullptr));

    return cap;

  }

  std::unique_ptr<PacketCapture>
  PacketCapture::createOfflineCapture(
    std::string &path_name,
    int count) {

    auto cap = std::make_unique<Offline>(
      std::move(path_name)
      , count);

    cap->set_buffer(std::make_shared<PacketRefBuffer>(count, nullptr));

    return cap;

  }

  void
  PacketCapture::pcap_loop_callback(
    u_char *data,
    const pcap_pkthdr *header,
    const u_char *packet) {

    const auto obj = reinterpret_cast<capture_objects*>(data);

    size_t index = obj->file.write(header, packet);

    obj->tpool.submit([obj, index] {

      const std::vector<std::byte> raw_pkt = obj->file.read(index);

      packet_ref pkt_ref = obj->parser.start_extract(raw_pkt, index); //make it so start_extract takes index

      obj->pktref_buffer->add(index, std::move(pkt_ref));

      obj->pkt_observer.notify_if_next(index);

    });

  }

  void
  PacketCapture::start_capture() const {

    using namespace parse;

    auto parser = PacketParse(m_data_link, 0);
    const char* file_path = "/Users/jt/projects/workspace/personal_projects/sniffer/tests/pcap_files.pcap";

    capture_objects ref = capture_objects::setup_capture(file_path, parser, _buffer, 5, handle());

    pcap_loop(handle(), _packets_to_capture, pcap_loop_callback, reinterpret_cast<u_char*>(&ref));

  }

  std::vector<std::string>
  PacketCapture::get_devices() {

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

  void 
  PacketCapture::free_devices
  (pcap_if_t* dev_list) {
    if (!dev_list) {
      return;
    } 

    pcap_freealldevs(dev_list);

  }

  void 
  PacketCapture::close_handle
  (pcap_t* handle) {
    if (!handle) 
      return;

    pcap_close(handle);

  }

  PacketCapture::capture_objects::capture_objects(
    parse::PacketParse& p,
    PacketRefBuffer* b,
    ThreadPool& tp,
    PacketObserver& o,
    PcapFile& f)
  : parser(p)
  , pktref_buffer(b)
  , tpool(tp)
  , pkt_observer(o)
  , file(f)
  {}

  PacketCapture::capture_objects
  PacketCapture::capture_objects::make(
    parse::PacketParse& p,
    PacketRefBuffer* b,
    ThreadPool& tp,
    PacketObserver& o,
    PcapFile& f) {

    return {p, b, tp, o, f};

  }

  PacketCapture::capture_objects
  PacketCapture::capture_objects::setup_capture(
    const char* file_path,
    parse::PacketParse &parser,
    const std::shared_ptr<PacketRefBuffer>& buffer,
    int thread_count,
    pcap_t *handle) {

    PcapFile file(file_path, handle, 2048);
    ThreadPool pool(thread_count);
    PacketObserver observer(*buffer);
    observer.wait_for_next();
    return make(parser, buffer.get(), pool, observer,file);

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

  int
  PacketCapture::get_datalink() const {
    return m_data_link;
  }

  void
  PacketCapture::set_buffer(const std::shared_ptr<PacketRefBuffer> &buf)
  {
    _buffer = buf;
  }

  std::shared_ptr<PacketRefBuffer> PacketCapture::get_buffer() {

    return _buffer;

  }


  PacketCapture::~PacketCapture() = default;

}
