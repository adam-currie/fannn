#include "governor_list_model.h"
#include <string>

GovernorListModel::GovernorListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant GovernorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    //todo
    return QVariant();//debug
}

Qt::ItemFlags GovernorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::ItemIsDropEnabled;//debug
}

int GovernorListModel::rowCount(const QModelIndex &parent) const {
    //todo
    return 3;//debug
}
