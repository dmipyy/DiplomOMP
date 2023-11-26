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

    DBusService dbusService(123);
    QString appName = "MyApp";
    QStringList capabilities = {"capability1", "capability2"};
    QString command = "/path/to/myapp";

    dbusService.registerApplication(appName, capabilities, command); // Вызов метода регистрации приложения
    return app.exec();
}
