//
// Created by jeremiah tesfaye on 10/23/25.
//

#include <util/TreeModelFactory.h>
#include <QList>

using namespace packet;


QStandardItem* TreeModelFactory::make_protocol_item(const ProtocolDetails& protocol) {
    // Create a top-level item with the protocol name
    QStandardItem* protocolItem = new QStandardItem(QString::fromStdString(protocol.name));

    // Add each field as a child row
    for (const auto& field : protocol.fields) {
        QStandardItem* fieldItem = new QStandardItem(QString::fromStdString(field));
        protocolItem->appendRow(fieldItem);
    }

    return protocolItem;
}

QStandardItemModel* TreeModelFactory::make_model(const std::vector<ProtocolDetails>& details, QObject* parent) {
    auto* model = new QStandardItemModel(parent);
    model->setHorizontalHeaderLabels(QStringList() << "Protocol Details");

    for (const auto& proto : details) {
        model->appendRow(make_protocol_item(proto));
    }

    return model;
}


