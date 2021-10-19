#include "profile_model.h"
#include "profile_persister.h"
#include <string>
#include <assert.h>

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


ProfileModel::SensorAliasOrGovNameCollision ProfileModel::addOrUpdateSensorAlias(QString id, QString alias) {
    bool govCollision, aliasCollision;
    bool success = persister
            .profile()
            .addOrUpdateSensorAlias(
                id.toStdString(),
                alias.toStdString(),
                govCollision,
                aliasCollision);

    if (success) {
        emit aliasesChanged();
        setUnsavedChanges(persister.unsavedChanges());
        return NoCollision;
    } else {
        return govCollision ? CollidesWithGovernor : CollidesWithSensorAlias;
    }
}

ProfileModel::SensorAliasOrGovNameCollision ProfileModel::updateGovernor(int index, Fannn::Governor gov) {
    bool govCollision, aliasCollision;
    bool success = persister
            .profile()
            .updateGovernor(
                index,
                gov,
                govCollision,
                aliasCollision);

    if (success) {
        emit governorsChanged();
        setUnsavedChanges(persister.unsavedChanges());
        return NoCollision;
    } else {
        return govCollision ? CollidesWithGovernor : CollidesWithSensorAlias;
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

void ProfileModel::addGovernor(Fannn::Governor gov) {
    bool a,b;
    assert (persister.profile().addGovernor(gov,a,b));
    emit governorsChanged();
    setUnsavedChanges(persister.unsavedChanges());
}

void ProfileModel::removeGovernor(int index) {
    persister.profile().removeGovernor(index);
    emit governorsChanged();
    setUnsavedChanges(persister.unsavedChanges());
}
