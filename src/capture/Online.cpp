
#include "Online.h"


namespace capture {

  Online::Online
  (
    std::string device_name,
    int count, 
    u_int8_t set
  ) 
  : PacketCapture(device_name, count, set), m_data_link(-1) {

    if (_handle == nullptr) {
      std::cerr << "Error in creating handle: " << errbuf << std::endl;
      return; //probably not the best way to handle an error but ill change it later
    }

    //decide how much of the packet will be captures
    if (_settings & FULL_CAP) {

      pcap_set_snaplen(handle(), FULL);
      
    } else if (_settings & BASIC_CAP) {

      pcap_set_snaplen(handle(), BASIC);

    } else {

      pcap_set_snaplen(handle(), DEFAULT);

    }

    //promisous mode
    if (_settings & PROMISC) {

      pcap_set_promisc(handle(), 1);

    }

    //rf mode
    if (_settings & MONITOR) {

      int rf = pcap_can_set_rfmon(handle());
      if (rf) {
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
      std::cerr << pcap_geterr(handle()) << std::endl;
      return;
    }

  }

  Online::~Online() {};


  void 
  Online::process_packet
  (
    u_char* args,
    const struct pcap_pkthdr* header,
    const u_char* packet
  ) {

    const u_int8_t* pck = packet;
    user_data* parsing = reinterpret_cast<user_data*> (args);

    link_layer l2 =
    parsing->link(packet);

    Packet(std::move(l2));

  }
  
  void 
  Online::start_capture() {

    if (m_data_link == -1) {
      m_data_link = pcap_datalink(handle());
      std::cout << "Set datalink to: " << pcap_datalink_val_to_name(m_data_link) << "\n";
    }

    try {

      Parse::LinkParse link_parse(m_data_link);

      user_data functions {link_parse};

      u_char* user_data = reinterpret_cast<u_char*> (&functions);

      pcap_loop(handle(), _packets_to_capture, process_packet, user_data);

    } catch (std::runtime_error &e) {

      std::cout << e.what() << std::endl;
      return;
    }

  }

  void Online::stop_capture() {

  }

  void Online::get_devices() {

    pcap_if_t* list = nullptr;

    if (pcap_findalldevs(&list, errbuf) == 0) {
      _device_list.reset(list);
    } else {
      std::cerr << "Error: " << errbuf << std::endl;
      return;
    }

    for (pcap_if_t* it = list; list != nullptr; list = list->next) {

      std::cout << "Name: " << list->name << " ";
      
      if(list->flags & PCAP_IF_LOOPBACK) {
        std::cout << " Loopback";
      }

      if (list->flags & PCAP_IF_WIRELESS) {
        std::cout << " Wireless ";
      }

      if (list->flags & PCAP_IF_UP) {
        std::cout << " up ";
      }

      std::cout << "\n";
    }

  }

  void Online::get_link_types() {

    int* dlt_buf;
    int list_size = pcap_list_datalinks(handle(), &dlt_buf);

    for (int i = 0; i < list_size; i++) {

      std::cout << pcap_datalink_val_to_name(dlt_buf[i]);
      std::cout << ": " << pcap_datalink_val_to_description(dlt_buf[i]) << "\n";

    }

    pcap_free_datalinks(dlt_buf);
      
  }
}