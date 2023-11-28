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

    // Путь к браузеру Firefox
    QString browserPath = "notepad-plus-plus";

    // Аргументы для запуска в режиме приватного окна
    QStringList browserArgs = {"/home/artem/Documents/Programming/Astra/hello_world.c"};

    // Запуск браузера Firefox
    QProcess::startDetached(browserPath, browserArgs);

}

QStringList DBusService::getApplicationsByCapability(const QString& capability)
{
    // Реализация метода запроса списка приложений по возможности
}

void DBusService::launchApplication(const QString& appName, const QString& filePath)
{
    // Аргументы для запуска приложения (путь до файла)
    QStringList appArgs = {filePath};
    // Запуск приложения
    QProcess::startDetached(appName, appArgs);

}
