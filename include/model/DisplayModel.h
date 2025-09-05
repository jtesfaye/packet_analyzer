//
// Created by jeremiah tesfaye on 5/18/25.
//

#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <parsing/PacketParse.h>
#include <packet/ProtocolTypes.h>
#include <util/PacketBuffer.h>

class DisplayModel : public QAbstractTableModel {

    Q_OBJECT

public:
    explicit DisplayModel(QObject* parent = nullptr);

    ~DisplayModel() override = default;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;


public slots:

    void add_data(const proccessed_packet&);

    [[nodiscard]] layer_details& get_details(int row);

private:

    int m_row_count;

    std::vector<std::vector<std::string>> m_row_entries;

    std::vector<layer_details> m_layer_details;

    const std::vector<QString> m_column_names = { "Source", "Destination", "Protocol", "info"};


};



#endif //DISPLAYMODEL_H
