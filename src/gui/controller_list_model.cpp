#include "controller_list_model.h"

#include <QtCore>
#include "plugins_composite_device_writer.h"
#include "user_level_logging_messages.h"

using Fannn::PluginsCompositeDeviceWriter;
using std::string;
using std::vector;

ControllerListModel::ControllerListModel(QObject *parent) : QAbstractListModel(parent) {
    scanForControllers();
}

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
    auto& writer = PluginsCompositeDeviceWriter::instance();

    //todo: reuse logic also for sensors?

    writer.rescan();

    std::vector<Fannn::Plugins::PluginLoadError> errs = writer.getPluginLoadErrors();
    std::string errBody = {};
    int userErrorCount = 0;
    for (auto const & err : errs) {
        if (err.likelyUserError) {
            ++userErrorCount;
            errBody += "<br>" + err.path + ": " + err.msg;
        }
    }
    if (userErrorCount > 0) {
        /*
         * do this on the next gui loop iteration so that when this is called
         * as part of the initialization of a window,
         * the resulting message box ends up on top
         */
        QTimer::singleShot(0, [=]()-> void {
            const char * const errHead = (userErrorCount == 1) ?
                    "error loading plugin:" : "error loading plugins:";
            qCWarning(UserLevelMessageHandling::PLUGINS) << errHead << errBody.c_str();
        });
    }
}
