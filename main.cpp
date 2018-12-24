#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "filecompare.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("io.brataas.filetools");
    QCoreApplication::setOrganizationDomain("io.brataas.filetools");

    qmlRegisterType<Folders>("io.brataas.filetools", 1, 0, "Folders");
    qmlRegisterType<FileList>("io.brataas.filetools", 1, 0, "FileList");
    qmlRegisterType<FileCompare>("io.brataas.filetools", 1, 0, "FileCompare");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
