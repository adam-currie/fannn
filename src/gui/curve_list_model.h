#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <vector>
#include "curve_model.h"
#include "profile_model.h"

class CurveModel;

class CurveListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(ProfileModel* profile READ profileModel WRITE setProfileModel NOTIFY profileChanged)
    ProfileModel* _profileModel = nullptr;

    enum Roles {
        CurveRole = Qt::UserRole + 1
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {CurveRole, "curve"}
    };

    std::vector<CurveModel*> curveModels;
    std::vector<QMetaObject::Connection> curveModelDestroyedSignalConnections;

    void addCurveHereNotInProfile(Fannn::Curve c);

    public:
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }
        CurveListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        ProfileModel* profileModel() { return _profileModel; }
        void setProfileModel(ProfileModel* value);

        bool checkNameInUse(std::string name);

        void pushChanges(CurveModel* curveModel);

        Q_INVOKABLE void add();

    signals:
        void profileChanged(ProfileModel* value);
};


