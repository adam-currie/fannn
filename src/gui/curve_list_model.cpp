#include "curve_list_model.h"
#include <string>

CurveListModel::CurveListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant CurveListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    //todo
    return QVariant();//debug
}

Qt::ItemFlags CurveListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int CurveListModel::rowCount(const QModelIndex &parent) const {
    //todo
    return 3;//debug
}
