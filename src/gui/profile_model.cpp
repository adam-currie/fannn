#include "profile_model.h"
#include "profile_persister.h"
#include <string>

using namespace std;

ProfileModel::ProfileModel(QObject *parent, Fannn::ProfilePersister persister)
    : QAbstractItemModel(parent), persister(persister) {
    setUnsavedChanges(persister.unsavedChanges());
}

int ProfileModel::updateIntervalMs() const {
    const Fannn::Profile c = persister.constProfile();
    return c.getUpdateInterval();
}

void ProfileModel::setUpdateIntervalMs(int value) {
    if(value != persister.profile().getUpdateInterval()){
        persister.profile().setUpdateInterval(value);//todo: range exception
        emit updateIntervalMsChanged(value);
        setUnsavedChanges(persister.unsavedChanges());
    }
}

void ProfileModel::save() {
    persister.save();
    setUnsavedChanges(persister.unsavedChanges());
}
