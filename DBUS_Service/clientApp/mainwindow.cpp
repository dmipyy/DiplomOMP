#include <mainwindow.h>
#include <dbus.h>
#include <QDebug>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    readButton = new QPushButton("Открыть файл", this);
    layout->addWidget(readButton);
    launchButton = new QPushButton("Загрузить приложения", this);
    layout->addWidget(launchButton);

    connect(readButton, &QPushButton::clicked, this, &MainWindow::on_readButton_clicked);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::on_launchButton_clicked);
}

void MainWindow::on_readButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Выбрать файл", "/home/Documents",
                                                "images files (*.png);; All files (*.*);");
    qDebug() << "File Path: " << filePath;
    sendMessageToDBus(filePath);
}

void MainWindow::on_launchButton_clicked()
{
    filePath = "/home/dmippy/Documents/abc.txt";
    sendMessageToDBus(filePath);
}


