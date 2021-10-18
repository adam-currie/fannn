#pragma once

#include <QtQml>
#include <qqml.h>
#include <QQuickTextDocument>

class ErrorHighlighter : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QQuickTextDocument* document MEMBER _document NOTIFY documentChanged)
    QQuickTextDocument* _document;

    public:
        ErrorHighlighter(QObject *parent = nullptr);

        Q_INVOKABLE void formatErrors(QVariantList errors);

    signals:
        void documentChanged(QQuickTextDocument* value);
};


