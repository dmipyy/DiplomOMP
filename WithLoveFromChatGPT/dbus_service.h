#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#endif // DBUS_SERVICE_H
#include <QObject>

class DBusService : public QObject
{
    Q_OBJECT

public: DBusService(int value);

public slots:
    void registerApplication(const QString& appName, const QStringList& capabilities, const QString& command);
    QStringList getApplicationsByCapability(const QString& capability);
    void launchApplication(const QString& appName, const QString& filePath);
};
