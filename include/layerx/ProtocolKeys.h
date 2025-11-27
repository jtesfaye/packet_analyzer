//
// Created by jeremiah tesfaye on 11/20/25.
//

#ifndef PROTOCOLKEYS_H
#define PROTOCOLKEYS_H
#include <unordered_map>
#include <pcap/pcap.h>
namespace protocol {
    enum class ProtocolKeys : int {
        ETH = 220,
        IPv4 = 2048,
        IPv6 = 34525,
        TCP = 6,
        UDP = 17,
        ICMP = 1,
    };

    static std::unordered_map<int, int> pcap_dlt_to_ieee {std::make_pair(DLT_EN10MB, 220)};

};

#endif //PROTOCOLKEYS_H
