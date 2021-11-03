#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <QPointF>
#include "curve.h"
#include "curve_list_model.h"

class CurveListModel;//debug

class CurveModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(qreal minX READ minX WRITE setMinX NOTIFY minXChanged)
    Q_PROPERTY(qreal minY READ minY WRITE setMinY NOTIFY minYChanged)
    Q_PROPERTY(qreal maxX READ maxX WRITE setMaxX NOTIFY maxXChanged)
    Q_PROPERTY(qreal maxY READ maxY WRITE setMaxY NOTIFY maxYChanged)
    Q_PROPERTY(int movingPointIndex MEMBER _movingPointIndex NOTIFY movingPointIndexChanged)
    Q_PROPERTY(bool needsPush READ needsPush NOTIFY needsPushChanged)

    bool __needsPush;
    void setNeedsPush(bool needsPush) {
        if (needsPush != __needsPush) {
            __needsPush = needsPush;
            emit needsPushChanged(needsPush);
        }
    }

    enum Roles {
        PointRole = Qt::UserRole + 1
    };
    inline static QHash<int, QByteArray> const rolesHash = {
        {Qt::DisplayRole, "display"},
        {PointRole, "_point"}
    };

    Fannn::Curve scratchCurve,pushedCurve;
    CurveListModel* owner;

    int _movingPointIndex = -1;

    public:
        QHash<int, QByteArray> roleNames() const override { return rolesHash; }
        CurveModel(QObject *parent, CurveListModel* owner, Fannn::Curve const & curve);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override{
            return 2;
        }

        bool needsPush() const { return __needsPush; }

        QString name() const {
            return QString::fromStdString(scratchCurve.name);
        }
        double minX() const { return scratchCurve.getMinX(); }
        double minY() const { return scratchCurve.getMinY(); }
        double maxX() const { return scratchCurve.getMaxX(); }
        double maxY() const { return scratchCurve.getMaxY(); }

        void setMinX(double value);
        void setMinY(double value);
        void setMaxX(double value);
        void setMaxY(double value);

        Fannn::Curve const & getCurve() const { return scratchCurve; }

        /**
         * @brief renames a curve
         * @return true on success, including if no name change takes place, false for collisions
         */
        Q_INVOKABLE bool rename(QString newName);
        Q_INVOKABLE void remove() { delete this; };
        Q_INVOKABLE void pushChanges();
        Q_INVOKABLE void discardChanges();

        Q_INVOKABLE int addPoint(QPointF);
        Q_INVOKABLE void removePoint(int index);

        Q_INVOKABLE void beginMovePoint(int index);
        //moves point contrained by the bounds of the graph, but also by the x value of it's neighbors
        Q_INVOKABLE void movePoint(QPointF point);
        Q_INVOKABLE void endMovePoint();

    signals:
        void needsPushChanged(bool value);
        void movingPointIndexChanged(int value);
        void nameChanged(QString value);
        void minXChanged(double value);
        void minYChanged(double value);
        void maxXChanged(double value);
        void maxYChanged(double value);
};


