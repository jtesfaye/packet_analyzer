//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef UDP_H
#define UDP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>
#include <layerx/layer4/Layer4Registry.h>
#include <vector>

using namespace layer::transport;
using namespace packet;

struct UDP : TransportPDU {

    UDP(size_t len, std::string src_port, std::string dest_port);
    ~UDP() override;

    std::string make_info() const override;
    std::string name() const override;

};

class udp_functions {
public:

    static std::unique_ptr<TransportPDU> udp_parse(
        const std::vector<std::byte>& raw_data,
        parse_context& context);

    static ProtocolDetails udp_detailed_parse(
        const std::vector<std::byte>& raw_data,
        parse_context& context);

    static Layer4Registry& get_udp_registry();

private:

    static std::string full_protocol_name() {
        return "User Datagram Protocol";
    }

};

#endif //UDP_H
