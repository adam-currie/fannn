#include "controller_list_model.h"
#include "composite_device_writer.h"

using Fannn::CompositeDeviceWriter;

ControllerListModel::ControllerListModel(QObject *parent)
    : QAbstractListModel(parent) {}

QVariant ControllerListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole: {
            auto name = CompositeDeviceWriter::instance().getAll()[index.row()];//todo: SLOW, DUMB
            return QString::fromStdString(name);
        }
        case GovernorNameRole:
            return QString::fromStdString("todo: gov name");
        default:
            //todo
            return QVariant();
    }

}

Qt::ItemFlags ControllerListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int ControllerListModel::rowCount(const QModelIndex &parent) const {
    return Fannn::CompositeDeviceWriter::instance().getAll().size();
}
