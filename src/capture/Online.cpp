
#include <capture/Online.h>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace capture {

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

  void
  Online::initialize_handle(int capture_size) {

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
}