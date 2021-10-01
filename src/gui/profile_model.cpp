#include "profile_model.h"
#include "profile_persister.h"
#include <string>

ProfileModel::ProfileModel(QObject *parent, Fannn::ProfilePersister persister) : QAbstractItemModel(parent), persister(persister) {}

int ProfileModel::updateIntervalMs() {
    return persister.profile.getUpdateInterval();
}

void ProfileModel::setUpdateIntervalMs(int value) {
    if(value != persister.profile.getUpdateInterval()){
        persister.profile.setUpdateInterval(value);//todo: range exception
        emit updateIntervalMsChanged(value);
        setUnsavedChanges(true);
    }
}

void ProfileModel::save() {
    persister.save();
    setUnsavedChanges(false);
}
