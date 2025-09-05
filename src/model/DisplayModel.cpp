//
// Created by jeremiah tesfaye on 5/18/25.
//

#include <model/DisplayModel.h>

#include <iostream>


DisplayModel::DisplayModel(QObject* parent)
: QAbstractTableModel(parent)
, m_row_count(0)
{
}



QVariant
DisplayModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole) {

        return {};

    }

    if (orientation == Qt::Horizontal && section < static_cast<int>(m_column_names.size())) {

        return m_column_names[section];

    }

    return QString::number(section + 1);

}

int
DisplayModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_row_entries.size());

}

int
DisplayModel::columnCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_column_names.size());

}

QVariant
DisplayModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || role != Qt::DisplayRole) {

        return {};

    }

    int row = index.row();

    int column = index.column();

    if (row >= static_cast<int>(m_row_entries.size()) || column >= static_cast<int>(m_column_names.size())) {


        return {};

    }

    return m_row_entries[row][column].data();

}



void
DisplayModel::add_data(const proccessed_packet& pkt) {

    int row = static_cast<int>(m_row_entries.size());

    beginInsertRows(QModelIndex(), row, row);

    m_row_entries.push_back(pkt.packet_row_entry);
    m_layer_details.push_back(pkt.details);

    endInsertRows();
}

layer_details& DisplayModel::get_details(const int row) {

    return m_layer_details[row];

}




