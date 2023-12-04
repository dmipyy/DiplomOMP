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
    void on_readButton_clicked();
    void on_launchButton_clicked();
private:
    QPushButton *readButton;
    QPushButton *launchButton;
    QString filePath;
};

#endif // MAINWINDOW_H
