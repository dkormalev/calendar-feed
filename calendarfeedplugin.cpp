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
#include <QTime>
#include <QDate>

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
    GConfItem enabledLastId("/apps/ControlPanel/CalendarFeed/EnableFeed");
    QVariant enabledVariant = enabledLastId.value();
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

void CalendarFeedPlugin::updateResults(const Buteo::SyncResults &results)
{
    m_results = results;
    m_results.setScheduled(true);
}

void CalendarFeedPlugin::updateFeed()
{
    QString body;
    QString icon;

    QOrganizerManager manager;
    QDateTime startDateTime = QDateTime::currentDateTime();
    QDateTime endDateTime = QDateTime::currentDateTime();
    endDateTime.setTime(QTime(23, 59, 59));

    QList<QOrganizerItem> events = manager.items(startDateTime, endDateTime);
    if (events.isEmpty()) {
        events = manager.items(startDateTime, QDateTime());
    }

    QList<QOrganizerItem> displayableEvents;
    for (int i = 0; i < 3 && i < events.size(); ++i)
        displayableEvents << events[i];

    QStringList descriptions;
    foreach (QOrganizerItem event, displayableEvents) {
        QString eventDescription;
        QOrganizerEventTime eventTimeDetail = event.detail<QOrganizerEventTime>();
        bool isAllDay = eventTimeDetail.isAllDay();
        QDateTime startDateTime = eventTimeDetail.startDateTime();
        QDateTime endDateTime = eventTimeDetail.endDateTime();
        if (!isAllDay &&
                startDateTime.time().hour() == 0 &&
                startDateTime.time().minute() == 0 &&
                endDateTime.time().hour() == 0 &&
                endDateTime.time().minute() == 0 &&
                startDateTime.date().addDays(1) == endDateTime.date())
            isAllDay = true;
        QDate startDate = startDateTime.date();
        if (startDate != QDate::currentDate())
            eventDescription += startDate.toString("MMM, d ");
        if (!isAllDay)
            eventDescription += startDateTime.time().toString("hh:mm ");
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
        icon = "icon-l-calendar";

    bool success = false;
    //Ok assuming that we now have the data that needs to be updated to event feed
    MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");
    qlonglong id  = MEventFeed::instance()->addItem(icon,
                                                    QString("Calendar"),
                                                    body,
                                                    QStringList(),
                                                    QDateTime::currentDateTime().addSecs(3600),
                                                    QString(),
                                                    false,
                                                    QUrl(),
                                                    QString("SyncFW-calendarfeed"),
                                                    QString("Calendar Feed"));
    if (id != -1) {
        success = true;
    }
    if(success)
        syncSuccess();
    else
        syncFailed();
}
