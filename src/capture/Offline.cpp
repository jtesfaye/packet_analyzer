
#include <capture/Offline.h>
#include <sys/_types/_u_int8_t.h>
#include <iostream>

Offline::Offline
(
  std::string&& path_name
)
: m_file_path(std::move(path_name))
{

  initialize_handle();

  set_data_link(pcap_datalink(handle()));

  if (!handle()) {

    std::cerr << "error with handle" << errbuf << "\n";

  }
}

void Offline::initialize_handle() {

  if (pcap_t* pcap_handle = pcap_open_offline(m_file_path.c_str(), errbuf)) {

    _handle = std::unique_ptr<pcap_t, decltype(&close_handle)> (
      pcap_handle,
      pcap_close
    );

  } else {

    throw std::runtime_error(errbuf);

  }
}

void Offline::capture_func() {

  PcapFile file{m_file_path.c_str()};
  PacketParse parser(get_datalink(), static_cast<u_int8_t>(0xff));
  std::shared_ptr<PacketRefBuffer> buffer = get_buffer();
  std::shared_ptr<PacketObserver> observer = get_observer();

  size_t amt = file.get_packet_count();
  std::cout << amt << std::endl;

  for (int i = 0; i < amt; i++) {

    auto data = file.read(i);
    auto pkt_ref = parser.start_extract(data, i);
    buffer->add(i, std::move(pkt_ref));
    observer->notify_if_next(i);

  }
}





