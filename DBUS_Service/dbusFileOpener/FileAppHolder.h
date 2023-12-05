#ifndef FILEAPPHOLDER_H
#define FILEAPPHOLDER_H
#include <QObject>
#include <QDebug>

class FileAppHolder : public QObject
{
    Q_OBJECT

public: FileAppHolder();

    const QMap<QString, QString>& getFileApp() const
    {
        return fileApplications_;
    }

    void addElemToFileApp(QString& key, QString& elem)
    {
        if (fileApplications_.contains(key)) {
            qDebug() << "Элемент с расширением " << key << " уже зарегистрирован";
        }
        else {
            fileApplications_.insert(key, elem);
        }
    }

private:
    QMap<QString, QString> fileApplications_
    {
        //{"txt", "gedit"},
        //{"c", "gedit"},
        //{"jpg", "eog"},
        //{"png", "eog"},
        //{"gif", "eog"},
        //{"mp3", "totem"},
        //{"wav", "totem"},
    };
};

#endif // FILEAPPHOLDER_H
