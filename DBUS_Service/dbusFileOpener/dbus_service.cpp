#include "FileAppHolder.h"
#include "dbus_service.h"
#include <algorithm>

DBusService::DBusService()
{

}

FileAppHolder::FileAppHolder()
{

}

void DBusService::switchMethod(const QString& filePath, const QString& choosedMethod)
{
    static FileAppHolder fileAppHolder;
    if(choosedMethod == "open") launchApplication(filePath, fileAppHolder);
    else if(choosedMethod == "reg") regApplication(filePath, fileAppHolder);
    else qDebug() << "Ошибка, ничего не передалось!";
}

QString getAppName(const QString& filePath, FileAppHolder& fileAppHolder)
{
    QString fileExtension;
    for(int i = filePath.length() - 1; i >= 0; --i) {
        if(filePath[i] == '.')
            break;

        fileExtension +=  filePath[i]; //QString stream почитать
    }
    std::reverse(fileExtension.begin(),fileExtension.end());

    QString application;
    // Проверяем, есть ли такой тип файла в словаре
    if (fileAppHolder.getFileApp().count(fileExtension) > 0) {
        application = fileAppHolder.getFileApp()[fileExtension];
        qDebug() << "Открываем файл с помощью приложения: " << application << '\n';
    }
    else
        qDebug() << "Неизвестный тип файла" << '\n';

    return application;
}

void DBusService::launchApplication(const QString& filePath, FileAppHolder& fileAppHolder)
{
    QString extension = getAppName(filePath, fileAppHolder);
    // Аргументы для запуска приложения (путь до файла)
    QStringList appArgs {filePath};
    // Запуск приложения
    QProcess::startDetached(extension, appArgs);

}

void DBusService::regApplication(const QString& filePath, FileAppHolder& fileAppHolder)
{
    //QString appName {getAppName(filePath, fileAppHolder)};
    QFile file(filePath);
    if(!file.exists()) {
        qDebug() << "Файл не существует, читать нечего!" << '\n';
        return;
    }
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Файл не открылся" << '\n';
        return;
    }

    QString key, elem;
    while(!file.atEnd()) {
        key = file.readLine();
        key.chop(1);
        elem = file.readLine();
        elem.chop(1);
        fileAppHolder.addElemToFileApp(key, elem);
        //qDebug() << "addElem";
    }
    //printMap(fileAppHolder);
    qDebug() << "Расширения и приложения зарегистрированы!" << '\n';
}

void printMap(FileAppHolder& fileAppHolder)
{
    const QMap<QString, QString>& fileApplications {fileAppHolder.getFileApp()};
    for (auto i = fileApplications.begin(); i != fileApplications.end(); ++i)
        qDebug() << "Key: " << i.key() << ", Value: " << i.value();
}











