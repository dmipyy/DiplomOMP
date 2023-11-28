#include <QCoreApplication>
#include "dbus_service.h"
#include <QDBusConnection>
#include <QDebug>
#include <QDBusError>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);


    QDBusConnection connection = QDBusConnection::sessionBus();

    if(!connection.registerService("org.example.Service"))
    {
        qDebug() << "error: " << connection.lastError().message();
        exit(-1);
    }

    DBusService object(60);

    connection.registerObject("/test/objects", &object,QDBusConnection::ExportAllSlots);
    //QString appName = "evince";
    //QString appName = "eog";
    QString appName = "totem";
    // Аргументы
    QString filePath = "/home/artem/Downloads/eminem-nate-dogg-till-i-collapse_(muztub.com).mp3";

    object.launchApplication(appName, filePath);
    return app.exec();
}
