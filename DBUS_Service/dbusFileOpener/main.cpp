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

    DBusService object;

    connection.registerObject("/interfaces", &object,QDBusConnection::ExportAllSlots);

    // Аргументы
    //QString filePath = "/home/dmippy/Downloads/cpp.png";
    //QString appName = getAppName(filePath);
    //sendMessageToDBus(filePath);

    return app.exec();
}
