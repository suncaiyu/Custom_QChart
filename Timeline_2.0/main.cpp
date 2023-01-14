#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "TimelineWidget.h"
#include "MyChart.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<TimelineWidget>("Furrain.Control", 1, 0, "Timeline");
    qmlRegisterType<MyChart>("Furrain.Control", 1, 0, "MyChart");
    qRegisterMetaType<TimelineContext*>("TimelineContext*");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
