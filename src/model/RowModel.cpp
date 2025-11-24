//
// Created by jeremiah tesfaye on 5/18/25.
//

#include <iostream>
#include <print>
#include <model/RowModel.h>
#include <span>

RowModel::RowModel(QObject* parent)
: QAbstractTableModel(parent)
, m_row_count(0)
{
}

QVariant RowModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole) {

        return {};

    }

    if (orientation == Qt::Horizontal && section < static_cast<int>(m_column_names.size())) {

        return m_column_names[section];

    }

    return QString::number(section + 1);

}

int RowModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_row_entries.size());

}

int RowModel::columnCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_column_names.size());

}

QVariant RowModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || role != Qt::DisplayRole) {

        return {};

    }

    int row = index.row();

    int column = index.column();

    if (row >= static_cast<int>(m_row_entries.size()) || column >= static_cast<int>(m_column_names.size())) {

        return {};

    }

    return m_row_entries[row][column];
}

size_t RowModel::row_entires_size() {
    return m_row_entries.size();
}

void RowModel::add_data(std::deque<packet::packet_ref>::iterator first, std::deque<packet::packet_ref>::iterator last) {


    int start_index = static_cast<int>(m_row_entries.size());
    int length = static_cast<int>(std::distance(first, last));

    beginInsertRows(QModelIndex(), start_index, start_index + length - 1);

    for (auto it = first; it != last; ++it) {

        const auto& ref = *it;
        auto pkt_row_entry = RowFactory::create_row(ref);
        auto row_array = pkt_row_entry.to_array();

        std::vector<QString> row_vector(row_array.size());
        std::ranges::transform(row_array, row_vector.begin(),
        [](const QString* ptr) {
            return *ptr;
        });

        m_row_entries.push_back(row_vector);
    }

    endInsertRows();

}







