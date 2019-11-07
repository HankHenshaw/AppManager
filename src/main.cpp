#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageprovider.h"
#include "appmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QScopedPointer<ImageProvider> imgProvider(new ImageProvider);
    AppModel model(imgProvider.data());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("provider", imgProvider.data());
    engine.addImageProvider("prov", imgProvider.data());
    engine.rootContext()->setContextProperty("AppModel", &model);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *obj = engine.rootObjects().first()->findChild<QObject*>("appView");
    QObject::connect(obj, SIGNAL(signalRunApp(QVariant)), &model, SIGNAL(sigRunApp(QVariant)));

    return app.exec();
}
