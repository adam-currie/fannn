#pragma once

#include <QAbstractListModel>

#include <qqml.h>

class ControllerListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    public:
        ControllerListModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent) const override;
};

