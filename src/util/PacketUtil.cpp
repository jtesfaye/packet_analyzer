//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <util/PacketUtil.h>
#include <string>
#include <format>
#include <iomanip>
#include <iostream>

using namespace packet;
packet_data::~packet_data() = default;

packet_data::packet_data() = default;

packet_data::packet_data(packet_data&&) noexcept = default;

packet_data& packet_data:: operator=(packet_data&&) noexcept = default;

std::string packet::format_mac(const u_int8_t *addr) {
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }
    return oss.str();
}

std::string packet::format_ipv4_src_dst(const u_int32_t& src_dest_add) {
    const u_int32_t addr = ntohl(src_dest_add);
    return std::format("{}.{}.{}.{}",
        addr >> 24 & 0xFF,
        addr >> 16 & 0xFF,
        addr >> 8 & 0xFF,
        addr & 0xFF
        );
}

std::string packet::format_ipv6_src_dest(const u_int8_t *addr) {
    std::array<uint16_t, 8> segments{};
    for (int i = 0; i < 16; i += 2) {
        segments[i / 2] = (addr[i] << 8) | addr[i + 1];
    }
    int bestStart = -1, bestLen = 0;
    for (int i = 0; i < 8; ) {
        if (segments[i] == 0) {
            int j = i;
            while (j < 8 && segments[j] == 0) j++;
            int len = j - i;
            if (len > bestLen) {
                bestLen = len;
                bestStart = i;
            }
            i = j;
        } else {
            i++;
        }
    }
    if (bestLen < 2) {
        bestStart = -1;
        bestLen = 0;
    }
    std::ostringstream oss;
    oss << std::hex;
    for (int i = 0; i < 8; ) {
        if (i == bestStart) {
            oss << "::";
            i += bestLen;
            if (i >= 8)
                break;
        } else {
            if (i > 0 && i != bestStart + bestLen)
                oss << ":";
            oss << std::noshowbase << std::hex << std::nouppercase << segments[i];
            i++;
        }
    }
    return oss.str();
}


bool packet::valid_length(
    std::span<std::byte> data,
    const size_t offset,
    const size_t needed) {
    return data.size() >= offset + needed;
}

std::string packet::protocol_to_string(u_int16_t protocol) {
    std::string info;
    switch (protocol) {
        case 1:
            info += "ICMP ";
        break;
        case 6:
            info += "TCP ";
        break;
        case 17:
            info += "UDP ";
        break;
        default:
    }
    return info;
}

boost::json::object packet::to_json(const packet_data& ref) {
  using namespace boost::json;
  object pktrow;

  pktrow["id"] = ref.index;
  pktrow["time_sec"] = ref.time.ts_sec;
  pktrow["time_usec"] = ref.time.ts_usec;
  pktrow["wire_length"] = ref.wire_length;
  pktrow["payload_length"] = ref.payload_length;

  std::string src;
  std::string dest;
  size_t protocol;
  std::string desc;
  if (ref.layer4) {

    src = ref.layer4->address_to_string(ref.layer3->src());
    dest = ref.layer4->address_to_string(ref.layer3->dest());
    protocol = static_cast<size_t>(ref.layer4->type());
    desc = ref.layer4->make_info();
  } else if (ref.layer3) {

    src = ref.layer3->address_to_string(ref.layer3->src());
    dest = ref.layer3->address_to_string(ref.layer3->dest());
    protocol = static_cast<size_t>(ref.layer3->type());
    desc = ref.layer3->make_info();
  } else if (ref.layer2) {
    src = ref.layer2->address_to_string(ref.layer2->src());
    dest = ref.layer2->address_to_string(ref.layer2->dest());
    protocol = static_cast<size_t>(ref.layer2->type());
    desc = ref.layer2->make_info();
  } else {
    pktrow =
    {
      {"id", ref.index},
      {"time_sec", nullptr},
      {"time_usc", nullptr},
      {"wire_length", nullptr},
      {"payload_length", nullptr},
      {"src", nullptr},
      {"dest", nullptr},
      {"protocol", nullptr},
      {"desc", "n/a"},
  };
    return pktrow;
  }
  pktrow["src"] = src;
  pktrow["dest"] = dest;
  pktrow["protocol"] = protocol;
  pktrow["desc"] = desc;
  return pktrow;
}

boost::json::object packet::to_json(const ProtocolDetails& d) {
  boost::json::object details;
  details["name"] = d.name;
  boost::json::array f;
  for (const auto& s : d.fields) {
    f.emplace_back(s);
  }
  details["fields"].emplace_array() = f;
  return details;
}

boost::json::object packet::to_json(const StreamStatistics& stats) {
  auto data = stats.get_data();
  boost::json::array vals;
  vals.reserve(data.size());
  for (const auto& i : data) {
    std::visit([&]<typename T0>(const T0& val) {
      using T = std::decay_t<T0>;
      if constexpr (std::is_same_v<T, std::monostate>) {
        vals.push_back(nullptr);
      } else if constexpr (std::is_same_v<T, EpochTime>) {
        vals.push_back(val.count());
      } else if constexpr (std::is_arithmetic_v<T>) {
        vals.push_back(val);
      } else {
        static_assert(!sizeof(T), "unhandled variant");
      }
    }, i);
  }
  boost::json::object obj;
  obj["values"] = vals;
  return obj;
}

