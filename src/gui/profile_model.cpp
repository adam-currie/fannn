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
    return persister.profile().getUpdateInterval();
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

bool ProfileModel::hasIssues() {
    return profile().hasIssues();
}

void ProfileModel::save() {
    persister.save();
    //todo: if save fails we dont want to call setunsavedchanges
    setUnsavedChanges(false);
}


ProfileModel::SensorAliasOrGovNameCollision ProfileModel::setSensorAlias(string id, QString alias) {
    bool govCollision, aliasCollision;
    bool success = persister
            .profile()
            .setSensorAlias(
                id,
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

bool ProfileModel::setGovernorForController(string id, string governorName) {
    bool success = persister
            .profile()
            .setGovernorForController(id, governorName);

    if (success)
        setUnsavedChanges(persister.unsavedChanges());

    return success;
}

bool ProfileModel::removeController(string id) {
    bool success = persister
            .profile()
            .removeController(id);

    if (success)
        setUnsavedChanges(persister.unsavedChanges());

    return success;
}

string ProfileModel::removeSensorAlias(string id) {
    string removedAlias = persister.profile()
            .removeAliasForSensor(id);

    if (!removedAlias.empty()) {
        emit aliasesChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }

    return removedAlias;
}

ProfileModel::SensorAliasOrGovNameCollision ProfileModel::updateGovernor(int index, Fannn::Governor const & gov) {
    bool govCollision, aliasCollision;
    bool changed = persister
            .profile()
            .updateGovernor(
                index,
                gov,
                govCollision,
                aliasCollision);

    if (changed) {
        emit governorsChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }

    if (govCollision) {
        return CollidesWithGovernor;
    } else if (aliasCollision) {
        return CollidesWithSensorAlias;
    } else {
        return NoCollision;
    }
}

ProfileModel::SensorAliasOrGovNameCollision ProfileModel::addGovernor(Fannn::Governor gov) {
    bool govCollision, aliasCollision;
    persister.profile().addGovernor(gov, govCollision, aliasCollision);

    if (govCollision) {
        return CollidesWithGovernor;
    } else if (aliasCollision) {
        return CollidesWithSensorAlias;
    } else {
        emit governorsChanged();
        setUnsavedChanges(persister.unsavedChanges());
        return NoCollision;
    }
}

void ProfileModel::removeGovernor(int index) {
    persister.profile().removeGovernor(index);
    emit governorsChanged();
    setUnsavedChanges(persister.unsavedChanges());
}

void ProfileModel::addCurve(Fannn::Curve  curve) {
    bool added = persister.profile().addCurve(curve);
    assert (added);
    emit curvesChanged();
    setUnsavedChanges(persister.unsavedChanges());
}

void ProfileModel::removeCurve(int index) {
    persister.profile().removeCurve(index);
    emit curvesChanged();
    setUnsavedChanges(persister.unsavedChanges());
}

bool ProfileModel::updateCurve(int index, Fannn::Curve curve) {
    bool nameCollision;//todo
    bool changed = persister
            .profile()
            .updateCurve(
                index,
                curve,
                nameCollision);

    if (changed) {
        emit curvesChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }

    return changed;
}
