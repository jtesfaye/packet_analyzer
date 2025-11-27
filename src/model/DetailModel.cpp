//
// Created by jeremiah tesfaye on 11/24/25.
//

#include <model/DetailModel.h>

struct Node {
    const packet::ProtocolDetails* proto;
    int fieldIndex; // -1 means protocol node
};

QModelIndex DetailModel::index(int row, int column, const QModelIndex &parent) const {

    if (!details) return {};

    if (!parent.isValid()) {
        if (row >= details->size()) return {};
        auto* proto = &(*details)[row];
        auto* node = new Node{proto, -1};
        return createIndex(row, column, node);
    }

    auto* parentNode = static_cast<Node*>(parent.internalPointer());
    if (parentNode->fieldIndex != -1) return {}; // fields have no children

    if (row >= parentNode->proto->fields.size()) return {};
    auto* node = new Node{parentNode->proto, row};
    return createIndex(row, column, node);
}

QModelIndex DetailModel::parent(const QModelIndex &index) const {

    if (!index.isValid()) return {};

    auto* node = static_cast<Node*>(index.internalPointer());

    // protocol node → no parent
    if (node->fieldIndex == -1) {
        return {};
    }

    // field node → parent is protocol
    auto* proto = node->proto;

    // find protocol index
    for (size_t i = 0; i < details->size(); ++i) {
        if (&(*details)[i] == proto) {
            auto* parentNode = new Node{proto, -1};
            return createIndex(static_cast<int>(i), 0, parentNode);
        }
    }

    return {};
}

int DetailModel::rowCount(const QModelIndex& parent) const {
    if (!details) return 0;

    if (!parent.isValid()) return details->size();

    auto* node = static_cast<Node*>(parent.internalPointer());
    if (node->fieldIndex == -1) return node->proto->fields.size();
    return 0;
}

int DetailModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant DetailModel::data(const QModelIndex& idx, int role) const {

    if (role != Qt::DisplayRole) return {};

    auto* node = static_cast<Node*>(idx.internalPointer());

    // protocol node
    if (node->fieldIndex == -1) {
        if (idx.column() == 0)
            return QString::fromStdString(node->proto->name.data());
        return {};
    }

    // field node
    if (idx.column() == 0)
        return QString::fromStdString(node->proto->fields[node->fieldIndex]);

    return {};
}

void DetailModel::set_data(const std::vector<packet::ProtocolDetails>& d) {
    details_store = d;
    beginResetModel();
    details = &details_store;
    endResetModel();
}

