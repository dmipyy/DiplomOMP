#include <mainwindow.h>
#include <dbus.h>
#include <QDebug>
#include<QFileDialog>

extern QString choosedMethod;
MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    readButton = new QPushButton("Открыть файл", this);
    layout->addWidget(readButton);
    launchButton = new QPushButton("Выбрать файл зарегестрированных приложений", this);
    layout->addWidget(launchButton);

    connect(readButton, &QPushButton::clicked, this, &MainWindow::on_readButton_clicked);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::on_launchButton_clicked);
    //connect(regFileButton, &QPushButton::clicked, this, &MainWindow::on_regFileButton_clicked);
}

void MainWindow::on_readButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выбрать файл", "", "Все файлы (*)");
    choosedMethod = "open";
    qDebug() << "File Path: " << filePath;
    sendMessageToDBus(filePath, choosedMethod);
}

void MainWindow::on_launchButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите текстовый файл регистрации", "", "Текстовые файлы (*.txt)");
    choosedMethod = "reg";
    qDebug() << "File Path: " << filePath;
    sendMessageToDBus(filePath, choosedMethod);
}

