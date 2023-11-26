#include "dbus_service.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusReply>

DBusService::DBusService(int value){
    int m_value = value;
}

void DBusService::registerApplication(const QString& appName, const QStringList& capabilities, const QString& command)
{
    QDBusConnection connection = QDBusConnection::sessionBus();

    if(!connection.registerService("my.App.Service"))
    {
        qDebug() << "error: " << connection.lastError().message();
        exit(-1);
    }

    DBusService object(60);

    connection.registerObject("NULL", &object,QDBusConnection::ExportAllSlots);
}

QStringList DBusService::getApplicationsByCapability(const QString& capability)
{
    // Реализация метода запроса списка приложений по возможности
}

void DBusService::launchApplication(const QString& appName, const QString& filePath)
{
    // Реализация метода запуска приложения
}
