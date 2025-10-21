//
// Created by jeremiah tesfaye on 10/10/25.
//

#ifndef CAPTURECONFIG_H
#define CAPTURECONFIG_H

#include <cstdint>

enum class CaptureMode {
    Online, Offline
  };

struct CaptureConfig {

    CaptureMode mode;
    std::string source;
    int packet_count = 0;
    int capture_size = 255;
    u_int8_t settings;
    u_int8_t flags;
    std::string filter;

};

#endif //CAPTURECONFIG_H
