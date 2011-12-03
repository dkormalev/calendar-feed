#include "settingshelper.h"
#include <QDBusConnection>

SettingsHelper::SettingsHelper(QObject *parent) :
    QObject(parent)
{
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
}
