#include "sensor_list_model.h"

#include <QtCore>
#include "plugins_composite_sensor_reader.h"
#include "user_level_logging_messages.h"

using Fannn::PluginsCompositeSensorReader;

SensorListModel::SensorListModel(QObject *parent) : QAbstractListModel(parent), readTimer(QTimer(this)) {
    connect(this, &SensorListModel::profileChanged,
            &SensorListModel::onProfileChanged);
    connect(&readTimer, &QTimer::timeout, [this](){
        emit dataChanged(index(0,0), index(rowCount()-1,0), {ValueRole});
    });
    scanForSensors();
}

QVariant SensorListModel::data(const QModelIndex &index, int role) const {
    auto& reader = PluginsCompositeSensorReader::instance();

    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(reader.getAll()[index.row()]);//todo: SLOW, DUMB
        case AliasRole: {
            if (!_profileModel)
                return QVariant();
            Fannn::Profile p = _profileModel->profile();
            std::string name = reader.getAll()[index.row()];//todo: SLOW, DUMB
            std::string alias = p.getAliasForSensor(name);
            return QString::fromStdString(alias);
        }
        case ValueRole:
            return QVariant{reader.read(reader.getAll()[index.row()])}.toString();//todo: SLOWER, DUMBER
        default:
            return QVariant();
    }
}

bool SensorListModel::removeAlias(int row) {
    if (!_profileModel)
        return false;

    std::string id = PluginsCompositeSensorReader::instance().getAll().at(row);
    std::string removedAlias = _profileModel->removeSensorAlias(id);

    bool removed = !removedAlias.empty();
    if (removed)
        emit dataChanged(index(row,0), index(row,0), {AliasRole});
    return removed;
}

void SensorListModel::scanForSensors() {
    auto& reader = PluginsCompositeSensorReader::instance();

    reader.scan();

    std::vector<Fannn::Plugins::PluginLoadError> errs = reader.getPluginLoadErrors();
    std::string errBody = {};
    int userErrorCount = 0;
    for (auto const & err : errs) {
        if (err.likelyUserError) {
            ++userErrorCount;
            errBody += "<br>" + err.path + ": " + err.msg;
        }
    }
    if (userErrorCount > 0) {
        /*
         * do this on the next gui loop iteration so that when this is called
         * as part of the initialization of a window,
         * the resulting message box ends up on top
         */
        QTimer::singleShot(0, [=]()-> void {
            const char * const errHead = (userErrorCount == 1) ?
                    "error loading plugin:" : "error loading plugins:";
            qCWarning(UserLevelMessageHandling::PLUGINS) << errHead << errBody.c_str();
        });
    }
}

ProfileModel::SensorAliasOrGovNameCollision SensorListModel::setAlias(int row, QString alias) {
    std::string id = PluginsCompositeSensorReader::instance().getAll().at(row);
    auto result = _profileModel->setSensorAlias(id, alias);

    if (result == ProfileModel::NoCollision)
        emit dataChanged(index(row,0), index(row,0), {AliasRole});

    return result;
}

double SensorListModel::readSensor(std::string id) {
    return Fannn::PluginsCompositeSensorReader::instance().read(id);
}

void SensorListModel::onProfileChanged(ProfileModel *value) {
    emit dataChanged(index(0,0), index(rowCount()-1,0), {AliasRole});
    readTimer.stop();

    for (auto& c : profileConnections)
        disconnect(c);
    profileConnections.clear();

    if (value) {
        readTimer.start(value->updateIntervalMs());

        profileConnections.push_back(connect(
            value, &ProfileModel::updateIntervalMsChanged,
            [this] (int value) { readTimer.start(value); }
        ));
    }
}

Qt::ItemFlags SensorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int SensorListModel::rowCount(const QModelIndex &parent) const {
    return PluginsCompositeSensorReader::instance().getAll().size();//todo: SLOW, DUMB
}
