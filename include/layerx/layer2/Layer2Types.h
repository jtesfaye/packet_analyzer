//
// Created by jeremiah tesfaye on 9/1/25.
//

#ifndef LAYER2TYPES_H
#define LAYER2TYPES_H

#include <utility>
#include <layerx/ProtocolDataUnit.h>

struct LinkPDU : ProtocolDataUnit {

    LinkPDU(size_t len, std::string src, std::string dest)
    : ProtocolDataUnit(len, std::move(src), std::move(dest))
    {}

    ~LinkPDU() override = default;

};

namespace packet::frame {

    struct ethernet_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t ether_type;

    };

    struct ether_802_1_Q_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t ether_type;

    };

    struct ether_802_1_ad_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t tpid_2;
        u_int16_t tci_2;
        u_int16_t ether_type;

    };

    struct _802_2_hdr {

        u_int8_t DSAP_addr;
        u_int8_t SSAP_addr;
        u_int8_t control;

    };

    struct snap_extension_hdr {

        u_int8_t oui[3]; //organizationally unique identifier, not 'yes' in french
        u_int16_t protocol_id;

    };
}

#endif //LAYER2TYPES_H
