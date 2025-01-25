
#ifndef ONLINE_H
#define ONLINE_H

#include "PacketCapture.h"

#include <cstring>

using packet::Packet;
using namespace packet::layers;
using namespace packet::frame;

namespace capture {

  class Online : public PacketCapture {
  public:
    Online() = delete;
    Online(std::string device_name, int cnt, const u_int8_t set);

    Online(Online&) = delete;
    Online operator= (Online&) = delete;

    ~Online() override;

    void start_capture() override;

    void stop_capture() override;

    void get_devices() override;

    void get_link_types() override;
  private:

    static void process_packet
    (
      u_char* data,
      const struct pcap_pkthdr* header, 
      const u_char* packet
    );

    void set_data_link(int dlt) {m_data_link = dlt;}
    int m_data_link;
    
  };

}



#endif