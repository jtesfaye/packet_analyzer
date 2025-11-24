//
// Created by jeremiah tesfaye on 9/16/25.
//


#include <iostream>
#include <util/RowFactory.h>

row_entry RowFactory::layer4_top_row(
    const size_t index,
    const double time,
    size_t length,
    const TransportPDU *data,
    const NetworkPDU *addr_data) {



    return row_entry {
        QString::number(index),
        QString::number(time, 'f', 6),
        QString::fromStdString(addr_data->address_to_string(addr_data->src())),
        QString::fromStdString(addr_data->address_to_string(addr_data->dest())),
        QString::fromStdString(data->name().data()),
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
        QString::number(time, 'f', 6),
        QString::fromStdString(data->address_to_string(data->src())),
        QString::fromStdString(data->address_to_string(data->dest())),
        QString::fromStdString(data->name().data()),
        QString::number(length),
        QString::fromStdString(data->make_info())
    };
}

row_entry RowFactory::layerx_error_row(size_t index,
    double time) {

    return row_entry {
        QString::number(index),
        QString::number(time, 'f', 6),
        "null",
        "null",
        "null",
        "null",
        "Error with packet"
    };

}

row_entry RowFactory::create_row(const packet::packet_ref &ref) {

    //if layer4 is defined, info and protocol field will be in place for that layer
    if (ref.layer4) {

        return layer4_top_row(
            ref.index,
            ref.time,
            ref.length,
            dynamic_cast<const TransportPDU *>(ref.layer4.get()),
            dynamic_cast<const NetworkPDU *>(ref.layer3.get())
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


    if (ref.layer2) {

        return layerx_top_row(
        ref.index,
        ref.time,
        ref.length,
        ref.layer2.get()
        );

    }

    return layerx_error_row(ref.index, ref.time);
}
