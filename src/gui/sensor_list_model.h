#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <QTimer>
#include "profile_model.h"

//todo: display sensors separated by their individual implementations of ISensorReader,
//      so that they can be toggled in settings(only for display purposes(still use composite reader for actual reading))
class SensorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    QTimer readTimer;

    void onProfileChanged(ProfileModel* value);

    std::vector<QMetaObject::Connection> profileConnections;

    enum Roles {
        NameRole = Qt::UserRole + 1,
        AliasRole,
        ValueRole
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {NameRole, "name"},
        {AliasRole, "_alias"},
        {ValueRole, "value"}
    };

    public:
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }
        SensorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        Q_INVOKABLE bool removeAlias(int row);
        Q_INVOKABLE ProfileModel::SensorAliasOrGovNameCollision setAlias(int row, QString alias);
        Q_INVOKABLE double readSensor(std::string id);

    public slots:
        void scanForSensors();

    signals:
        void profileChanged(ProfileModel* value);
};
