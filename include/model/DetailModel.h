//
// Created by jeremiah tesfaye on 11/24/25.
//

#ifndef DETAILMODEL_H
#define DETAILMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <packet/PacketUtil.h>

class DetailModel : public QAbstractItemModel {

    Q_OBJECT

public:

    explicit DetailModel(QObject *parent = nullptr) : QAbstractItemModel(parent) {};
    ~DetailModel() override = default;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void set_data(const std::vector<packet::ProtocolDetails>& d);

private:

    const std::vector<packet::ProtocolDetails> *details = nullptr;
    std::vector<packet::ProtocolDetails> details_store;

};

#endif //DETAILMODEL_H
