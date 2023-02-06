#include "path_list_model.h"

#include <QtCore>


QVariant PathListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    return QString::fromStdString(index.row()?"one":"two");
}

Qt::ItemFlags PathListModel::flags(const QModelIndex &index) const {
    return Qt::NoItemFlags;
}

int PathListModel::rowCount(const QModelIndex &parent) const {
    return 2;//todo
}

int PathListModel::columnCount(const QModelIndex &parent) const {
    return 1;//todo
}
