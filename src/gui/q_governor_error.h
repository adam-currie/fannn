#pragma once

#include <qqml.h>
#include <QVector>
#include <QPair>
#include "governor.h"

class QGovernorError {
    public:
        QVector<QPair<int, int>> ranges;
        QString errMsg;

        QGovernorError(Fannn::Governor::Error error) {
            errMsg = QString::fromStdString(error.errMsg);
            for (const auto & range : error.ranges)
                ranges.append(range);
        }

        QGovernorError() = default;
};

Q_DECLARE_METATYPE(QGovernorError)
