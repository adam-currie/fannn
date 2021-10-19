#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"
#include "sensor_list_model.h"
#include "q_governor_error.h"

class GovernorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    Q_PROPERTY(SensorListModel* sensors MEMBER _sensorListModel)
    SensorListModel* _sensorListModel;

    void onProfileChanged(ProfileModel* value);

    std::vector<Fannn::Governor> const & governors() const {
        return _profileModel->constProfile().getGovernors();
    }

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1,
            ErrorsRole,
            ErrorStrRole
        };

        GovernorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void add();
        Q_INVOKABLE void remove(int row);
        Q_INVOKABLE ProfileModel::SensorAliasOrGovNameCollision rename(int row, QString newName);
        Q_INVOKABLE void setExpression(int row, QString exp);

    signals:
        void profileChanged(ProfileModel* value);
};
