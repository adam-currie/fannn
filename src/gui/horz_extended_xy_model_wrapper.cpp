#include "horz_extended_xy_model_wrapper.h"
#include <QPointF>
#include <limits>
#include "qtmetamacros.h"

using namespace std;

HorzExtendedXYModelWrapper::HorzExtendedXYModelWrapper(QObject* parent)
    : QAbstractListModel(parent) {}

void HorzExtendedXYModelWrapper::setModel(QAbstractListModel *value) {
    if (_model != value) {
        beginResetModel();

        _model = value;

        for (auto& c : modelConnections)
            disconnect(c);
        modelConnections.clear();

        if (_model) {
            modelConnections = {
                connect(_model, &QAbstractListModel::modelAboutToBeReset, this, &QAbstractListModel::modelAboutToBeReset),
                connect(_model, &QAbstractListModel::modelReset, this, &QAbstractListModel::modelReset),
                connect(_model, &QAbstractListModel::dataChanged,
                    [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles){
                        emit dataChanged(expandIndexLeft(topLeft), expandIndexRight(bottomRight), roles);
                    }
                ),
                connect(_model, &QAbstractListModel::rowsAboutToBeInserted,
                    [this](const QModelIndex& parent, int start, int end){
                        emit rowsAboutToBeInserted(parent, start+1, end+1, {});
                    }
                ),
                connect(_model, &QAbstractListModel::rowsAboutToBeRemoved,
                    [this](const QModelIndex& parent, int start, int end){
                        emit rowsAboutToBeRemoved(parent, start+1, end+1, {});
                    }
                ),
                connect(_model, &QAbstractListModel::rowsInserted,
                    [this](const QModelIndex& parent, int start, int end){
                        emit rowsInserted(parent, start+1, end+1, {});
                        if (start == 0)// first row will update too
                            emit dataChanged(this->index(0,0), this->index(0,1));
                        if (end == _model->rowCount()-1)//update end too
                            emit dataChanged(this->index(_model->rowCount()+1,0), this->index(_model->rowCount()+1,1));
                        }
                ),
                connect(_model, &QAbstractListModel::rowsRemoved, //todo: test
                    [this](const QModelIndex& parent, int start, int end){
                        emit rowsRemoved(parent, start+1, end+1, {});
                        if (start == 0)// first row will update too
                            emit dataChanged(this->index(0,0), this->index(0,1));
                        if (end == _model->rowCount()-1)//update end too
                            emit dataChanged(this->index(_model->rowCount()+1,0), this->index(_model->rowCount()+1,1));
                    }
                ),
                connect(_model, &QAbstractListModel::rowsAboutToBeMoved,
                    [this](const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow){
                        throw logic_error("todo: not implemented");
                    }
                ),
                connect(_model, &QAbstractListModel::rowsMoved, this,
                    [this](const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row){
                        throw logic_error("todo: not implemented");
                    }
                )
            };
        }

        endResetModel();
        emit modelChanged(value);
    }
}

void HorzExtendedXYModelWrapper::setMax(qreal max) {
    if (_max == max) return;
    _max = max;
    emit dataChanged(this->index(_model->rowCount()+1,0), this->index(_model->rowCount()+1,1));
}

void HorzExtendedXYModelWrapper::setMin(qreal min) {
    if (_min == min) return;
    _min = min;
    emit dataChanged(this->index(0,0), this->index(0,1));
}

QVariant HorzExtendedXYModelWrapper::data(const QModelIndex &index, int role) const {
    int row = index.row();

    bool first = row == 0;
    bool last = row == rowCount()-1;

    if (!first) --row;
    if (last) --row;

    QVariant var = _model->data(this->index(row, index.column()), role);

    bool gettingX = index.column() == 0;
    if (gettingX && (first || last)) {
        qreal x = var.value<qreal>();
        x = first ? _min :_max;
        var.setValue(x);
    }

    return var;
}

Qt::ItemFlags HorzExtendedXYModelWrapper::flags(const QModelIndex &index) const {
    //todo
    return Qt::NoItemFlags;
}

QModelIndex const HorzExtendedXYModelWrapper::expandIndexLeft(QModelIndex const & index) const {
    int row = index.row();
    bool first = row == 0;
    if (!first) ++row;
    return this->index(row, index.column());
}

QModelIndex const HorzExtendedXYModelWrapper::expandIndexRight(QModelIndex const & index) const {
    int row = index.row();
    bool last = row == (_model->rowCount() - 1);
    ++row;//normal shift
    if (last) ++row;//shift to outer virtual item
    return this->index(row, index.column());
}
