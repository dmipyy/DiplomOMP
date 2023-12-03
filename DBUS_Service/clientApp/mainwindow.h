#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QProcess>
#include <QStringList>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void readFilePath();

private:
    QLineEdit *filePathEdit;
    QPushButton *readButton;
};

#endif // MAINWINDOW_H
