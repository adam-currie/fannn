#include "controller_list_model.h"
#include "plugins_composite_device_writer.h"

using Fannn::PluginsCompositeDeviceWriter;
using std::string;
using std::vector;

ControllerListModel::ControllerListModel(QObject *parent)
    : QAbstractListModel(parent) {}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole: {
            string id = PluginsCompositeDeviceWriter::instance().getAll()[index.row()];
            return QString::fromStdString(id);
        }
        case GovernorChoices: {
            QList<QString> choices = {"none"};

            if (_profileModel)
                for (const auto & gov : _profileModel->profile().getGovernors())
                    choices.append(QString::fromStdString(gov.name));

            return choices;
        }
        case SelectedGovernorIndex: {
            if (!_profileModel)
                return 0;

            string id = PluginsCompositeDeviceWriter::instance().getAll()[index.row()];
            string govName = "";
            for (const auto & c : _profileModel->profile().getControllers())
                if (c.id == id)
                    govName = c.governorName;

            const auto & govs = _profileModel->profile().getGovernors();

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
    return Fannn::PluginsCompositeDeviceWriter::instance().getAll().size();
}

void ControllerListModel::setGovernor(int row, int governorIndex){
    if (!_profileModel)
        return;

    string id = PluginsCompositeDeviceWriter::instance().getAll().at(row);

    if (governorIndex == 0) {
        _profileModel->removeController(id);
    } else {
        string govName = _profileModel->profile().getGovernors()[governorIndex-1].name;
        _profileModel->setGovernorForController(id, govName);
    }

    emit dataChanged(index(row,0), index(row,0), {SelectedGovernorIndex});
}

int ControllerListModel::indexOfGovernor(QString governorName) {
    if (!_profileModel)
        return -1;

    std::string nameStr = governorName.toStdString();
    const auto & govs = _profileModel->profile().getGovernors();
    for (int i=0; i<govs.size(); ++i) {
        if (govs[i].name == nameStr) {
            return i;
        }
    }
    return -1;
}

void ControllerListModel::scanForControllers() {
    //todo
}
