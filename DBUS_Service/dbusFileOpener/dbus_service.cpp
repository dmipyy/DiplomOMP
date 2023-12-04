#include "dbus_service.h"

DBusService::DBusService()
{

}

QString getAppName(const QString& filePath)
{
    QString fileExtension;
    for(int i = filePath.length() - 1; i >= 0; i--)
    {
        if(filePath[i] == '.')
        {

            break;
        }
        fileExtension +=  filePath[i];
    }
    QString reversedStr;
    for (int i = fileExtension.length() - 1; i >= 0; i--) {
        reversedStr += fileExtension[i];
    }
    fileExtension = reversedStr;

    QString application;
    // Проверяем, есть ли такой тип файла в словаре
    DBusService temp;
    QMap<QString, QString> fileApplications = temp.getMap();
    if (fileApplications.count(fileExtension) > 0)
    {
        application = fileApplications[fileExtension];
        qDebug() << "Открываем файл с помощью приложения: " << application << '\n';
    }
    else
    {
        qDebug() << "Неизвестный тип файла" << '\n';
    }

    return application;
}

void DBusService::launchApplication(const QString& filePath)
{
    if(filePath == "/home/dmippy/Documents/abc.txt")
    {
        regApplication(filePath);
    }
    else
    {
        QString appName = getAppName(filePath);
        // Аргументы для запуска приложения (путь до файла)
        QStringList appArgs = {filePath};
        // Запуск приложения
        QProcess::startDetached(appName, appArgs);
    }
}

void DBusService::regApplication(const QString& filePath)
{
    DBusService temp;
    QMap<QString, QString> fileApplications;

    QString appName = getAppName(filePath);
    QFile file(filePath);
    QString key, elem;
    while(!file.atEnd())
    {
        key = file.readLine();
        elem = file.readLine();
        temp.setMap(key, elem);
    }
    abc();

}

void sendMessageToDBus(const QString& filePath)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.example.Service", // Имя целевого объекта
        "/interfaces", // Путь к объекту
        "local.dbusFileOpener.DBusService", // Интерфейс
        "launchApplication" // Метод
    );
    message << filePath;

    QDBusMessage reply = bus.call(message);
    if (reply.type() == QDBusMessage::ReplyMessage) {
        qDebug() << "Сообщение успешно отправлено";
    } else {
        qDebug() << "Ошибка при отправке сообщения:" << reply.errorName() << reply.errorMessage();
    }

}

void abc()
{
    DBusService a;
    QMap<QString, QString> fileApplications = a.getMap();
    for (auto i = fileApplications.begin(); i != fileApplications.end(); ++i)
    {
        qDebug() << "Key: " << i.key() << ", Value: " << i.value();
    }
}











