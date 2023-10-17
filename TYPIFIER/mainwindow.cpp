#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getExecutablePath(QString& processName)
{
    QProcess process;
    process.start("which", QStringList() << processName);
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput().trimmed();
    if (output.isEmpty()) {
        qWarning() << "Could not find executable path for" << processName;
        return QString();
    }
    process.start("readlink", QStringList() << "-f" << output);
    process.waitForFinished();
    QByteArray result = process.readAllStandardOutput().trimmed();
    if (result.isEmpty()) {
        qWarning() << "Could not resolve symbolic link for" << output;
        return QString();
    }
    return QString(result);
}

void MainWindow::executeApp(QString executePath,QString applicationName)
{

    QString nameApp, interfaceApp;
    //QDBusInterface interface("org.gnome.Nautilus", "/org/gnome/Nautilus", "org.gnome.Nautilus");
    //QDBusMessage message = interface.call("OpenFolder", "/home/user/Documents");
    //if (message.type() == QDBusMessage::ErrorMessage) {
    //    qWarning() << message.errorMessage();
   //}

    // Получаем соединение с сеансовой шиной D-Bus
     QDBusConnection connection = QDBusConnection::systemBus();
     if (!connection.isConnected()) {
         qWarning() << "Failed to connect to the D-Bus session bus.";
     }


     // Имя или путь к исполняемому файлу приложения

    QString applicationInterface = "org.freedesktop.DBus";

     // Создаем интерфейс для запуска приложения
     //QDBusInterface interface("org.freedesktop.DBus",
      //                        "/org/freedesktop/DBus",
      //                        "org.freedesktop.DBus",
     //                         connection, this
     //                         );
     // Запускаем приложение

     QString serviceArgs = "-config=config.ini";
     int startType = 0;
     int sessionID = 1;
     QString desktopName = "Desktop1";
     int error = 0;

     QProcess process;
     QStringList arguments;
     //arguments << applicationName.toStdU32String() << serviceArgs
     //          << QString::number(startType) << QString::number(sessionID)
     //          << desktopName << QString::number(error);

     QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "StartServiceByName");

     arguments << applicationName << serviceArgs;
     message << arguments;
     qDebug() << "1" ;
     //process.start("StartServiceByName", arguments);
     qDebug() << "2" ;
     //if(process.waitForFinished(-1)){
     //    qDebug() << "3" ;
     //}

     int exitCode = 0;
     exitCode = process.exitCode();
     qDebug() << exitCode;

    qDebug() << "4" ;

    //QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "StartServiceByName");


    //    message.setArguments(arguments);

        // Отправляем сообщение и ждем ответа
    QDBusMessage reply = connection.call(message);
    if (reply.type() == QDBusMessage::ReplyMessage) {
            qDebug() << "Метод успешно вызван";
        } else {
            qDebug() << "Ошибка при вызове метода:" << reply.errorName() << reply.errorMessage();
        }
    //QDBusMessage message = interface.call("StartServiceByName", applicationName,serviceArgs);
    //qDebug() << "5" ;
    //if (message.type() == QDBusMessage::ErrorMessage) {
    //    qWarning() << message.errorMessage();
    //}
}

void MainWindow::start(QString fileType) {
    QMap<QString, QString> fileApplications = {
            {"txt", "notepadqq"},
            {"jpg", "mspaint.exe"},
            {"png", "mspaint.exe"},
            {"gif", "mspaint.exe"},
            {"mp3", "winamp.exe"},
            {"wav", "windows media player.exe"},
            {"mp4", "vlc.exe"},
            {"avi", "vlc.exe"},
            {"mov", "quicktime.exe"}
        };

        qDebug() << "Введите тип файла: ";

        // Проверяем, есть ли такой тип файла в словаре
        if (fileApplications.count(fileType) > 0) {
            QString application = fileApplications[fileType];
            qDebug() << "Открываем файл с помощью приложения: " << application << '\n';
            QString executePath = getExecutablePath(application);
            qDebug() << "Path to executable:" << executePath;
            // Здесь можно добавить код для запуска приложения с указанным файлом!!!!!!!
            executeApp(executePath,application);
        } else {
            qDebug() << "Неизвестный тип файла" << '\n';
        }

}


void MainWindow::on_pushButton_clicked()
{
     QString str = ui->lineEdit->text();
    start(str);
}

