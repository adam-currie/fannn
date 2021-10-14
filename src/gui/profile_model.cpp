#include "profile_model.h"
#include "profile_persister.h"
#include <string>

using namespace std;

ProfileModel::ProfileModel(QObject *parent, Fannn::ProfilePersister persister)
    : QAbstractItemModel(parent), persister(persister) {
    setUnsavedChanges(persister.unsavedChanges());
}

int ProfileModel::updateIntervalMs() const {
    return persister.constProfile().getUpdateInterval();
}

void ProfileModel::setUpdateIntervalMs(int value) {
    if(value != persister.profile().getUpdateInterval()){
        persister.profile().setUpdateInterval(value);//todo: range exception
        emit updateIntervalMsChanged(value);
        setUnsavedChanges(persister.unsavedChanges());
    }
}

QString ProfileModel::name() {
    return QString::fromStdString(persister.getName());
}

void ProfileModel::save() {
    persister.save();
    //todo: if save fails we dont want to call setunsavedchanges
    setUnsavedChanges(false);
}


ProfileModel::SetAliasResult ProfileModel::addOrUpdateSensorAlias(QString id, QString alias) {
    bool govCollision, aliasCollision;
    bool setOrUpdated = persister
            .profile()
            .addOrUpdateSensorAlias(
                id.toStdString(),
                alias.toStdString(),
                govCollision,
                aliasCollision);

    if (setOrUpdated) {
        emit aliasesChanged();
        setUnsavedChanges(persister.unsavedChanges());
        return AliasSet;
    } else {
        if (govCollision) {
            return AliasCollidesWithGovernor;
        } else if (aliasCollision) {
            return AliasCollidesWithSensorAlias;
        } else {
            return AliasAlreadySet;
        }
    }
}

QString ProfileModel::removeAliasForSensor(QString id) {
    std::string removed = persister.profile()
            .removeAliasForSensor(id.toStdString());

    if (!removed.empty()) {
        emit aliasesChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }

    return QString::fromStdString(removed);
}

void ProfileModel::addOrUpdateGovernor(Fannn::Governor gov) {
    bool setOrUpdated = persister.profile()
            .addOrUpdateGovernor(gov);

    if (setOrUpdated) {
        emit governorsChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }
}

void ProfileModel::removeGovernor(string name) {
    bool removed = persister.profile()
            .removeGovernor(name);

    if (removed) {
        emit governorsChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }
}
