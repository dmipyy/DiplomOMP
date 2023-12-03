#include <dbus.h>

void sendMessageToDBus(const QString& filePath)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createMethodCall(
        serviceName, // Имя целевого объекта
        servicePath, // Путь к объекту
        serviceInterface, // Интерфейс
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
