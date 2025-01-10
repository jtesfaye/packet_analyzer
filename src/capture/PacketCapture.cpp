
#include "PacketCapture.h"

namespace Capture {

  PacketCapture::PacketCapture
  (
    const char* device_name, 
    int cnt, 
    const Options&& opt
  )
  : _device(device_name),
    _packets_to_capture(cnt),
    _options(opt),
    _handle(nullptr),
    _device_list(nullptr)
    {}

}