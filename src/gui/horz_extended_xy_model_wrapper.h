#pragma once

#include <QAbstractListModel>
#include <qqml.h>

class HorzExtendedXYModelWrapper : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    inline static QHash<int, QByteArray> const rolesHash = {
        {Qt::DisplayRole, "display"}
    };

    Q_PROPERTY(QAbstractListModel* model READ model WRITE setModel NOTIFY modelChanged REQUIRED)
    QAbstractListModel* _model = nullptr;

    //if you set this too small it won't work with chartview
    Q_PROPERTY(qreal max READ max WRITE setMax)
    qreal _max = 10000;
    //if you set this too small it won't work with chartview
    Q_PROPERTY(qreal min READ min WRITE setMin)
    qreal _min = -10000;

    std::vector<QMetaObject::Connection> modelConnections;

    /**
     * @brief convert from inner model index to outer model index
     *        if this is the first index of inner model,
     *          expand to be the first (virtual) index of the outer model
     *        otherwise
     *          shift the index appropriately
     * @param index inner model index
     * @return the converted index
     */
    QModelIndex const expandIndexLeft(QModelIndex const & index) const;

    /**
     * @brief convert from inner model index to outer model index
     *        if this is the last index of inner model,
     *          expand to be the last (virtual) index of the outer model
     *        otherwise
     *          shift the index appropriately
     * @param index inner model index
     * @return the converted index
     */
    QModelIndex const expandIndexRight(QModelIndex const & index) const;

    public:
        QHash<int, QByteArray> roleNames() const override {
            return rolesHash;
        }
        HorzExtendedXYModelWrapper(QObject* parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override {
            return _model?
                    _model->rowCount(parent) + 2 :
                    0;
        }
        int columnCount(const QModelIndex &parent = QModelIndex()) const override {
            return 2;
        }

        QAbstractListModel* model() { return _model; }
        void setModel(QAbstractListModel* value);

        qreal max() { return _max; }
        void setMax(qreal max);

        qreal min() { return _min; }
        void setMin(qreal min);

    signals:
        void modelChanged(QAbstractListModel* value);

};
