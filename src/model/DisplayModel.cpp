//
// Created by jeremiah tesfaye on 5/18/25.
//

#include <iostream>
#include <print>
#include <model/DisplayModel.h>
#include <util/RowFactory.h>

DisplayModel::DisplayModel(PacketRefBuffer& buffer, QObject* parent)
: QAbstractTableModel(parent)
, buffer(buffer)
, m_row_count(0)
{
}

QVariant DisplayModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole) {

        return {};

    }

    if (orientation == Qt::Horizontal && section < static_cast<int>(m_column_names.size())) {

        return m_column_names[section];

    }

    return QString::number(section + 1);

}

int DisplayModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_row_entries.size());

}

int DisplayModel::columnCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_column_names.size());

}

QVariant DisplayModel::data(const QModelIndex &index, int role) const
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

void DisplayModel::add_data(size_t start, size_t end) {

    int length = end - start;
    int row = static_cast<int>(m_row_entries.size());

    std::println("Adding to model: {} to {}", start, end);

    beginInsertRows(QModelIndex(), row, row + length);

    for (size_t i = start; i <= end; i++) {

        auto pkt_row_entry = RowFactory::create_row(buffer.get_ref(i));
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





