#pragma once

#include <QAbstractItemModel>
#include <qqml.h>
#include "profile_persister.h"

class ProfileModel : public QAbstractItemModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int updateIntervalMs READ updateIntervalMs WRITE setUpdateIntervalMs NOTIFY updateIntervalMsChanged)
    Q_PROPERTY(bool unsavedChanges READ unsavedChanges NOTIFY unsavedChangesChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool hasIssues READ hasIssues)

    Fannn::ProfilePersister persister;
    bool __unsavedChanges;
    void setUnsavedChanges(bool value) {
        if (value != __unsavedChanges) {
            __unsavedChanges = value;
            emit unsavedChangesChanged(value);
        }
    }

    public:
        ProfileModel(QObject *parent = nullptr, Fannn::ProfilePersister = Fannn::ProfilePersister(""));
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{
            return QModelIndex();
        }
        Qt::ItemFlags flags(const QModelIndex &index) const override {
            return Qt::NoItemFlags;
        }
        int rowCount(const QModelIndex &parent) const override {
            return 0;
        }
        int columnCount(const QModelIndex &parent) const override {
            return 0;
        }
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override{
            return QModelIndex();
        }
        QModelIndex parent(const QModelIndex &index) const override {
            return QModelIndex();
        }

        Fannn::Profile const & constProfile() const { return persister.constProfile(); }

        int updateIntervalMs() const;
        void setUpdateIntervalMs(int);
        bool unsavedChanges() const { return __unsavedChanges; }
        QString name();
        bool hasSensor(std::string idOrAlias) const;
        bool hasIssues();

        enum SensorAliasOrGovNameCollision {
            NoCollision,
            CollidesWithSensorAlias,
            CollidesWithGovernor
        };
        Q_ENUM(SensorAliasOrGovNameCollision)

        SensorAliasOrGovNameCollision setSensorAlias(int index, QString alias);
        std::string removeSensorAlias(int index);

        Q_INVOKABLE SensorAliasOrGovNameCollision updateGovernor(int index, Fannn::Governor gov);
        Q_INVOKABLE void addGovernor(Fannn::Governor gov);
        Q_INVOKABLE void removeGovernor(int index);

    signals:
        void updateIntervalMsChanged(int);
        void unsavedChangesChanged(bool);
        void nameChanged(QString);
        void aliasesChanged();
        void governorsChanged();

    public slots:
        void save();
};

