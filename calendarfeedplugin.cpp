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

#include "calendarfeedplugin.h"

#include <libsyncpluginmgr/PluginCbInterface.h>
#include <LogMacros.h>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QUrl>
#include <meventfeed.h>
#include <gconfitem.h>
#include <QVariant>
#include <QOrganizerManager>
#include <QOrganizerItem>
#include <QOrganizerEventTime>
#include <QOrganizerTodoTime>
#include <QTime>
#include <QDate>
#include <QDBusConnection>
#include <QDataStream>

QTM_USE_NAMESPACE

extern "C" CalendarFeedPlugin *createPlugin(const QString& pluginName,
                                            const Buteo::SyncProfile &profile,
                                            Buteo::PluginCbInterface *cbInterface)
{
    return new CalendarFeedPlugin(pluginName, profile, cbInterface);
}

extern "C" void destroyPlugin(CalendarFeedPlugin *client)
{
    delete client;
}

CalendarFeedPlugin::CalendarFeedPlugin(const QString &pluginName,
                                       const Buteo::SyncProfile &profile,
                                       Buteo::PluginCbInterface *cbInterface) :
    ClientPlugin(pluginName, profile, cbInterface)
{
}

CalendarFeedPlugin::~CalendarFeedPlugin()
{
}

bool CalendarFeedPlugin::init()
{
    return true;
}

bool CalendarFeedPlugin::uninit()
{
    return true;
}

bool CalendarFeedPlugin::startSync()
{
    GConfItem enabledConfItem("/apps/ControlPanel/CalendarFeed/EnableFeed");
    QVariant enabledVariant = enabledConfItem.value();
    if (enabledVariant.isValid()) {
        bool enabled = enabledVariant.toBool();
        if (!enabled) {
            MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");
            return false;
        }
    }
    QTimer::singleShot(1, this, SLOT(updateFeed()));

    return true;
}

void CalendarFeedPlugin::abortSync(Sync::SyncStatus status)
{
    Q_UNUSED(status);
}

bool CalendarFeedPlugin::cleanUp()
{
    return true;
}

Buteo::SyncResults CalendarFeedPlugin::getSyncResults() const
{
    return m_results;
}

void CalendarFeedPlugin::connectivityStateChanged(Sync::ConnectivityType type,
                                                  bool state)
{
    Q_UNUSED(type);
    Q_UNUSED(state);
}

void CalendarFeedPlugin::syncSuccess()
{
    updateResults(Buteo::SyncResults(QDateTime::currentDateTime(), Buteo::SyncResults::SYNC_RESULT_SUCCESS, Buteo::SyncResults::NO_ERROR));
    emit success(getProfileName(), "Success!!");
}

void CalendarFeedPlugin::syncFailed()
{
    updateResults(Buteo::SyncResults(QDateTime::currentDateTime(),
                                     Buteo::SyncResults::SYNC_RESULT_FAILED, Buteo::SyncResults::ABORTED));
    emit error(getProfileName(), "Error!!", Buteo::SyncResults::SYNC_RESULT_FAILED);
}

void CalendarFeedPlugin::updateFeed()
{
    QString body;
    QString icon;
    QDate firstEventDate = QDate::currentDate();

    QOrganizerManager manager;
    QDateTime startDateTime = QDateTime::currentDateTime();
    QDateTime endDateTime = QDateTime::currentDateTime();
    endDateTime.setTime(QTime(23, 59, 59));

    QList<QOrganizerItem> events = manager.items(startDateTime, endDateTime);
    bool fillWithFuture = false;
    GConfItem fillConfItem("/apps/ControlPanel/CalendarFeed/FillWithFuture");
    QVariant fillVariant = fillConfItem.value();
    if (fillVariant.isValid())
        fillWithFuture = fillVariant.toBool();
    if (fillWithFuture || events.isEmpty()) {
        endDateTime = QDateTime();
        GConfItem limitFutureConfItem("/apps/ControlPanel/CalendarFeed/LimitFuture");
        QVariant limitFutureVariant = limitFutureConfItem.value();
        if (limitFutureVariant.isValid() && limitFutureVariant.toBool()) {
            GConfItem limitSizeConfItem("/apps/ControlPanel/CalendarFeed/LimitDaysSize");
            QVariant limitSizeVariant = limitSizeConfItem.value();
            int limitSize = (limitSizeVariant.isValid()) ? limitSizeVariant.toInt() : 7;
            if (limitSize < 1)
                limitSize = 7;
            endDateTime = QDateTime::currentDateTime().addDays(limitSize);
            endDateTime.setTime(QTime(23, 59, 59));
        }

        events = manager.items(startDateTime, endDateTime);
    }

    QList<QOrganizerItem> displayableEvents;
    int displayableCount = 3;
    GConfItem displayableConfItem("/apps/ControlPanel/CalendarFeed/FeedSize");
    QVariant displayableVariant = displayableConfItem.value();
    if (displayableVariant.isValid())
        displayableCount = displayableVariant.toInt();
    if (displayableCount < 1)
        displayableCount = 3;

    for (int i = 0; i < displayableCount && i < events.size(); ++i)
        displayableEvents << events[i];

    QStringList descriptions;
    bool isFirstDate = true;
    foreach (QOrganizerItem event, displayableEvents) {
        QString eventDescription;
        bool isAllDay = false;
        QDateTime startDateTime = QDateTime::currentDateTime();

        if (event.type().toLower() == "todo") {
            QOrganizerTodoTime todoTimeDetail = event.detail<QOrganizerTodoTime>();
            startDateTime = todoTimeDetail.dueDateTime();
            isAllDay = true;
        } else {
            QOrganizerEventTime eventTimeDetail = event.detail<QOrganizerEventTime>();
            isAllDay = eventTimeDetail.isAllDay();
            startDateTime = eventTimeDetail.startDateTime();
            QDateTime endDateTime = eventTimeDetail.endDateTime();
            if (!isAllDay &&
                    startDateTime.time().hour() == 0 &&
                    startDateTime.time().minute() == 0 &&
                    endDateTime.time().hour() == 0 &&
                    endDateTime.time().minute() == 0 &&
                    startDateTime.date().addDays(1) == endDateTime.date())
                isAllDay = true;
        }

        QDate startDate = startDateTime.date();
        if (startDate.isNull())
            startDate = QDate::currentDate();
        if (isFirstDate) {
            isFirstDate = false;
            firstEventDate = startDate;
        }
        if (startDate != QDate::currentDate())
            eventDescription += QString("%1").arg(startDate.toString("MMM, d "));
        if (!isAllDay)
            eventDescription += QString("%1").arg(startDateTime.time().toString("hh:mm "));
        eventDescription += event.displayLabel();
        if (eventDescription.length() > 32)
            eventDescription = eventDescription.left(29)+"...";

        descriptions << eventDescription;
        if (icon.isEmpty()) {
            icon = QString("icon-l-calendar-%1").arg(startDate.toString("dd"));
        }
    }

    if (descriptions.isEmpty())
        body = "No events in your calendar.";
    else
        body = descriptions.join("<br />");
    if (icon.isEmpty())
        icon = QString("icon-l-calendar-%1").arg(QDate::currentDate().toString("dd"));

    MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");

    QDBusMessage message = QDBusMessage::createMethodCall(
            "com.nokia.home.EventFeed",
            "/eventfeed",
            "com.nokia.home.EventFeed",
            "addItem");

    QList<QVariant> args;
    QVariantMap itemArgs;
    itemArgs.insert("title", "Calendar");
    itemArgs.insert("icon", icon);
    itemArgs.insert("body", body);
    itemArgs.insert("timestamp", QDateTime::currentDateTime().addDays(1).toString("yyyy-MM-dd hh:mm:ss"));
    itemArgs.insert("sourceName", "SyncFW-calendarfeed");
    itemArgs.insert("sourceDisplayName", "Calendar Feed");
    itemArgs.insert("action", QString("com.nokia.Calendar / com.nokia.maemo.meegotouch.CalendarInterface showMonthView %1 %2 %3")
                    .arg(base64SerializedVariant(firstEventDate.year()))
                    .arg(base64SerializedVariant(firstEventDate.month()))
                    .arg(base64SerializedVariant(firstEventDate.day())));

    QDBusConnection bus = QDBusConnection::sessionBus();

    args.append(itemArgs);
    message.setArguments(args);
    bus.callWithCallback(message, this, SLOT(dbusRequestCompleted(QDBusMessage)));
}

void CalendarFeedPlugin::dbusRequestCompleted(const QDBusMessage &reply)
{
    int replyId = reply.arguments().first().toInt();
    if(replyId < 0)
        syncFailed();
    else
        syncSuccess();
}

void CalendarFeedPlugin::dbusErrorOccured(const QDBusError &error, const QDBusMessage &message)
{
    Q_UNUSED(error);
    Q_UNUSED(message);
    syncFailed();
}

void CalendarFeedPlugin::updateResults(const Buteo::SyncResults &results)
{
    m_results = results;
    m_results.setScheduled(true);
}

QString CalendarFeedPlugin::base64SerializedVariant(const QVariant &value) const
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << value;
    return ba.toBase64();
}
