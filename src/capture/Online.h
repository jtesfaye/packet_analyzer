
#ifndef ONLINE_H
#define ONLINE_H

#include "PacketCapture.h"
namespace Capture {

  class OnlinePacketCapture : public PacketCapture {
  public:
    OnlinePacketCapture() = delete;
    OnlinePacketCapture(const char* device_name, int cnt, const Options& opt);

    OnlinePacketCapture(OnlinePacketCapture&) = delete;
    OnlinePacketCapture operator= (OnlinePacketCapture&) = delete;

    ~OnlinePacketCapture() override;

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