#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class ProfileListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ProfileModel* currentProfile READ currentProfile NOTIFY currentProfileChanged)
    ProfileModel* _currentProfile = nullptr;//
    void setCurrentProfile(ProfileModel* value){
        if(_currentProfile != value){
            _currentProfile = value;
            emit currentProfileChanged(value);
        }
    }

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1
            //todo:more roles will start after userrole + 1
        };

        ProfileListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent) const override;
        QHash<int, QByteArray> roleNames() const override;

        ProfileModel* currentProfile() { return _currentProfile; }

    signals:
        void currentProfileChanged(ProfileModel* value);

    public slots:
        void loadProfile(QString name);
};
