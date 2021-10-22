#include "curve_list_model.h"
#include <string>

CurveListModel::CurveListModel(QObject *parent) : QAbstractListModel(parent) {
    //todo
}

QVariant CurveListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case NameRole:
            return QString::fromStdString(curves().at(index.row()).name);
        default:
            //todo
            return QVariant();
    }

}

int CurveListModel::rowCount(const QModelIndex &parent) const {
    return _profileModel ? curves().size() : 0;
}

Qt::ItemFlags CurveListModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

void CurveListModel::add() {
    std::string name;
    int i = 0;

tryNextName:
    name = "curve" + std::to_string(++i);
    for (auto const & c : curves())
        if (c.name == name)
            goto tryNextName;

    int preRowCount = rowCount();
    beginInsertRows(QModelIndex(), preRowCount, preRowCount);
    _profileModel->addCurve(Fannn::Curve(name));
    endInsertRows();
}

void CurveListModel::remove(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    _profileModel->removeCurve(row);
    endRemoveRows();
}

bool CurveListModel::rename(int row, QString newName) {
    return false;//todo
}
