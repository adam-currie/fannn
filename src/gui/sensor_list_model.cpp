#include "sensor_list_model.h"

SensorListModel::SensorListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant SensorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(compositeReader.getAll()[index.row()]);//todo: SLOW, DUMB
        case AliasRole:
            return "todo";
        case ValueRole:
            return compositeReader.getValue(compositeReader.getAll()[index.row()]);//todo: SLOWER, DUMBER
        default:
            return QVariant();
    }
}

Qt::ItemFlags SensorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int SensorListModel::rowCount(const QModelIndex &parent) const {
    return compositeReader.getAll().size();//todo: SLOW, DUMB
}

QHash<int, QByteArray> SensorListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AliasRole] = "_alias";
    roles[ValueRole] = "value";
    return roles;
}
