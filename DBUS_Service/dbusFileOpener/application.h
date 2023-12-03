#ifndef APPLICATION_H
#define APPLICATION_H


#include <QString>
#include <QList>

class Application
{
public:
    Application(const QString& name, const QList<QString>& capabilities, const QString& command);

    QString name() const;
    QList<QString> capabilities() const;
    QString command() const;

private:
    QString m_name;
    QList<QString> m_capabilities;
    QString m_command;
};

#endif // APPLICATION_H
