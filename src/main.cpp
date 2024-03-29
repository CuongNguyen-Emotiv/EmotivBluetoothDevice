#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "HostDevice.h"
#include "Singleton.h"
#include "HeadsetController.h"
#include "ServiceController.h"
#include "CharacteristicController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<HeadsetController>("HeadsetController", 1, 0, "HeadsetController");
    qmlRegisterType<ServiceController>("ServiceController", 1, 0, "ServiceController");
    qmlRegisterType<CharacteristicController>("CharacteristicController", 1, 0, "CharacteristicController");

    engine.rootContext()->setContextProperty("HostDevice", Singleton<HostDevice>::instance());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    engine.rootObjects().first()->setProperty("width", 640);
    engine.rootObjects().first()->setProperty("height", 1136);
#endif

    return app.exec();
}
