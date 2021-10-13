#include "sensor_list_model.h"

SensorListModel::SensorListModel(QObject *parent) : QAbstractListModel(parent), readTimer(QTimer(this)) {
    connect(this, &SensorListModel::profileChanged,
            &SensorListModel::onProfileChanged);
    connect(&readTimer, &QTimer::timeout, [this](){
        emit dataChanged(index(0,0), index(rowCount()-1,0), {ValueRole});
    });
}

QVariant SensorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(compositeReader.getAll()[index.row()]);//todo: SLOW, DUMB
        case AliasRole: {
            if (!_profileModel)
                return QVariant();
            Fannn::Profile p = _profileModel->constProfile();
            std::string name = compositeReader.getAll()[index.row()];//todo: SLOW, DUMB
            return QString::fromStdString(p.getAliasForSensor(name));
        }
        case ValueRole:
            return QVariant{compositeReader.getValue(compositeReader.getAll()[index.row()])}.toString();//todo: SLOWER, DUMBER
        default:
            return QVariant();
    }
}

void SensorListModel::onProfileChanged(ProfileModel *value) {
    emit dataChanged(index(0,0), index(rowCount()-1,0), {AliasRole});
    readTimer.stop();
    for (auto c : profileConnections) disconnect(c);
    profileConnections.clear();
    if (value) {
        profileConnections.push_back(connect(
            value, &ProfileModel::aliasesChanged,
            [this] () { emit dataChanged(index(0,0), index(rowCount()-1,0), {AliasRole}); }
        ));
        profileConnections.push_back(connect(
            value, &ProfileModel::updateIntervalMsChanged,
            [this] (int value) { readTimer.start(value); }
        ));
        readTimer.start(value->updateIntervalMs());
    }
}

Qt::ItemFlags SensorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int SensorListModel::rowCount(const QModelIndex &parent) const {
    return compositeReader.getAll().size();//todo: SLOW, DUMB
}

QHash<int, QByteArray> SensorListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AliasRole] = "_alias";
    roles[ValueRole] = "value";
    return roles;
}
