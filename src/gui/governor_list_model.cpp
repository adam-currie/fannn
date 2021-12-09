#include "governor_list_model.h"
#include <QPair>
#include "q_governor_error.h"

GovernorListModel::GovernorListModel(QObject *parent) : QAbstractListModel(parent) {}

void GovernorListModel::execAllAndPushToProfile() {
    for (int i = 0; i < _profileModel->profile().governorCount(); ++i) {
        auto & g = _profileModel->profile().governorAt(i);
        g.exec(context);
        _profileModel->updateGovernor(i, g);
    }
}

void GovernorListModel::setProfileModel(ProfileModel* value) {
    if (value == _profileModel)
        return;
    beginResetModel();

    _profileModel = value;
    context.profile = &value->profile();

    for (auto& c : profileConnections)
        disconnect(c);
    profileConnections.clear();

    if (value) {
        execAllAndPushToProfile();
        //todo: check how old code was updating when other govs changed, dont think we did it through profile connections
        auto updateAllAndSignal = [this] () {
            execAllAndPushToProfile();
            emit dataChanged(index(0,0), index(rowCount()-1,0), {ErrorsRole, ErrorStrRole});
        };

        profileConnections.push_back(connect(
            value, &ProfileModel::aliasesChanged, updateAllAndSignal
        ));
        profileConnections.push_back(connect(
            value, &ProfileModel::curvesChanged, updateAllAndSignal
        ));
    }

    endResetModel();
    emit profileChanged(value);
}

QVariant GovernorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(_profileModel->profile().governorAt(index.row()).name);
        case ExpressionRole:
            return QString::fromStdString(_profileModel->profile().governorAt(index.row()).getExpressionStr());
        case ErrorsRole: {
            Fannn::Governor const & gov = _profileModel->profile().governorAt(index.row());

            QVariantList errors;
            for (const auto & error : gov.getErrors()) {
                QVariant e;
                e.setValue(QGovernorError(error));
                errors.append(e);
            }

            return errors;
        }
        case ErrorStrRole: {
            Fannn::Governor const & gov = _profileModel->profile().governorAt(index.row());
            auto errors = gov.getErrors();
            int size = errors.size();

            if (size == 0)
                return QString();

            QString errStr;
            int i = 0;
            while (1) {
                errStr.append(QGovernorError(errors[i]).errMsg);
                if (++i >= size)
                    break;
                errStr.append(", ");
            }

            return errStr;
        }
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
    return _profileModel ? _profileModel->profile().governorCount() : 0;
}

void GovernorListModel::add() {
    std::string name;
    int i = 0;

tryNextName:
    name = "governor" + std::to_string(++i);
    for (auto const & g : _profileModel->profile().getGovernors())
        if (g.name == name)
            goto tryNextName;
    for (auto const & sa : _profileModel->profile().getSensorAliases())
        if (sa.alias == name)
            goto tryNextName;

    int preRowCount = rowCount();
    beginInsertRows(QModelIndex(), preRowCount, preRowCount);
    _profileModel->addGovernor(Fannn::Governor(name, ""));
    endInsertRows();

    execAllAndPushToProfile();
    emit dataChanged(index(0,0), index(rowCount()-1,0), {ErrorsRole, ErrorStrRole});
}


void GovernorListModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _profileModel->removeGovernor(row);
    endRemoveRows();
}

ProfileModel::SensorAliasOrGovNameCollision GovernorListModel::rename(int row, QString newName) {
    Fannn::Governor gov = _profileModel->profile().governorAt(row);
    std::string nameStr = newName.toStdString();

    if (nameStr == gov.name)
        return ProfileModel::NoCollision;

    gov.name = nameStr;

    auto result = _profileModel->updateGovernor(row, gov);

    if (result == ProfileModel::NoCollision) {
        execAllAndPushToProfile();
        emit dataChanged(index(0,0), index(rowCount()-1,0), {ErrorsRole, ErrorStrRole});
        emit dataChanged(index(row,0), index(row,0), {NameRole});
    }

    //todo: rn this isnt needed because but it's better to be safe, remove in some future build for perf?
    emit dataChanged(index(row,0), index(row,0), {NameRole});

    return result;
}

void GovernorListModel::setExpression(int row, QString exp) {
    Fannn::Governor const & gov = _profileModel->profile().governorAt(row);

    if (gov.getExpressionStr() == exp.toStdString())
        return;

    _profileModel->updateGovernor(
            row,
            Fannn::Governor(gov.name, exp.toStdString())
    );

    //todo: rn this isnt needed but it's better to be safe, remove in some future build for perf?
    emit dataChanged(index(row,0), index(row,0), {ExpressionRole});

    //need to update re-execute all because they might reference this governor
    execAllAndPushToProfile();
    emit dataChanged(index(0,0), index(rowCount()-1,0), {ErrorsRole, ErrorStrRole});
}
