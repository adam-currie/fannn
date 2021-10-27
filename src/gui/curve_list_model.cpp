#include "curve_list_model.h"
#include <assert.h>
#include <exception>
#include <string>

CurveListModel::CurveListModel(QObject *parent) : QAbstractListModel(parent) {}

void CurveListModel::addCurveHereNotInProfile(Fannn::Curve c) {
    CurveModel* curveModel = new CurveModel(this, this, c);
    curveModelDestroyedSignalConnections.push_back(connect(curveModel, &CurveModel::destroyed, this, [this, curveModel](){
        for (int i=0;;++i){
            assert(i < curveModels.size());
            if (curveModels[i] == curveModel) {
                beginRemoveRows(QModelIndex(), i, i);
                curveModels.erase(curveModels.begin()+i);
                _profileModel->removeCurve(i);
                endRemoveRows();
                break;
            }
        }
    }));
    curveModels.push_back(curveModel);
}

bool CurveListModel::checkNameInUse(std::string name) {
    for (auto c : _profileModel->constProfile().getCurves())
        if (c.name == name)
            return true;
    return false;
}

void CurveListModel::pushChanges(CurveModel *curveModel) {
    int i = 0;
    while (i < curveModels.size() && curveModels[i] != curveModel) ++i;
    if (i == curveModels.size()) throw std::invalid_argument("curve not in list");
    _profileModel->updateCurve(i, curveModels[i]->getCurve());
}

void CurveListModel::setProfileModel(ProfileModel* value) {
    if (value == _profileModel)
        return;

    beginResetModel();

    _profileModel = value;

    for (auto c : curveModelDestroyedSignalConnections)
        disconnect(c);
    curveModelDestroyedSignalConnections.clear();
    for (auto c : curveModels)
        delete c;
    curveModels.clear();

    if (value) {
        for (auto c : _profileModel->constProfile().getCurves())
            addCurveHereNotInProfile(c);
    }

    endResetModel();
    emit profileChanged(value);
}

QVariant CurveListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case CurveRole:  {
            return QVariant::fromValue<QObject*>(curveModels[index.row()]);
        }
        default:
            //todo
            return QVariant();
    }

}

int CurveListModel::rowCount(const QModelIndex &parent) const {
    return curveModels.size();
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
    if (checkNameInUse(name))
        goto tryNextName;

    int preRowCount = rowCount();
    beginInsertRows(QModelIndex(), preRowCount, preRowCount);

    Fannn::Curve c(name);
    _profileModel->addCurve(c);
    addCurveHereNotInProfile(c);

    endInsertRows();
}
