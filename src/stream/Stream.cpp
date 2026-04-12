//
// Created by jeremiah tesfaye on 11/21/25.
//

#include <cassert>
#include <stream/Stream.h>
#include <chrono>
#include <iostream>
#include <format>
#include <stream/TCPStream.h>

std::shared_ptr<Stream> Stream::createConnection(const ProtocolKeys type) {
    switch (type) {
        case ProtocolKeys::TCP:
            return std::make_unique<TCPStream>(type);
        default:
            return nullptr;
    }
}

bool Stream::is_ephemeral(const Address &port) {
    u_int16_t portnum;
    std::memcpy(&portnum, port.bytes.data(), sizeof(portnum));
    return portnum >= EPHEMERAL_PORT_START;
}





