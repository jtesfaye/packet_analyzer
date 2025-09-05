//
// Created by jeremiah tesfaye on 5/20/25.
//


#include "../../include/util/PacketBuffer.h"
#include <iostream>
#include <fstream>

PacketBuffer::PacketBuffer(QObject* parent) :
m_size(0)
{}



void
PacketBuffer::add(packet_ref& parsed_pkt) {

    mutex.lock();

    std::cout << "Packet being added\n";

    proccessed_packet pkt
    {
        PacketRead::create_row_entry(parsed_pkt)
        , PacketRead::copy(parsed_pkt.l2)
        ,std::nullopt

    };

    packet_buffer.push_back(pkt);

    emit new_packet();

    ++m_size;

    mutex.unlock();

}

proccessed_packet PacketBuffer::read() {

    mutex.lock();

    std::cout << "Packet being read\n";
    proccessed_packet pkt {packet_buffer.back()};

    packet_buffer.pop_back();
    --m_size;

    mutex.unlock();

    return pkt;

}


size_t PacketBuffer::size() {

        return m_size;
}


