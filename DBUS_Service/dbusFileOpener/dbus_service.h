#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H


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
    const QMap<QString, QString>& getMap()
    {
        return fileApplications;
    }

    void setMap(QString key, QString elem)
    {
        if (!fileApplications.contains(key))
        {
            fileApplications.insert(key, elem);
        }
        else
        {
            qDebug() << "Элемент с расширением " << key << " уже зарегистрирован";
        }
    }

private:
    QMap<QString, QString> fileApplications =
    {
        {"txt", "gedit"},
        //{"c", "gedit"},
        //{"jpg", "eog"},
        {"png", "eog"},
        //{"gif", "eog"},
        //{"mp3", "totem"},
        //{"wav", "totem"},
    };
public slots:
    void launchApplication(const QString& filePath);
    void regApplication(const QString& filePath);
};

QString getAppName(const QString& filePath);
void sendMessageToDBus(const QString& filePath);
void abc();
#endif // DBUS_SERVICE_H
