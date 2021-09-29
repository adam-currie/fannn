#include "profile_list_model.h"
#include "profile_persister.h"
#include <string>

ProfileListModel::ProfileListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant ProfileListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    std::vector<std::string> names = Fannn::ProfilePersister::getProfileNames();

    return QString::fromStdString(names.at(index.row()));
}

Qt::ItemFlags ProfileListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::ItemIsDropEnabled;//debug
}

int ProfileListModel::rowCount(const QModelIndex &parent) const {
    return Fannn::ProfilePersister::getProfileNames().size();
}

QHash<int, QByteArray> ProfileListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
