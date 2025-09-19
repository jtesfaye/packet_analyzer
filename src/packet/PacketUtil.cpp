//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <packet/PacketUtil.h>
#include <layerx/layer2/Layer2Types.h>
#include <layerx/layer3/Layer3Types.h>
#include <layerx/layer4/Layer4Types.h>

using namespace packet;
packet_ref::~packet_ref() = default;

packet_ref::packet_ref() = default;

packet_ref::packet_ref(packet_ref&&) noexcept = default;

packet_ref& packet_ref:: operator=(packet_ref&&) noexcept = default;