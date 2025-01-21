
#ifndef ONLINE_H
#define ONLINE_H

#include "PacketCapture.h"
namespace capture {

  class Online : public PacketCapture {
  public:
    Online() = delete;
    Online(const char* device_name, int cnt, const u_int8_t set);

    Online(Online&) = delete;
    Online operator= (Online&) = delete;

    ~Online() override;

  private:

    void start_capture() override;

    void stop_capture() override;

    static void process_packet
    (
      u_char* data,
      const struct pcap_pkthdr* header, 
      const u_char* packet
    );

  };

}



#endif