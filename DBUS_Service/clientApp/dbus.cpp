#include <dbus.h>

QString choosedMethod;
void sendMessageToDBus(const QString& filePath, const QString& choosedMethod)
{
    QString messageMethod = "switchMethod";

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createMethodCall(
        serviceName, // Имя целевого объекта
        servicePath, // Путь к объекту
        serviceInterface, // Интерфейс
        messageMethod // Метод
    );
    message << filePath << choosedMethod;

    QDBusMessage reply = bus.call(message);
    if (reply.type() == QDBusMessage::ReplyMessage) {
        qDebug() << "Сообщение успешно отправлено";
    } else {
        qDebug() << "Ошибка при отправке сообщения:" << reply.errorName() << reply.errorMessage();
    }

}
