//
// Created by jeremiah tesfaye on 10/23/25.
//

#include <util/TreeModelFactory.h>
#include <QList>

QStandardItem *TreeModelFactory::make_level(
    std::string& name,
    std::vector<std::string> &&data) {

    QStandardItem* top_level = new QStandardItem(name.data());

    for (const std::string& d : data) {
        top_level->appendRow(new QStandardItem(d.data()));
    }

    return top_level;

}

QStandardItemModel* TreeModelFactory::make_model(
    std::vector<QStandardItem *> &items) {

    auto* model = new QStandardItemModel;

    for (auto i : items) {
        model->appendRow(i);
    }

    return model;
}




