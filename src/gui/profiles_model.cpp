#include "profiles_model.h"
#include "profile_persister.h"
#include <string>

ProfilesModel::ProfilesModel(QObject *parent) : QAbstractListModel(parent) {
    throw std::runtime_error("debug");//debug
}

QVariant ProfilesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    std::vector<std::string> names = Fannn::ProfilePersister::getProfileNames();
    return QString::fromStdString(names.at(index.row()));
}

Qt::ItemFlags ProfilesModel::flags(const QModelIndex &index) const {
    if (index.isValid())
        return (QAbstractListModel::flags(index)|Qt::ItemIsDragEnabled);

    return Qt::NoItemFlags;
}

int ProfilesModel::rowCount(const QModelIndex &parent) const {
    return Fannn::ProfilePersister::getProfileNames().size();
}
