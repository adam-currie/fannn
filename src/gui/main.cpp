#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQuickStyle>
#include "user_level_logging_messages.h"
#include "sensor_list_model.h"
#include "profile_list_model.h"
#include "controller_list_model.h"
#include "governor_list_model.h"
#include "sensor_list_model.h"
#include "curve_list_model.h"



int main(int argc, char *argv[]){
    QApplication::setApplicationName("Fannn");
    QApplication::setOrganizationName("Fannn");
    QApplication app(argc, argv);

    UserLevelMessageHandling::init();

    QSettings settings;
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    ProfileListModel profilesModel;

    engine.setInitialProperties({
        { "profilesModel", QVariant::fromValue(&profilesModel) }
    });

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
