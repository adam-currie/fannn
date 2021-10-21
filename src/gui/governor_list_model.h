#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"
#include "sensor_list_model.h"
#include "q_governor_error.h"

class GovernorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile READ profileModel WRITE setProfileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    std::vector<Fannn::Governor> const & governors() const {
        return _profileModel->constProfile().getGovernors();
    }

    void onProfileChanged(ProfileModel* value);

    void validateGovNameLookups(Fannn::Governor& gov);
    void validateAllGovNameLookups();

     std::vector<QMetaObject::Connection> profileConnections;

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1,
            ExpressionRole,
            ErrorsRole,
            ErrorStrRole
        };

        GovernorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        ProfileModel* profileModel() { return _profileModel; }
        void setProfileModel(ProfileModel* value);

        Q_INVOKABLE void add();
        Q_INVOKABLE void remove(int row);
        Q_INVOKABLE ProfileModel::SensorAliasOrGovNameCollision rename(int row, QString newName);
        Q_INVOKABLE void setExpression(int row, QString exp);

    signals:
        void profileChanged(ProfileModel* value);
};
