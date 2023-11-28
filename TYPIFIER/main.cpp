#include "mainwindow.h"
#include <iostream>
#include <QString>
#include <unordered_map>
#include <QDebug>
#include <QTextStream>
#include <QApplication>
#include <QMap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
