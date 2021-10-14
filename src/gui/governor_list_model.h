#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class GovernorListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile MEMBER _profileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    void onProfileChanged(ProfileModel* value);

    std::vector<QMetaObject::Connection> profileConnections;

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1
        };

        GovernorListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void add();

    signals:
        void profileChanged(ProfileModel* value);
};
