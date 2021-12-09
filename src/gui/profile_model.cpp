#include "profile_model.h"
#include "profile_persister.h"
#include "composite_sensor_reader.h"
#include "composite_device_writer.h"
#include <string>
#include <assert.h>

using namespace std;

using Fannn::CompositeSensorReader;
using Fannn::CompositeDeviceWriter;

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


ProfileModel::SensorAliasOrGovNameCollision ProfileModel::setSensorAlias(int index, QString alias) {
    string id = CompositeSensorReader::instance().getAll().at(index);
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

bool ProfileModel::setGovernorForController(int index, string governorName) {
    string id = CompositeDeviceWriter::instance().getAll().at(index);

    bool success = persister
            .profile()
            .setGovernorForController(id, governorName);

    if (success)
        setUnsavedChanges(persister.unsavedChanges());

    return success;
}

bool ProfileModel::removeController(int index) {
    string id = CompositeDeviceWriter::instance().getAll().at(index);

    bool success = persister
            .profile()
            .removeController(id);

    if (success)
        setUnsavedChanges(persister.unsavedChanges());

    return success;
}

string ProfileModel::removeSensorAlias(int index) {
    string id = CompositeSensorReader::instance().getAll().at(index);
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

void ProfileModel::addGovernor(Fannn::Governor gov) {
    bool a,b;
    bool added = persister.profile().addGovernor(gov,a,b);
    assert (added && !a && !b);
    emit governorsChanged();
    setUnsavedChanges(persister.unsavedChanges());
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
    bool success = persister
            .profile()
            .updateCurve(
                index,
                curve,
                nameCollision);

    if (success) {
        emit curvesChanged();
        setUnsavedChanges(persister.unsavedChanges());
    }

    return success;
}
