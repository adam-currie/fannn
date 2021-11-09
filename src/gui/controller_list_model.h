#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class ControllerListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    std::vector<QMetaObject::Connection> profileConnections;

    enum Roles {
        NameRole = Qt::UserRole + 1,
        GovernorNameRole
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {NameRole, "name"},
        {GovernorNameRole, "governorName"}
    };

    public:
        QHash<int, QByteArray> roleNames() const override {
            return rolesHash;
        }
        ControllerListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = NameRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    signals:
        void profileChanged(ProfileModel* value);
};
