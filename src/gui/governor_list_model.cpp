#include "governor_list_model.h"
#include <QPair>
#include "q_governor_error.h"

GovernorListModel::GovernorListModel(QObject *parent) : QAbstractListModel(parent) {}

void GovernorListModel::validateGovNameLookups(Fannn::Governor& gov) {
    gov.validateNameLookups(
        [] (std::string s) { return true; }, //debug: todo: real curves
        [this] (std::string s) {
            for (auto const & g : governors())
                if (g.name == s)
                    return true;
            return _profileModel->hasSensor(s);
        }
    );
}

void GovernorListModel::validateAllGovNameLookups() {
    const auto & govs = governors();
    for (int i = 0; i < govs.size(); ++i) {
        auto g = govs.at(i);
        validateGovNameLookups(g);
        _profileModel->updateGovernor(i, g);
    }
}

void GovernorListModel::setProfileModel(ProfileModel* value) {
    if (value == _profileModel)
        return;
    beginResetModel();

    _profileModel = value;

    if (value) {
        validateAllGovNameLookups();

        for (auto& c : profileConnections)
            disconnect(c);
        profileConnections.clear();

        profileConnections.push_back(connect(
            value, &ProfileModel::aliasesChanged,
            [this] () {
                validateAllGovNameLookups();
                emit dataChanged(index(0,0), index(rowCount()-1,0), {ErrorsRole, ErrorStrRole});
            }
        ));
        //todo: curves
    }

    endResetModel();
    emit profileChanged(value);
}

QVariant GovernorListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(governors().at(index.row()).name);
        case ExpressionRole:
            return QString::fromStdString(governors().at(index.row()).getExpressionStr());
        case ErrorsRole: {
            Fannn::Governor const & gov = governors().at(index.row());

            QVariantList errors;
            for (const auto & error : gov.getErrors()) {
                QVariant e;
                e.setValue(QGovernorError(error));
                errors.append(e);
            }

            return errors;
        }
        case ErrorStrRole: {
            Fannn::Governor const & gov = governors().at(index.row());
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
    return _profileModel ? governors().size() : 0;
}

void GovernorListModel::add() {
    std::string name;
    int i = 0;

tryNextName:
    name = "governor" + std::to_string(++i);
    for (auto const & g : governors())
        if (g.name == name)
            goto tryNextName;
    for (auto const & sa : _profileModel->constProfile().getSensorAliases())
        if (sa.alias == name)
            goto tryNextName;

    int preRowCount = rowCount();
    beginInsertRows(QModelIndex(), preRowCount, preRowCount);
    _profileModel->addGovernor(Fannn::Governor(name, ""));
    endInsertRows();
}


void GovernorListModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _profileModel->removeGovernor(row);
    endRemoveRows();
}

 ProfileModel::SensorAliasOrGovNameCollision GovernorListModel::rename(int row, QString newName) {
     Fannn::Governor gov = governors()[row];
     std::string nameStr = newName.toStdString();

     if (nameStr == gov.name)
         return ProfileModel::NoCollision;

     gov.name = nameStr;

     auto result = _profileModel->updateGovernor(row, gov);

     if (result == ProfileModel::NoCollision)
        emit dataChanged(index(row,0), index(row,0), {NameRole});

     return result;
 }

void GovernorListModel::setExpression(int row, QString exp) {
    Fannn::Governor gov = governors()[row];

    if (gov.getExpressionStr() == exp.toStdString())
        return;

    gov.setExpression(exp.toStdString());
    validateGovNameLookups(gov);

    _profileModel->updateGovernor(row, gov);
    emit dataChanged(index(row,0), index(row,0), {ErrorsRole, ErrorStrRole});
}

QHash<int, QByteArray> GovernorListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ExpressionRole] = "expression";
    roles[ErrorsRole] = "errors";
    roles[ErrorStrRole] = "errorStr";
    return roles;
}
