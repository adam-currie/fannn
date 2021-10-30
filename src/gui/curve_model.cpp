#include "curve_model.h"
#include <string>
#include <exception>

using namespace std;
using namespace Fannn;

CurveModel::CurveModel(QObject* parent, CurveListModel* owner, Curve const & curve)
    : QAbstractListModel(parent), owner(owner) {

    scratchCurve = curve;
    //todo: enforce minimum distance between points and stuff
    pushedCurve = scratchCurve;
}

QVariant CurveModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    auto p = scratchCurve.getPoints()[index.row()];

    switch (role) {
        case PointRole:  {
            assert(index.column() == 0);
            return QPointF(p.x, p.y);
        }
        case Qt::DisplayRole:  {
            assert(index.column() < 2);
            return (index.column() == 0) ? p.x : p.y;
        }
        default:
            assert(false);
            return QVariant();
    }
}

Qt::ItemFlags CurveModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int CurveModel::rowCount(const QModelIndex &parent) const {
    return scratchCurve.getPoints().size();
}

void CurveModel::pushChanges() {
    owner->pushChanges(this);
    pushedCurve = scratchCurve;
}

void CurveModel::beginMovePoint(int row) {
    if (row < 0 || row > rowCount())
        throw invalid_argument("invalid point index");
    if (_movingPointIndex != row)
        endMovePoint();

    _movingPointIndex = row;
}

void CurveModel::movePoint(QPointF p) {
    if (_movingPointIndex == -1)
        throw logic_error("no move operation started, you need to call beginMovePoint first");
    scratchCurve.updatePoint(_movingPointIndex, {p.x(), p.y()});
    emit dataChanged(index(_movingPointIndex,0), index(_movingPointIndex,1), {PointRole});
}

int CurveModel::addPoint(QPointF p) {
    //todo: doing this a slow way cause i don't know if it's okay to update the model before calling
    //      beginInsertRows, maybe we can not worry about that, or implement a more efficient way
    auto tempCurve = scratchCurve;
    int newIndex = tempCurve.addPoint({p.x(), p.y()});
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    scratchCurve = tempCurve;
    endInsertRows();
    return newIndex;
}

void CurveModel::endMovePoint(){
    if (_movingPointIndex == -1) return;

    auto points = scratchCurve.getPoints();

    int firstOnThisX = _movingPointIndex;
    int lastOnThisX = _movingPointIndex;
    while (firstOnThisX > 0 && points[firstOnThisX-1].x == points[_movingPointIndex].x)
        --firstOnThisX;
    while (lastOnThisX < points.size()-1 && points[lastOnThisX+1].x == points[_movingPointIndex].x)
        ++lastOnThisX;

    int otherPointsOnThisX = lastOnThisX - firstOnThisX;

    //only want 2 points on a vertical line
    if (otherPointsOnThisX > 1) {
        beginRemoveRows(QModelIndex(), firstOnThisX+1, lastOnThisX-1);
        scratchCurve.removePoints(firstOnThisX+1, lastOnThisX-1);
        endRemoveRows();
    }

    _movingPointIndex = -1;
}

bool CurveModel::rename(QString newName) {
    std::string s = newName.toStdString();

    if (s == pushedCurve.name || !owner->checkNameInUse(s)) {
        scratchCurve.name = s;
        return true;
    } else {
        return false;
    }
}



