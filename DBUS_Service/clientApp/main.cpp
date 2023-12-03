#include "mainwindow.h"

#include <QApplication>
#include <QFileInfo>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    // Получаем путь к файлу, который мы хотим найти
//    QString fileName = "dbusFileOpener.exe";

//    // Получаем информацию о файле
//    QFileInfo fileInfo(fileName);

//    // Получаем полный путь к директории, содержащей файл
//    QString exePath = fileInfo.absoluteFilePath();

//    // Выводим результат
//    qDebug() << "Путь к файлу:" << fileName;
//    qDebug() << "Путь к директории с файлом:" << exePath;
//    QStringList arguments;

//    QProcess *myProcess = new QProcess();
//    // Устанавливаем программу и аргументы
//    myProcess->start(exePath, arguments);

//    // Обработка завершения процесса
//    QObject::connect(myProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//                     [=](int exitCode, QProcess::ExitStatus exitStatus){
//        qDebug() << "Процесс завершен с кодом:" << exitCode;
//    });

    return a.exec();
}
