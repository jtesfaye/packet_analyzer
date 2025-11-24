//
// Created by jeremiah tesfaye on 11/24/25.
//

#include <model/DetailModel.h>

QModelIndex DetailModel::index(int row, int column, const QModelIndex &parent) const {

    if (!details)
        return {};

    if (!parent.isValid()) {
        if (row >= details->size())
            return {};

        auto* protocol = &(*details)[row];
        return createIndex(row, column, protocol);
    }

    auto* proto = static_cast<const packet::ProtocolDetails*> (parent.internalPointer());
    if (row >= proto->fields.size())
        return {};

    return createIndex(row, column, proto);
}

QModelIndex DetailModel::parent(const QModelIndex &index) const {

    if (!index.isValid())
        return {};

    auto* proto = static_cast<const packet::ProtocolDetails*> (index.internalPointer());

    for (size_t i = 0; i < details->size(); ++i) {
        if ((&(*details)[i]) == proto) {
            return {};
        }
    }

    for (size_t i = 0; i < details->size(); ++i) {
        if (&(*details)[i] == proto) {
            return createIndex(i, 0, proto);
        }
    }

    return {};
}

int DetailModel::rowCount(const QModelIndex& parent) const {
    if (!details) return 0;

    if (!parent.isValid()) {
        return details->size();  // number of protocols
    } else {
        auto* proto = static_cast<const packet::ProtocolDetails*>(parent.internalPointer());
        return proto->fields.size(); // number of fields
    }
}

int DetailModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant DetailModel::data(const QModelIndex& idx, int role) const {

    if (role != Qt::DisplayRole) return {};

    auto* proto = static_cast<const packet::ProtocolDetails*>(idx.internalPointer());

    // protocol nodes
    if (!idx.parent().isValid()) {
        return QString::fromStdString(std::string{proto->name});
    }

    // field nodes
    int row = idx.row();
    return QString::fromStdString(proto->fields[row]);
}

void DetailModel::set_data(const std::vector<packet::ProtocolDetails> *d) {
    details = d;
}


