//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef _802_11_H
#define _802_11_H

#include "tests/include/packet/LayerWrappers.h"
#include <pcap/pcap.h>

using namespace packet;
using namespace packet::frame;

class _802_11_functions {
public:

    static link_layer_ref _802_11_parse(const u_int8_t* raw_data, parse_context&);

};

#endif //_802_11_H
