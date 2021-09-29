#pragma once

#include <QAbstractListModel>
#include <qqml.h>

class ProfilesModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    public:
        enum Roles {
            NameRole = Qt::UserRole + 1
            //todo:more roles will start after userrole + 1
        };
        ProfilesModel(QObject *parent = nullptr);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent) const override;
        QHash<int, QByteArray> roleNames() const override;
};
