//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <layerx/layer4/ICMP.h>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>
#include <utility>

ICMP::ICMP(const size_t len, const u_int8_t type, const u_int8_t code) :
TransportPDU(len, "", ""), //icmp does not have src or dest addresses so we pass blank
type(type),
code(code)
{
}

std::string ICMP::make_info() const {

    return icmp_functions::type_code_to_string(type, code);
}

std::string ICMP::name() const {
    return "ICMP";
}

Layer4Registry &icmp_functions::get_icmp_registry() {
    static Layer4Registry icmp_reg(layer::iana::ICMP, icmp_parse);
    return icmp_reg;
}


std::unique_ptr<TransportPDU> icmp_functions::icmp_parse(
    const std::vector<std::byte> &raw_data,
    packet::parse_context &context) {

    using namespace layer::transport;

    if (!PacketRead::valid_length(raw_data, context.offset, sizeof(icmp_header))) {

        return nullptr;
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto icmp_hdr = reinterpret_cast<const icmp_header*> (start);

    return std::make_unique<ICMP>(
        64,
        icmp_hdr->type,
        icmp_hdr->code);

}

std::string icmp_functions::type_code_to_string(u_int8_t type, u_int8_t code) {

    using namespace layer::transport;

    switch (type) {

        case icmp_types::ECHO_REPLY:
            return "Echo (ping) reply";

        case icmp_types::ECHO_REQUEST:
            return "Echo (ping) request";

        case icmp_types::DEST_UNREACHABLE:

            switch (code) {
                case 0:
                    return "Destination network unreachable";

                case 1:
                    return "Destination host unreachable";

                case 2:
                    return "Destination protocol unreachable";

                case 3:
                    return "Destination port unreachable";

                default:
                    return "Destination unreachable (code " + std::to_string(code) + ")";

            }

        case icmp_types::SOURCE_QUENCH:
            return "Source quench (congestion control)";

        case icmp_types::REDIRECT:

            switch (code) {

                case 0:
                    return "Redirect datagrams for the network";

                case 1:
                    return "Redirect datagrams for the host";

                default:
                    return "Redirect message (code " + std::to_string(code) + ")";
            }

        case icmp_types::TIME_EXCEEDED:

            switch (code) {

                case 0:
                    return "TTL expired in transit";

                case 1:
                    return "Fragment reassembly time exceeded";

                default:
                    return "Time exceeded (code " + std::to_string(code) + ")";
            }

        case icmp_types::PARAMETER_PROBLEM:
            return "Parameter problem";

        case icmp_types::TIMESTAMP_REQUEST:
            return "Timestamp request";

        case icmp_types::TIMESTAMP_REPLY:
            return "Timestamp reply";

        case icmp_types::INFORMATION_REQUEST:
            return "Information request";

        case icmp_types::INFORMATION_REPLY:
            return "Information reply";

        case icmp_types::ADDRESS_MASK_REQUEST:
            return "Address mask request";

        case icmp_types::ADDRESS_MASK_REPLY:
            return "Address mask reply";

        default:
            return "Unknown ICMP type (" + std::to_string(type) + ")";
    }
}


