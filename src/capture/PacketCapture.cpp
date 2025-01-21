
#include "PacketCapture.h"

namespace capture {

  PacketCapture::PacketCapture
  (
    const char* device_name, 
    int cnt, 
    u_int8_t settings
  )
  : _device(device_name),
    _packets_to_capture(cnt),
    _settings(settings),
    _handle(nullptr),
    _device_list(nullptr)
    {}

  PacketCapture::~PacketCapture() {};

}