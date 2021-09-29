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
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings.value("style").toString());

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    const QString styleInSettings = settings.value("style").toString();
    if (styleInSettings.isEmpty())
        settings.setValue(QLatin1String("style"), QQuickStyle::name());

    QQmlApplicationEngine engine;

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
        QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal") };

    ProfileListModel profilesModel;
    SensorListModel sensorsModel;
    CurveListModel curvesModel;
    GovernorListModel governorsModel;
    ControllerListModel controllersModel;

    engine.setInitialProperties({
        { "builtInStyles", builtInStyles },
        { "profilesModel", QVariant::fromValue(&profilesModel) },
        { "sensorsModel", QVariant::fromValue(&sensorsModel) },
        { "curvesModel", QVariant::fromValue(&curvesModel) },
        { "governorsModel", QVariant::fromValue(&governorsModel) },
        { "controllersModel", QVariant::fromValue(&controllersModel) }
    });

    const QUrl url(u"qrc:/fannn/main.qml"_qs);
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
