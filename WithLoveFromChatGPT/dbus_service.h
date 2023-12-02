#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#endif // DBUS_SERVICE_H
#include <QObject>
#include <QProcess>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

#include <QCoreApplication>

#include <QDBusInterface>
#include <QDBusReply>



class DBusService : public QObject
{
    Q_OBJECT

public: DBusService();
    QMap<QString, QString> get()
    {
        return fileApplications;
    }

private:
    QMap<QString, QString> fileApplications;

public slots:
    void launchApplication(const QString& filePath);

};

QString getAppName(const QString& filePath);
void sendMessageToDBus(const QString& filePath);
