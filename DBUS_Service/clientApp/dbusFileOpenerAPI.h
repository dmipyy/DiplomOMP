#ifndef DBUSFILEOPENERAPI_H
#define DBUSFILEOPENERAPI_H
#include <QString>

const QString serviceName = "org.example.Service"; // Имя целевого объекта
const QString servicePath = "/interfaces"; // Путь к объекту
const QString serviceInterface = "local.dbusFileOpener.DBusService"; // Интерфейс
const QString serviceMethod = "launchApplication"; // Метод

#endif // DBUSFILEOPENERAPI_H
