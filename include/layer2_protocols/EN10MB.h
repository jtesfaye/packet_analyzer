//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef EN10MB_H
#define EN10MB_H

#include "../Packet/LayerWrappers.h"
#include "../Packet/ProtocolTypes.h"

using namespace packet;

class EN10MB_functions {
public:

    static link_layer_ref EN10MB_parse(const u_int8_t*, parse_context&);

};
#endif //EN10MB_H
