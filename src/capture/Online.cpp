
#include <capture/Online.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <print>

Online::Online
(
  std::string&& device_name,
  const int count,
  const int capture_size,
  const u_int8_t settings,
  const u_int8_t flags
)
: _device(std::move(device_name))
, _settings(settings)
, m_flags(flags) {

  _packets_to_capture = count;

  initialize_handle(capture_size);

  set_data_link(pcap_datalink(handle()));

}

Online::~Online() = default;

void Online::capture_func() {

  const char* file_path = "/Users/jt/Desktop/pcap_files/pcap_file1.pcap";
  auto parser = PacketParse(m_data_link, m_flags);

  capture_objects ref = capture_objects::setup_capture(
    file_path,
    parser,
    _buffer,
    5,
    handle(),
    *get_observer());

  pcap_loop(handle(), _packets_to_capture, pcap_loop_callback, reinterpret_cast<u_char*>(&ref));

}

void Online::pcap_loop_callback(
  u_char *data,
  const pcap_pkthdr *header,
  const u_char *packet) {

  const auto obj = reinterpret_cast<capture_objects*>(data);

  size_t index = obj->file->write(header, packet);
  size_t pkt_size = sizeof(pcaprec_hdr_t) + header->caplen;
  std::vector<std::byte> packet_copy;
  packet_copy.resize(pkt_size);

  std::memcpy(packet_copy.data(), header, sizeof(pcaprec_hdr_t));
  std::memcpy(packet_copy.data() + sizeof(pcaprec_hdr_t), packet, header->caplen);

  obj->tpool->submit([obj,index, packet_copy]{

    packet_ref pkt_ref = obj->parser.start_extract(packet_copy, index);

    obj->pktref_buffer->add(index, std::move(pkt_ref));

    obj->pkt_observer.notify_if_next(index);

  });
}

void
Online::initialize_handle(int capture_size) {

  using namespace capture;

  if (pcap_t* pcap_handle = pcap_create(_device.c_str(), errbuf)) {

    _handle = std::unique_ptr<pcap_t, decltype(&pcap_close)> (
      pcap_handle,
      pcap_close
    );

  } else {

    throw std::runtime_error(errbuf);

  }

  if (handle() == nullptr) {
    std::cerr << "Error in creating handle: " << errbuf << std::endl;
    return; //probably not the best way to handle an error but ill change it later
  }

  //decide how much of the packet will be captures
  if (capture_size == 0)

    pcap_set_snaplen(handle(), DEFAULT);
  else
    pcap_set_snaplen(handle(), capture_size);

  //promisous mode
  if (_settings & PROMISC) {

    pcap_set_promisc(handle(), 1);

  }

  //rf mode
  if (_settings & MONITOR) {

    if (int rf = pcap_can_set_rfmon(handle())) {
      pcap_set_rfmon(handle(), 1);
    } else {
      //send message to user than rf mode cant be used on this device
    }

  }

  //decided if packet gets immediatly delivered instead of going to buffer
  if (_settings & IMMEDIATE) {

    pcap_set_immediate_mode(handle(), 1);

  }

  //precision of timestamps
  if (_settings & PRECISION) {

    pcap_set_tstamp_type(handle(), PCAP_TSTAMP_HOST_HIPREC);

  } else {

    pcap_set_tstamp_type(handle(), PCAP_TSTAMP_HOST);

  }

  //time between checking packet buffer
  if (_settings & HIGH_TRAFF) {

    pcap_set_timeout(handle(), 10);

  } else {

    pcap_set_timeout(handle(), 500);

  }

  if (pcap_activate(handle()) != 0) {
    throw std::runtime_error(pcap_geterr(handle()));
  }

}


void Online::get_link_types() const {

  int* dlt_buf;
  const int list_size = pcap_list_datalinks(handle(), &dlt_buf);

  for (int i = 0; i < list_size; i++) {

    std::cout << pcap_datalink_val_to_name(dlt_buf[i]);
    std::cout << ": " << pcap_datalink_val_to_description(dlt_buf[i]) << "\n";

  }

  pcap_free_datalinks(dlt_buf);

}

Online::capture_objects::capture_objects(
  PacketParse& p,
  PacketRefBuffer* b,
  std::shared_ptr<ThreadPool> tp,
  PacketObserver& o,
  std::shared_ptr<PcapFile> f)
: parser(p)
, pktref_buffer(b)
, tpool(std::move(tp))
, pkt_observer(o)
, file(std::move(f))
{}

Online::capture_objects
Online::capture_objects::make(
  PacketParse& p,
  PacketRefBuffer* b,
  std::shared_ptr<ThreadPool> tp,
  PacketObserver& o,
  std::shared_ptr<PcapFile> f) {

  return {p, b, std::move(tp), o, std::move(f)};

}

Online::capture_objects
Online::capture_objects::setup_capture(
  const char* file_path,
  PacketParse &parser,
  const std::shared_ptr<PacketRefBuffer>& buffer,
  int thread_count,
  pcap_t *handle,
  PacketObserver& observer) {

  auto file =std::make_shared<PcapFile>(file_path, handle, 2048);
  auto pool = std::make_shared<ThreadPool>(thread_count);

  return make(parser, buffer.get(), pool, observer, file);

}

