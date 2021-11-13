#include "controller_list_model.h"
#include "composite_device_writer.h"

using Fannn::CompositeDeviceWriter;
using std::string;
using std::vector;

ControllerListModel::ControllerListModel(QObject *parent)
    : QAbstractListModel(parent) {}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole: {
            string id = CompositeDeviceWriter::instance().getAll()[index.row()];
            return QString::fromStdString(id);
        }
        case GovernorChoices: {
            QList<QString> choices = {"none"};

            if (_profileModel)
                for (const auto & gov : _profileModel->constProfile().getGovernors())
                    choices.append(QString::fromStdString(gov.name));

            return choices;
        }
        case SelectedGovernorIndex: {
            if (!_profileModel)
                return 0;

            string id = CompositeDeviceWriter::instance().getAll()[index.row()];
            string govName =
                    _profileModel ?
                            _profileModel->constProfile().getGovernorForController(id)
                            : "";

            const auto & govs = _profileModel->constProfile().getGovernors();

            for (int i=0; i<govs.size(); ++i)
                if (govs[i].name == govName)
                    return i+1;

            return 0;
        }
        default:
            //todo
            return QVariant();
    }

}

void ControllerListModel::setProfileModel(ProfileModel* value) {
    if (value == _profileModel)
        return;
    beginResetModel();

    _profileModel = value;

    for (auto& c : profileConnections)
        disconnect(c);
    profileConnections.clear();

    if (value) {
        profileConnections.push_back(connect(
            value, &ProfileModel::governorsChanged, [this](){
                emit dataChanged(index(0,0), index(rowCount()-1,0), {GovernorChoices});
                emit dataChanged(index(0,0), index(rowCount()-1,0), {SelectedGovernorIndex});
            }
        ));
    }

    endResetModel();
    emit profileChanged(value);
}

Qt::ItemFlags ControllerListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int ControllerListModel::rowCount(const QModelIndex &parent) const {
    return Fannn::CompositeDeviceWriter::instance().getAll().size();
}

void ControllerListModel::setGovernor(int row, int governorIndex){
    if (!_profileModel)
        return;

    if (governorIndex == 0) {
        _profileModel->removeController(row);
    } else {
        string govName = _profileModel->constProfile().getGovernors()[governorIndex-1].name;
        _profileModel->setGovernorForController(row, govName);
    }

    emit dataChanged(index(row,0), index(row,0), {SelectedGovernorIndex});
}

int ControllerListModel::indexOfGovernor(QString governorName) {
    if (!_profileModel)
        return -1;

    std::string nameStr = governorName.toStdString();
    const auto & govs = _profileModel->constProfile().getGovernors();
    for (int i=0; i<govs.size(); ++i) {
        if (govs[i].name == nameStr) {
            return i;
        }
    }
    return -1;
}
