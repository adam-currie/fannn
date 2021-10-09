#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQuickStyle>
#include "sensor_list_model.h"
#include "profile_list_model.h"
#include "controller_list_model.h"
#include "governor_list_model.h"
#include "sensor_list_model.h"
#include "curve_list_model.h"

int main(int argc, char *argv[]){
    QGuiApplication::setApplicationName("Fannn");
    QGuiApplication::setOrganizationName("Fannn");
    QGuiApplication app(argc, argv);

    QSettings settings;
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    ProfileListModel profilesModel;
    CurveListModel curvesModel;
    GovernorListModel governorsModel;
    ControllerListModel controllersModel;

    engine.setInitialProperties({
        { "profilesModel", QVariant::fromValue(&profilesModel) },
        { "curvesModel", QVariant::fromValue(&curvesModel) },
        { "governorsModel", QVariant::fromValue(&governorsModel) },
        { "controllersModel", QVariant::fromValue(&controllersModel) }
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
