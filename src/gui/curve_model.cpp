#include "curve_model.h"
#include <QPointF>
#include <string>

using namespace std;
using namespace Fannn;

CurveModel::CurveModel(QObject* parent, CurveListModel* owner, Curve curve)
    : QAbstractListModel(parent), curve(curve), owner(owner), realName(curve.name) {}

QVariant CurveModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {
        case PointRole:  {
            auto p = curve.getPoints()[index.row()];
            return QPointF(p.x, p.y);
        }
        default:
            //todo
            return QVariant();
    }
}

Qt::ItemFlags CurveModel::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

int CurveModel::rowCount(const QModelIndex &parent) const {
    return curve.getPoints().size();
}

void CurveModel::pushChanges() {
    owner->pushChanges(this);
}

bool CurveModel::rename(QString newName) {
    std::string s = newName.toStdString();

    if (s == realName || !owner->checkNameInUse(s)) {
        curve.name = s;
        return true;
    } else {
        return false;
    }
}



