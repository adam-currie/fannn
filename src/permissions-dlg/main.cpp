#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QQuickStyle>
#include <QStringListModel>


int main(int argc, char *argv[]){
    QApplication::setApplicationName("Fannn");
    QApplication::setOrganizationName("Fannn");
    QApplication app(argc, argv);

    QSettings settings;
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    QStringList paths;
    for (int i = 1; i < argc; ++i)
        paths << argv[i];

    engine.setInitialProperties({
        { "pathsModel", QVariant::fromValue(paths) }
    });

    const QUrl url(u"qrc:/FannnPermissionsDlg/main.qml"_qs);
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
