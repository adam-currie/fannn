#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class ControllerListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile READ profileModel WRITE setProfileModel NOTIFY profileChanged)
    ProfileModel* _profileModel = nullptr;

    std::vector<QMetaObject::Connection> profileConnections;

    enum Roles {
        NameRole = Qt::UserRole + 1,
        GovernorChoices,
        SelectedGovernorIndex
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {NameRole, "name"},
        {GovernorChoices, "governorChoices"},
        {SelectedGovernorIndex, "selectedGovernorIndex"}
    };

    public:
        QHash<int, QByteArray> roleNames() const override {
            return rolesHash;
        }
        ControllerListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = NameRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        ProfileModel* profileModel() { return _profileModel; }
        void setProfileModel(ProfileModel* value);

        Q_INVOKABLE void setGovernor(int row, int governorIndex);

        /**
         * @brief gets the index of a governor by name
         * @return the index, or -1 if not found
         */
        Q_INVOKABLE int indexOfGovernor(QString governorName);

    signals:
        void profileChanged(ProfileModel* value);
};
