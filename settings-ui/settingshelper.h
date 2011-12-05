#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QObject>
#include <QDBusMessage>
#include <QDateTime>

class SettingsHelper : public QObject
{
    Q_OBJECT
public:
    explicit SettingsHelper(QObject *parent = 0);

    Q_INVOKABLE QString formatDate(const QDateTime &date, const QString &format);

signals:
    void errorOccured(const QString &message);

public slots:
    void refreshFeedItem();

protected slots:
    void dbusRequestCompleted(const QDBusMessage &reply);
    void dbusErrorOccured(const QDBusError &error, const QDBusMessage &message);

};

#endif // SETTINGSHELPER_H
