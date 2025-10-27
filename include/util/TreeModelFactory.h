//
// Created by jeremiah tesfaye on 10/23/25.
//

#ifndef TREEMODELFACTORY_H
#define TREEMODELFACTORY_H

#include <QStandardItem>
#include <QStandardItemModel>

class TreeModelFactory {
public:

    static QStandardItemModel* make_model(std::vector<QStandardItem*>& items);
    static QStandardItem* make_level(std::string& name, std::vector<std::string>&& data);

};

#endif //TREEMODELFACTORY_H
