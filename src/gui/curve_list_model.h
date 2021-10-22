#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "profile_model.h"

class CurveListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile READ profileModel WRITE setProfileModel NOTIFY profileChanged)
    ProfileModel* _profileModel;

    enum Roles {
        NameRole = Qt::UserRole + 1
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {NameRole, "name"}
    };

    inline std::vector<Fannn::Curve> const & curves() const {
        return _profileModel->constProfile().getCurves();
    }

    public:
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }
        CurveListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        ProfileModel* profileModel() { return _profileModel; }
        void setProfileModel(ProfileModel* value) { _profileModel = value; }

        Q_INVOKABLE void add();
        Q_INVOKABLE void remove(int row);

        /**
         * @brief renames a curve
         * @return true if name changed, false if name already taken
         */
        Q_INVOKABLE bool rename(int row, QString newName);

    signals:
        void profileChanged(ProfileModel* value);
};


