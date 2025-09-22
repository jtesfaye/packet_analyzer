//
// Created by jeremiah tesfaye on 9/16/25.
//


#include <util/RowFactory.h>
#include <layerx/ProtocolTypes.h>

row_entry RowFactory::layer4_top_row(
    const size_t index,
    const double time,
    size_t length,
    const TransportPDU *data,
    const NetworkPDU *addr_data) {

    return row_entry {
        QString::number(index),
        QString::number(time),
        QString::fromStdString(addr_data->src),
        QString::fromStdString(addr_data->dest),
        QString::fromStdString(data->name()),
        QString::number(length),
        QString::fromStdString(data->make_info())
    };
}

row_entry RowFactory::layerx_top_row(
    size_t index,
    double time,
    size_t length,
    const ProtocolDataUnit *data) {

    return row_entry {
        QString::number(index),
        QString::number(time),
        QString::fromStdString(data->src),
        QString::fromStdString(data->dest),
        QString::fromStdString(data->name()),
        QString::number(length),
        QString::fromStdString(data->make_info())
    };
}

row_entry RowFactory::create_row(packet_ref &ref) {

    //if layer4 is defined, info and protocol field will be in place for that layer
    if (ref.layer4) {

        return layer4_top_row(
            ref.index,
            ref.time,
            ref.length,
            ref.layer4.get(),
            ref.layer3.get()
            );
    }

    //if layer3 is defined, that layer will populate info and protocol field
    if (ref.layer3) {

        return layerx_top_row(
            ref.index,
            ref.time,
            ref.length,
            ref.layer3.get()
            );
    }

    //otherwise default to layer2 data
    return layerx_top_row(
        ref.index,
        ref.time,
        ref.length,
        ref.layer2.get()
        );
}
