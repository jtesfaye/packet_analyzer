
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

    ~OnlinePacketCapture();

    void start_capture();

  };

}



#endif