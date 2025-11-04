//
// Created by jeremiah tesfaye on 10/23/25.
//

#ifndef TREEMODELFACTORY_H
#define TREEMODELFACTORY_H

#include <QStandardItem>
#include <QStandardItemModel>
#include <packet/PacketUtil.h>

class TreeModelFactory {
public:

    static QStandardItemModel* make_model(const std::vector<packet::ProtocolDetails>& items, QObject* parent);
    static QStandardItem* make_protocol_item(const packet::ProtocolDetails &details);


};

#endif //TREEMODELFACTORY_H
