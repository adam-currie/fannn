#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include "curve.h"
#include "curve_list_model.h"

class CurveListModel;//debug

class CurveModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    enum Roles {
        PointRole = Qt::UserRole + 1
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {PointRole, "point"}
    };

    Fannn::Curve curve;
    CurveListModel* owner;
    std::string realName;

    public:
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }
        CurveModel(QObject *parent, CurveListModel* owner, Fannn::Curve curve);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        QString name() const { return QString::fromStdString(curve.name); }

        Fannn::Curve const & getCurve() const { return curve; }

        /**
         * @brief renames a curve
         * @return true on success, including if no name change takes place, false for collisions
         */
        Q_INVOKABLE bool rename(QString newName);
        Q_INVOKABLE void remove() { delete this; };
        Q_INVOKABLE void pushChanges();

    signals:
        void nameChanged(QString value);
};


