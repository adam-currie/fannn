#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQuickStyle>
#include "sensors_model.h"
#include "profiles_model.h"

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

    ProfilesModel profilesModel;

    engine.setInitialProperties({
        { "builtInStyles", builtInStyles },
        { "profilesModel", QVariant::fromValue(&profilesModel) }
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
