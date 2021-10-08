#include "profile_list_model.h"
#include "profile_persister.h"
#include <stdexcept>
#include <string>

ProfileListModel::ProfileListModel(QObject *parent) : QAbstractListModel(parent) {
    profileNames = Fannn::ProfilePersister::getProfileNames();
}

QVariant ProfileListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    return QString::fromStdString(profileNames.at(index.row()));
}

Qt::ItemFlags ProfileListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int ProfileListModel::rowCount(const QModelIndex &parent) const {
    return profileNames.size();
}

QHash<int, QByteArray> ProfileListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

void ProfileListModel::loadProfileNames() {
    auto newNames = Fannn::ProfilePersister::getProfileNames();
    if (newNames != profileNames){
        //need to use begin and end functions or the private signals wont fire
        beginRemoveRows(QModelIndex(), 0, profileNames.size()-1);
        profileNames.clear();
        endRemoveRows();
        beginInsertRows(QModelIndex(), 0, newNames.size()-1);
        profileNames = newNames;
        endInsertRows();
    }
}

int ProfileListModel::indexOf(QString profileName) {
    for(int i=0; i<profileNames.size(); i++)
        if (profileNames[i] == profileName.toStdString())
            return i;
    throw std::out_of_range("'" + profileName.toStdString() + "' not found");
}

void ProfileListModel::loadProfile(QString name) {
    Fannn::ProfilePersister persister(name.toStdString());
    //persister.save();//debug
    persister.load();//todo: LoadError

    //todo: delete old profile model?

    setCurrentProfile(new ProfileModel(this, persister));
}
