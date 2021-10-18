#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <QTimer>
#include "profile_model.h"
#include "composite_sensor_reader.h"

//todo: display sensors seperated by their individual implementations of ISensorReader,
//      so that they can be toggled in settings(only for display purposes(still use composite reader for actual reading))
class SensorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    QTimer readTimer;
    Fannn::CompositeSensorReader compositeReader;

    void onProfileChanged(ProfileModel* value);

    std::vector<QMetaObject::Connection> profileConnections;

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1,
            AliasRole,
            ValueRole
        };

        SensorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        bool hasSensor(std::string sensor) const;
        double readSensor(std::string sensor) const;

    signals:
        void profileChanged(ProfileModel* value);
};
