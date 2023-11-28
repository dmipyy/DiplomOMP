#include <QCoreApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Путь к браузеру Firefox
    QString browserPath = "firefox";

    // Аргументы для запуска в режиме приватного окна
    QStringList browserArgs = {"--private-window", "https://example.com"};

    // Запуск браузера Firefox
    QProcess::startDetached(browserPath, browserArgs);

    return a.exec();
}

