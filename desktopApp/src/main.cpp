#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include "camerathread.h"
#include "whiteboardimageprovider.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "BoardStream_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;

    CameraThread thread;
    WhiteboardImageProvider *provider = new WhiteboardImageProvider();

    engine.rootContext()->setContextProperty("VideoStreamer", &thread);
    engine.rootContext()->setContextProperty("liveImageProvider", provider);

    engine.addImageProvider("stream", provider);


    const QUrl url(u"qrc:/BoardStream/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject::connect(&thread, &CameraThread::newImage, provider, &WhiteboardImageProvider::update);
    thread.startCapture(2);

    return app.exec();
}
