/**************************************************************************
**
** This file is part of Calendar Feed
**
** Copyright (C) 2011  Denis Kormalev <kormalev.denis@gmail.com>
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** This application is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
**
**************************************************************************/

#include "settingshelper.h"
#include <QDBusConnection>
#include <QFile>
#include <QXmlStreamReader>

SettingsHelper::SettingsHelper(QObject *parent) :
    QObject(parent)
{
    m_refreshInterval = 20;
    QString fileName = "/home/user/.sync/profiles/sync/calendarfeed.xml";
    if (!QFile::exists(fileName))
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

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
}

QString SettingsHelper::formatDate(const QDateTime &date, const QString &format)
{
    return date.toString(format);
}

int SettingsHelper::refreshInterval() const
{
    return m_refreshInterval;
}

void SettingsHelper::setRefreshInterval(int arg)
{
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
