#include "sensor_list_model.h"

SensorListModel::SensorListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant SensorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

//debug
//    if (role == Qt::DecorationRole)
//        return QIcon(pixmaps.value(index.row()).scaled(m_PieceSize, m_PieceSize,
//                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    else if (role == Qt::UserRole)
//        return pixmaps.value(index.row());
//    else if (role == Qt::UserRole + 1)
//        return locations.value(index.row());
//    return QVariant();

    return 666;//todo
}

Qt::ItemFlags SensorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int SensorListModel::rowCount(const QModelIndex &parent) const {
    return 3;//todo
}

QHash<int, QByteArray> SensorListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AliasRole] = "_alias";
    roles[ValueRole] = "value";
    return roles;
}
