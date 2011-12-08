#include "settingshelper.h"
#include <QDBusConnection>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

SettingsHelper::SettingsHelper(QObject *parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    m_refreshInterval = 20;
    QString fileName = "/home/user/.sync/profiles/sync/calendarfeed.xml";
    if (!QFile::exists(fileName))
        return;
    qDebug() << Q_FUNC_INFO << "Sync profile file exists";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    qDebug() << Q_FUNC_INFO << "Sync profile file opened ok";

    QXmlStreamReader reader(&file);
    while(!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == "schedule") {
            m_refreshInterval = reader.attributes().value("interval").toString().toInt();
            //We don't set 1 here, because in most cases 0 here means error in parsing,
            //so we simply falling back to default
            if (m_refreshInterval < 1)
                m_refreshInterval = 20;
            if (m_refreshInterval > 60)
                m_refreshInterval = 60;
            break;
        }
    }
    qDebug() << Q_FUNC_INFO << "Refresh interval=" << m_refreshInterval;
}

QString SettingsHelper::formatDate(const QDateTime &date, const QString &format)
{
    return date.toString(format);
}

int SettingsHelper::refreshInterval() const
{
    qDebug() << Q_FUNC_INFO << m_refreshInterval;
    return m_refreshInterval;
}

void SettingsHelper::setRefreshInterval(int arg)
{
    qDebug() << Q_FUNC_INFO << arg << m_refreshInterval;
    if (arg < 1)
        arg = 1;
    if (arg > 60)
        arg = 60;
    if (m_refreshInterval != arg) {
        m_refreshInterval = arg;
        QDBusMessage message = QDBusMessage::createMethodCall(
                "com.meego.msyncd",
                "/synchronizer",
                "com.meego.msyncd",
                "setSyncSchedule");
        QList<QVariant> args;
        args
                << "calendarfeed"
                << QString("<schedule enabled=\"true\" interval=\"%1\" days=\"1,2,3,4,5,6,7\" syncconfiguredtime=\"\" time=\"\"></schedule>")
                   .arg(m_refreshInterval);
        message.setArguments(args);
        QDBusConnection bus = QDBusConnection::sessionBus();
        if (bus.isConnected()) {
            bus.callWithCallback(message, this, SLOT(dbusRequestCompleted(QDBusMessage)), SLOT(dbusErrorOccured(QDBusError,QDBusMessage)));
        }
        else {
            //% "Internal OS error. Please, reboot your device."
            emit errorOccured(qtTrId("calendar_feed_refresh_error"));
        }

        emit refreshIntervalChanged(arg);
    }
}


void SettingsHelper::refreshFeedItem()
{

    QDBusMessage message = QDBusMessage::createMethodCall(
            "com.meego.msyncd",
            "/synchronizer",
            "com.meego.msyncd",
            "startSync");
    QList<QVariant> args;
    args << "calendarfeed";
    message.setArguments(args);
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (bus.isConnected()) {
        bus.callWithCallback(message, this, SLOT(dbusRequestCompleted(QDBusMessage)), SLOT(dbusErrorOccured(QDBusError,QDBusMessage)));
    }
    else {
        //% "Internal OS error. Please, reboot your device."
        emit errorOccured(qtTrId("calendar_feed_refresh_error"));
    }
}

void SettingsHelper::dbusRequestCompleted(const QDBusMessage &reply)
{
    bool result = reply.arguments().first().toBool();
    if(!result) {
        emit errorOccured(qtTrId("calendar_feed_refresh_error"));
    }
}

void SettingsHelper::dbusErrorOccured(const QDBusError &error, const QDBusMessage &message)
{
    Q_UNUSED(error);
    Q_UNUSED(message);
    emit errorOccured(qtTrId("calendar_feed_refresh_error"));
}

void dummyTr() {
    //% "Calendar"
    qtTrId("calendar_feed_item_title");
    //% "No events in your calendar."
    qtTrId("calendar_feed_item_no_events");
    //% "Calendars Shown"
    qtTrId("calendar_feed_setting_calendars_shown");
}
