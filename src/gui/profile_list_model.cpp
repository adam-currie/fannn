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

void ProfileListModel::loadProfileNames() {
    auto newNames = Fannn::ProfilePersister::getProfileNames();
    if (newNames != profileNames){
        beginResetModel();
        profileNames = newNames;
        endResetModel();
    }
}

int ProfileListModel::indexOf(QString profileName) {
    for(int i=0; i<profileNames.size(); i++)
        if (profileNames[i] == profileName.toStdString())
            return i;
    throw std::out_of_range("'" + profileName.toStdString() + "' not found");
}

void ProfileListModel::createAndSwitchTo() {
    std::string newName;
    int i = 0;

tryNextName:
    newName = "profile" + std::to_string(++i);
    for (const std::string& name : profileNames)
        if (name == newName)
            goto tryNextName;

    Fannn::ProfilePersister pp(newName);
    pp.save(); //need to mark our territory
    loadProfileNames();
    setCurrentProfile(new ProfileModel(this, pp));
}

void ProfileListModel::loadProfile(QString name) {
    Fannn::ProfilePersister persister(name.toStdString());
    persister.load();//todo: LoadError
    setCurrentProfile(new ProfileModel(this, persister));
}
