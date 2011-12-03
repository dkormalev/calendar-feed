#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QObject>
#include <QDBusMessage>

class SettingsHelper : public QObject
{
    Q_OBJECT
public:
    explicit SettingsHelper(QObject *parent = 0);

signals:
    void errorOccured(const QString &message);

public slots:
    void refreshFeedItem();

protected slots:
    void dbusRequestCompleted(const QDBusMessage &reply);
    void dbusErrorOccured(const QDBusError &error, const QDBusMessage &message);

};

#endif // SETTINGSHELPER_H
