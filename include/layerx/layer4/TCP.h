//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef TCP_H
#define TCP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>
#include <layerx/layer4/Layer4Registry.h>
#include <vector>

using namespace layer::transport;
using namespace packet;

struct TCP final : TransportPDU {

    TCP(size_t len, std::string src_port, std::string dest_port, u_int8_t flags);
    ~TCP() override;

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t flags;

};

class tcp_functions {

    friend struct TCP;

public:

    static std::unique_ptr<TransportPDU> tcp_parse(
        std::span<std::byte> raw_data,
        parse_context& context);

    static ProtocolDetails tcp_detailed_parse(
        std::span<std::byte> raw_data,
        parse_context& context);

    static void register_tcp();

private:

    static std::string tcp_flags_to_string(u_int8_t flags);

    static std::string full_protocol_name() {
        return "Transmission Control Protocol";
    }
};



#endif //TCP_H
