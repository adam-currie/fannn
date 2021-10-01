#include "controller_list_model.h"
#include <string>

ControllerListModel::ControllerListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    //todo
    return QVariant();//debug
}

Qt::ItemFlags ControllerListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int ControllerListModel::rowCount(const QModelIndex &parent) const {
    //todo
    return 3;//debug
}
