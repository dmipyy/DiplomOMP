#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#include "FileAppHolder.h"
#include <QObject>
#include <QProcess>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#include <QCoreApplication>

#include <QDBusInterface>
#include <QDBusReply>
#include <QFile>


class DBusService : public QObject
{
    Q_OBJECT

public: DBusService();

public slots:
    void launchApplication(const QString& filePath, FileAppHolder& fileAppHolder);
    void regApplication(const QString& filePath, FileAppHolder& fileAppHolder);
    void switchMethod(const QString& filePath, const QString& choosedMethod);
private:

};

QString getAppName(const QString& filePath, FileAppHolder& fileAppHolder);
void sendMessageToDBus(const QString& filePath);
void printMap(FileAppHolder& fileAppHolder);
#endif // DBUS_SERVICE_H
