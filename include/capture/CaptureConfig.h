//
// Created by jeremiah tesfaye on 10/10/25.
//

#ifndef CAPTURECONFIG_H
#define CAPTURECONFIG_H
#include <cstdint>
#include <iostream>
#include <boost/json.hpp>
namespace capture {
    constexpr int DEFAULT_CAPSIZE = 255;
    constexpr u_int8_t PROMISC = 0x20;
    constexpr u_int8_t IMMEDIATE = 0x10;
    constexpr u_int8_t MONITOR = 0x08;
    constexpr u_int8_t PRECISION = 0x04;
    constexpr u_int8_t HIGH_TRAFF = 0x02;
}

enum class CaptureMode {
    Online = 0,
    Offline = 1
};

struct CaptureConfig {
    CaptureMode mode;
    std::string source;
    int packet_count = 0;
    int capture_size = 255;
    size_t settings{};
    size_t flags {};
    std::string filter;

    CaptureConfig() = default;
    explicit CaptureConfig(const boost::json::object& obj) {
        using namespace boost::json;
        using namespace capture;

        mode = static_cast<CaptureMode>(obj.at("mode").as_int64());
        source = obj.at("source").as_string();
        packet_count = obj.at("packet_count").as_int64();
        capture_size = obj.at("capture_size").as_int64();
        auto s = obj.at("settings").as_object();
        if (s.at("immediate").as_bool()) {
            settings ^= IMMEDIATE;
        }
        if (s.at("promisc").as_bool()) {
            settings ^= PROMISC;
        }
        if (s.at("precision").as_bool()) {
            settings ^= PRECISION;
        }
        if (auto v = obj.try_at("filter")) {
            if (!v->is_null()) {
                filter = obj.at("filter").as_string();
            } else {
                filter = "n/a";
            }
        } else {
            filter = "n/a";
        }
    }
};


#endif //CAPTURECONFIG_H
