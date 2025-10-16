//
// Created by jeremiah tesfaye on 5/18/25.
//

#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <util/PacketRefBuffer.h>

class DisplayModel : public QAbstractTableModel {

    Q_OBJECT

public:
    explicit DisplayModel(const std::shared_ptr<PacketRefBuffer>& buffer, QObject* parent = nullptr);

    ~DisplayModel() override = default;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void add_data(size_t start, size_t end);

private:

    std::shared_ptr<PacketRefBuffer> buffer;

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
