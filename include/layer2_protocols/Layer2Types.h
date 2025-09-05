//
// Created by jeremiah tesfaye on 9/1/25.
//

#ifndef LAYER2TYPES_H
#define LAYER2TYPES_H
#include <cstdint>

namespace packet::frame {

    constexpr uint16_t VLAN_PCP = 0xE000;
    constexpr uint16_t VLAN_DEI = 0x1000;
    constexpr uint16_t VLAN_VID = 0x0fff;

    namespace _802_11_mask {

        constexpr u_int16_t PROTOCOL_VERS = 0xC000;
        constexpr u_int16_t TYPE = 0x3000;
        constexpr u_int16_t SUBTYPE = 0x0f00;
        constexpr u_int16_t ToDS = 0x0080;
        constexpr u_int16_t FromDS = 0040;
        constexpr u_int16_t MF = 0x0020;
        constexpr u_int16_t RETRY = 0x0010;
        constexpr u_int16_t PWRMGMT = 0x0008;
        constexpr u_int16_t MOREDATA = 0x0004;
        constexpr u_int16_t PROTECTED = 0x0002;
        constexpr u_int16_t ORDER = 0x0001;
        constexpr u_int16_t DATAFRAME = 0x2000;
        constexpr u_int16_t MANAGMENT = 0x0000;
        constexpr u_int16_t CONTROL = 0x1000;

    }

    struct EN10MB {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t ether_type;

    };

    struct EN10MB_802_1_Q {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t ether_type;

    };

    struct EN10MB_802_1_ad {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t tpid_2;
        u_int16_t tci_2;
        u_int16_t ether_type;

    };

    struct _802_11 {

        u_int16_t frame_control;
        u_int16_t duration_id;
        u_int8_t addr1[6];
        u_int8_t addr2[6];
        u_int8_t addr3[6];
        u_int16_t sqnc_ctrl;

    };

    struct _802_2 {

        u_int8_t DSAP_addr;
        u_int8_t SSAP_addr;
        u_int8_t control;

    };

    struct snap_extension {

        u_int8_t oui[3]; //organizationally unique identifier, not 'yes' in french
        u_int16_t protocol_id;

    };

}

#endif //LAYER2TYPES_H
