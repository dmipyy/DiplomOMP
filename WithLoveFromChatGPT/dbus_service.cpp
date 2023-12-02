#include "dbus_service.h"

DBusService::DBusService()
{
    fileApplications =
    {
        {"txt", "gedit"},
        {"c", "gedit"},
        {"jpg", "eog"},
        {"png", "eog"},
        {"gif", "eog"},
        {"mp3", "totem"},
        {"wav", "totem"},
        {"mp4", "vlc.exe"},
        {"avi", "vlc.exe"},
        {"mov", "quicktime.exe"}
    };
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
    QMap<QString, QString> fileApplications = temp.get();
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
    QString appName = getAppName(filePath);
    // Аргументы для запуска приложения (путь до файла)
    QStringList appArgs = {filePath};
    // Запуск приложения
    QProcess::startDetached(appName, appArgs);

}

void sendMessageToDBus(const QString& filePath)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.example.Service", // Имя целевого объекта
        "/interfaces", // Путь к объекту
        "local.WithLoveFromChatGPT.DBusService", // Интерфейс
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













