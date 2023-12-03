#include <mainwindow.h>
#include <dbus.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    filePathEdit = new QLineEdit(this);
    layout->addWidget(filePathEdit);

    readButton = new QPushButton("Открыть файл", this);
    layout->addWidget(readButton);

    connect(readButton, &QPushButton::clicked, this, &MainWindow::readFilePath);
}

void MainWindow::readFilePath() {
    QString filePath = filePathEdit->text();
    qDebug() << "File Path: " << filePath;
    sendMessageToDBus(filePath);
}
