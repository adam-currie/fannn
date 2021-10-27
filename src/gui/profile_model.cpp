#include "profile_model.h"
#include "profile_persister.h"
#include "composite_sensor_reader.h"
#include <string>
#include <assert.h>

using namespace std;

using Fannn::CompositeSensorReader;

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

bool ProfileModel::hasIssues() {
    return constProfile().hasIssues();
}

bool ProfileModel::hasSensor(string idOrAlias) const {
    for (auto const & sa : constProfile().getSensorAliases()) {
        if (idOrAlias == sa.alias){
            if (CompositeSensorReader::instance().hasSensor(sa.id))
                return true;
            break;
        }
    }
    return CompositeSensorReader::instance().hasSensor(idOrAlias);
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
            .addOrUpdateSensorAlias(
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
