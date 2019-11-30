#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QQuickStyle>
#include "imageprovider.h"
#include "appmodel.h"
#include "appproxymodel.h"

//TODO: ImageProvider жалуется при использовании прокси, наверное, опять проблема в индексах
//TODO: При использовании сортировки прокси модели, неправильно отображаются иконки приложений, скорее всего, из-за того что берется индекс
//иконки из прокси, а не из основной модели, собс-но сделсть возможность вызвать mapToSource из QML

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QScopedPointer<ImageProvider> imgProvider(new ImageProvider);
    AppModel model(imgProvider.data());
    AppProxyModel proxy;
    proxy.setSourceModel(&model);

    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("provider", imgProvider.data());
    engine.addImageProvider("prov", imgProvider.data());
    engine.rootContext()->setContextProperty("AppModel", &proxy);
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject *appView = engine.rootObjects().first()->findChild<QObject*>("appView");
    if(!appView) qDebug() << "Can't find appView";
    QObject::connect(appView, SIGNAL(signalRunApp(QVariant)), &model, SIGNAL(sigRunApp(QVariant)));
    QObject::connect(appView, SIGNAL(signalGetAppInfo(QVariant)), &model, SIGNAL(signalAppInfo(QVariant)));

    QObject *searchLine = engine.rootObjects().first()->findChild<QObject*>("SearchFilter");
    if(!searchLine) qDebug() << "Can't find SearchFilter";
    QObject::connect(searchLine, SIGNAL(signalSearchText(QVariant)), &proxy, SLOT(slotSearchText(QVariant)));


    return app.exec();
}
