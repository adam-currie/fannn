#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class ProfileListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* currentProfile READ currentProfile NOTIFY currentProfileChanged)
    ProfileModel* _currentProfile = nullptr;
    void setCurrentProfile(ProfileModel* value);

    Q_PROPERTY(QString activeProfileName READ activeProfileName WRITE setActiveProfileName NOTIFY activeProfileNameChanged)

    std::vector<std::string> profileNames;

    enum Roles {
        NameRole = Qt::UserRole + 1
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {NameRole, "name"}
    };

    public:
        ProfileListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent) const override;
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }

        Q_INVOKABLE int indexOf(QString profileName);//todo: try string
        Q_INVOKABLE void createAndSwitchTo();

        ProfileModel* currentProfile() const { return _currentProfile; }
        QString activeProfileName() const;

        void setActiveProfileName(QString profileName);

    signals:
        void currentProfileChanged(ProfileModel* value);
        void activeProfileNameChanged(QString profileName);

    public slots:
        void loadProfile(QString name);
        void loadProfileNames();
};
