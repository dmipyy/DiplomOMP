#ifndef DBUS_H
#define DBUS_H
#include <QString>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#include <QCoreApplication>

#include <QDBusInterface>
#include <QDBusReply>

#include <dbusFileOpenerAPI.h>

void sendMessageToDBus(const QString& filePath);


#endif // DBUS_H
