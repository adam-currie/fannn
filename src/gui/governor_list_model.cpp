#include "governor_list_model.h"

GovernorListModel::GovernorListModel(QObject *parent) : QAbstractListModel(parent) {}

QVariant GovernorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(
                    _profileModel->constProfile()
                    .getGovernors()
                    .at(index.row())
                    .name
            );
        default:
            //todo
            return QVariant();
    }

}

Qt::ItemFlags GovernorListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int GovernorListModel::rowCount(const QModelIndex &parent) const {
    return _profileModel ?
            _profileModel->constProfile().getGovernors().size() :
            0;
}

void GovernorListModel::add() {
    std::string name;
    int i = 0;

tryNextName:
    name = "governor" + std::to_string(++i);
    for (auto const & g : _profileModel->constProfile().getGovernors())
        if (g.name == name)
            goto tryNextName;
    for (auto const & sa : _profileModel->constProfile().getSensorAliases())
        if (sa.alias == name)
            goto tryNextName;

    int preRowCount = rowCount();
    beginInsertRows(QModelIndex(), preRowCount, preRowCount);
    _profileModel->addOrUpdateGovernor(Fannn::Governor(name));
    endInsertRows();
}


 void GovernorListModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _profileModel->removeGovernor(row);
    endRemoveRows();
}
    }

QHash<int, QByteArray> GovernorListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
