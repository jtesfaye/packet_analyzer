//
// Created by jeremiah tesfaye on 5/18/25.
//

#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <deque>
#include <packet/PacketUtil.h>
#include <span>
#include <util/RowFactory.h>

class RowModel : public QAbstractTableModel {

    Q_OBJECT

public:
    explicit RowModel(QObject* parent = nullptr);

    ~RowModel() override = default;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    size_t row_entires_size();

public slots:

    void add_data(std::deque<packet::packet_ref>::iterator first, std::deque<packet::packet_ref>::iterator last);

private:

    int m_row_count;

    std::vector<std::vector<QString>> m_row_entries;

    const std::vector<QString> m_column_names = {
        "No.",
        "Time",
        "Source",
        "Destination",
        "Protocol",
        "Length",
        "Info"};

};



#endif //DISPLAYMODEL_H
