#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"
#include "composite_sensor_reader.h"

//todo: display sensors seperated by their individual implementations of ISensorReader,
//      so that they can be toggled in settings(only for display purposes(still use composite reader for actual reading))
class SensorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel)
    ProfileModel* _profileModel;

    Fannn::CompositeSensorReader compositeReader;

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1,
            AliasRole,
            ValueRole
        };

        SensorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent) const override;
        QHash<int, QByteArray> roleNames() const override;
};
