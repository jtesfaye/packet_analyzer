//
// Created by jeremiah tesfaye on 11/20/25.
//

#ifndef PROTOCOLKEYS_H
#define PROTOCOLKEYS_H

namespace protocol {
    enum class ProtocolKeys : int {
        ETH = 220,
        IPv4 = 2048,
        IPv6 = 34525,
        TCP = 6,
        UDP = 17,
        ICMP = 1,
    };
};

#endif //PROTOCOLKEYS_H
