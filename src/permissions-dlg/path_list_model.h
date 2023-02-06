#pragma once

#include <QAbstractListModel>
#include <qqml.h>

class PathListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    public:
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};
