//
// Created by jeremiah tesfaye on 6/29/25.
//

#include "../../include/util/PacketGetter.h"

#include <iostream>

PacketGetter::PacketGetter(PacketBuffer* buffer, QObject *parent)
: buffer(buffer)
{
}


void PacketGetter::get_packet() {

    proccessed_packet pkt = buffer->read();

    emit packet_ready(pkt);

}

