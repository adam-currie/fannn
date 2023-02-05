#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQuickStyle>
#include "user_level_logging_messages.h"


int main(int argc, char *argv[]){
    QApplication::setApplicationName("Fannn");
    QApplication::setOrganizationName("Fannn");
    QApplication app(argc, argv);

    UserLevelMessageHandling::init();

    QSettings settings;
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/Fannn/main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
