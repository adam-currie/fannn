#include "profile_list_model.h"
#include "profile_persister.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include "contains.h"

using std::string;

ProfileListModel::ProfileListModel(QObject *parent) : QAbstractListModel(parent) {
    auto persister = Fannn::ProfilePersister(Fannn::ProfilePersister::getActiveProfileName());
    persister.load();
    setCurrentProfile(new ProfileModel(this, persister));
}
void ProfileListModel::setCurrentProfile(ProfileModel *value) {
    if(_currentProfile != value){
        //todo: delete old profile model?
        _currentProfile = value;
        emit currentProfileChanged(value);
        loadProfileNames();
    }
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

    if (_currentProfile) {
        //put at front
        //and make sure it's on the list even if not read from disk, cause we can just save it anyway
        string curName = _currentProfile->name().toStdString();
        for (auto iter = newNames.begin(); iter != newNames.end(); ++iter) {
            if (*iter == curName) {
                newNames.erase(iter);
                break;
            }
        }
        newNames.insert(newNames.begin(), curName);
    }

    if (newNames != profileNames) {
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
    //we want this to be up to date
    auto latestProfileNames = Fannn::ProfilePersister::getProfileNames();

    string name = "profile1";
    for (int i=2; latestProfileNames>>contains(name); ++i)
        name = "profile" + std::to_string(i);

    Fannn::ProfilePersister pp(name);
    pp.save(); //need to mark our territory
    setCurrentProfile(new ProfileModel(this, pp));
}

void ProfileListModel::loadProfile(QString name) {
    Fannn::ProfilePersister persister(name.toStdString());
    persister.load();//todo: LoadError
    setCurrentProfile(new ProfileModel(this, persister));
}

void ProfileListModel::setActiveProfileName(QString profileName) {
    string newName = profileName.toStdString();
    string oldName = Fannn::ProfilePersister::getActiveProfileName();

    if (newName != oldName) {
        Fannn::ProfilePersister::setActiveProfileName(newName);
        emit activeProfileNameChanged(QString::fromStdString(newName));
    }
}

QString ProfileListModel::activeProfileName() const {
    return QString::fromStdString(Fannn::ProfilePersister::getActiveProfileName());
}
