#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QObject>
#include <QDBusMessage>
#include <QDateTime>

class SettingsHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY refreshIntervalChanged)
public:
    explicit SettingsHelper(QObject *parent = 0);

    Q_INVOKABLE QString formatDate(const QDateTime &date, const QString &format);

    int refreshInterval() const;
    void setRefreshInterval(int arg);

signals:
    void errorOccured(const QString &message);

    void refreshIntervalChanged(int arg);

public slots:
    void refreshFeedItem();

protected slots:
    void dbusRequestCompleted(const QDBusMessage &reply);
    void dbusErrorOccured(const QDBusError &error, const QDBusMessage &message);

private:
    int m_refreshInterval;
};

#endif // SETTINGSHELPER_H
