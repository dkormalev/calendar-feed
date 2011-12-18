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
#include <QVariant>
#include <QOrganizerManager>
#include <QOrganizerItem>
#include <QOrganizerEventTime>
#include <QOrganizerTodoTime>
#include <QTime>
#include <QDate>
#include <QDBusConnection>
#include <QDataStream>
#include <MLocale>
#include <mkcal/extendedcalendar.h>
#include <mkcal/extendedstorage.h>
#include <kcalcoren/ksystemtimezone.h>
#include <kcalcoren/event.h>

#include "settings.h"
#include "calendarevent.h"

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
    m_settings = new Settings();
    if (m_settings->isEnabled()) {
        QTimer::singleShot(1, this, SLOT(updateFeed()));
        return true;
    } else {
        MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");
        return false;
    }
}

void CalendarFeedPlugin::abortSync(Sync::SyncStatus status)
{
    Q_UNUSED(status);
}

bool CalendarFeedPlugin::cleanUp()
{
    delete m_settings;
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
    bool fillWithFuture = m_settings->isFilledWithFuture();

    QHash<QString, QList<QDateTime> > correctStartDateTimes;
    QHash<QString, bool> allDaysFromKCal;

    QOrganizerManager manager;
    QDateTime startDateTime = QDateTime::currentDateTime();
    QDateTime endDateTime = QDateTime::currentDateTime();
    endDateTime.setTime(QTime(23, 59, 59));


    QList<QOrganizerItem> events = manager.items(startDateTime, endDateTime);

    if (fillWithFuture || events.isEmpty()) {
        endDateTime = QDateTime();
        if (m_settings->isFutureLimited()) {
            endDateTime = QDateTime::currentDateTime().addDays(m_settings->futureLimit());
            endDateTime.setTime(QTime(23, 59, 59));
        }
        events = manager.items(startDateTime, endDateTime);
    }
    QDateTime todoStartTime = QDateTime::currentDateTime();
    todoStartTime.setTime(QTime(0,0));
    QList<QOrganizerItem> possibleToDos = manager.items(todoStartTime, startDateTime);

    QString firstFetchedEventGuid = "";
    if (!events.empty())
        firstFetchedEventGuid = events[0].guid();
    QList<QOrganizerItem> toDosToAdd;
    foreach(QOrganizerItem event, possibleToDos) {
        if (event.type().toLower() == "todo" && event.guid() != firstFetchedEventGuid)
            toDosToAdd << event;
    }
    while (!toDosToAdd.empty())
        events.prepend(toDosToAdd.takeLast());

    QList<QOrganizerItem> displayableEvents;
    int displayableCount = m_settings->eventsShown();

    for (int i = 0; i < displayableCount && i < events.size(); ++i)
        displayableEvents << events[i];

    QDate startDateForMKCal = QDate::currentDate();
    QDate endDateForMKCal = QDate::currentDate();

    if (displayableEvents.size()) {
        QOrganizerEventTime eventTimeDetail = displayableEvents[0].detail<QOrganizerEventTime>();
        if (eventTimeDetail.startDateTime().isValid())
            startDateForMKCal = eventTimeDetail.startDateTime().date().addDays(-2);
        eventTimeDetail = displayableEvents[displayableEvents.size()-1].detail<QOrganizerEventTime>();
        if (eventTimeDetail.endDateTime().isValid())
            endDateForMKCal = eventTimeDetail.endDateTime().date().addDays(1);
        else if (startDateForMKCal > QDate::currentDate())
            endDateForMKCal = startDateForMKCal;
    }

    //TODO: need to think more about it. Maybe full switch to mkcal will work better here
    mKCal::ExtendedCalendar::Ptr calendarBackend = mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));

    mKCal::ExtendedStorage::Ptr calendarStorage =
            mKCal::ExtendedStorage::Ptr(mKCal::ExtendedCalendar::defaultStorage(calendarBackend));
    calendarStorage->open();
    calendarStorage->load(startDateForMKCal, endDateForMKCal);
    calendarStorage->loadRecurringIncidences();

    KCalCore::Incidence::List incidences = calendarBackend->incidences(startDateForMKCal, endDateForMKCal);

    mKCal::ExtendedCalendar::ExpandedIncidenceList
            incidenceList = calendarBackend->expandRecurrences(&incidences,
                                                                    KDateTime(startDateForMKCal),
                                                                    KDateTime(endDateForMKCal));

    foreach(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident,
            incidenceList) {
        KCalCore::Incidence::Ptr incidence = expandedIncident.second;
        if (incidence->type() != KCalCore::IncidenceBase::TypeEvent)
            continue;
        KCalCore::Event::Ptr event = incidence.staticCast<KCalCore::Event>();
        correctStartDateTimes[event->uid()] << expandedIncident.first.dtStart;
        allDaysFromKCal[event->uid()] = event->allDay();
    }

    QList<CalendarEvent *> eventsToShow;
    foreach (QOrganizerItem event, displayableEvents) {
        CalendarEvent *toAdd = new CalendarEvent;

        if (event.type().toLower() == "todo") {
            QOrganizerTodoTime todoTimeDetail = event.detail<QOrganizerTodoTime>();
            toAdd->setStartDate(todoTimeDetail.dueDateTime());
            toAdd->setAllDay(true);
        } else {
            QOrganizerEventTime eventTimeDetail = event.detail<QOrganizerEventTime>();
            toAdd->setAllDay(eventTimeDetail.isAllDay());
            QDateTime eventStartDateTime = eventTimeDetail.startDateTime();
            QDateTime eventEndDateTime = eventTimeDetail.endDateTime();

            QString uid = event.guid();
            if (correctStartDateTimes.contains(uid)) {
                foreach (const QDateTime &dateTime, correctStartDateTimes[uid]) {
                    if (dateTime.date() == eventStartDateTime.date()) {
                        if (dateTime.time() != eventStartDateTime.time()) {
                            eventStartDateTime.setTime(dateTime.time());
                            eventEndDateTime = eventEndDateTime.addSecs(eventStartDateTime.time().secsTo(dateTime.time()));
                        }
                    }
                }
            }
            if (!toAdd->isAllDay() && allDaysFromKCal.contains(uid))
                toAdd->setAllDay(allDaysFromKCal[uid]);

            if (!toAdd->isAllDay() &&
                    eventStartDateTime.time().hour() == 0 &&
                    eventStartDateTime.time().minute() == 0 &&
                    eventEndDateTime.time().hour() == 0 &&
                    eventEndDateTime.time().minute() == 0 &&
                    eventStartDateTime.date().addDays(1) == eventEndDateTime.date())
                toAdd->setAllDay(true);
            toAdd->setStartDate(eventStartDateTime);
        }
        toAdd->setSummary(event.displayLabel());
        toAdd->setCalendarColor(manager.collection(event.collectionId())
                               .metaData(QOrganizerCollection::KeyColor).toString());
        eventsToShow << toAdd;
    }

    fillFeed(eventsToShow);
    qDeleteAll(eventsToShow);
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

void CalendarFeedPlugin::fillFeed(const QList<CalendarEvent *> &events)
{
    MLocale locale;
    locale.installTrCatalog("calendarfeed");
    bool showCalendarBar = m_settings->isCalendarColorShown();
    QString dateFormat = m_settings->dateFormat() + " ";
    bool highlightToday = m_settings->isTodayHighlighted();

    QString body;
    QString icon;
    QDate firstEventDate = QDate::currentDate();

    QStringList descriptions;
    bool isFirstDate = true;
    foreach (CalendarEvent *event, events) {
        QString eventDescription;

        QDate startDate = event->startDateTime().date();
        if (startDate.isNull())
            startDate = QDate::currentDate();
        if (isFirstDate) {
            isFirstDate = false;
            firstEventDate = startDate;
        }

        bool greyOutThisEvent = false;
        if (startDate != QDate::currentDate()) {
            eventDescription += QString("%1").arg(startDate.toString(dateFormat));
            greyOutThisEvent = highlightToday;
        }
        if (!event->isAllDay())
            eventDescription += QString("%1").arg(event->startDateTime().time().toString("hh:mm "));
        eventDescription += event->summary();

        if (eventDescription.length() > 32)
            eventDescription = eventDescription.left(30-(showCalendarBar ? 1 : 0))+"&#x2026;";

        eventDescription.replace(" ", "&nbsp;");

        if (greyOutThisEvent)
            eventDescription = QString("<font color='#A0A0A0'>%1</font>").arg(eventDescription);

        if (showCalendarBar) {
            eventDescription = QString("<font color='%1'>&#x2503;</font>%2")
                    .arg(event->calendarColor())
                    .arg(eventDescription);
        }

        descriptions << eventDescription;
        if (icon.isEmpty()) {
            icon = QString("icon-l-calendar-%1").arg(startDate.toString("dd"));
        }
    }

    if (descriptions.isEmpty())
        //% "No events in your calendar."
        body = locale.translate("", "calendar_feed_item_no_events");
    else
        body = descriptions.join("<br />");
    if (icon.isEmpty())
        icon = QString("icon-l-calendar-%1").arg(QDate::currentDate().toString("dd"));

    QDBusMessage message = QDBusMessage::createMethodCall(
            "com.nokia.home.EventFeed",
            "/eventfeed",
            "com.nokia.home.EventFeed",
            "addItem");

    QList<QVariant> args;
    QVariantMap itemArgs;
    itemArgs.insert("title", locale.translate("", "calendar_feed_item_title"));
    itemArgs.insert("icon", icon);
    itemArgs.insert("body", body);
    itemArgs.insert("timestamp", QDateTime::currentDateTime().addDays(1).toString("yyyy-MM-dd hh:mm:ss"));
    itemArgs.insert("sourceName", "SyncFW-calendarfeed");
    itemArgs.insert("sourceDisplayName", locale.translate("", "calendar_feed_title"));
    itemArgs.insert("action", QString("com.nokia.Calendar / com.nokia.maemo.meegotouch.CalendarInterface showMonthView %1 %2 %3")
                    .arg(base64SerializedVariant(firstEventDate.year()))
                    .arg(base64SerializedVariant(firstEventDate.month()))
                    .arg(base64SerializedVariant(firstEventDate.day())));

    QDBusConnection bus = QDBusConnection::sessionBus();

    args.append(itemArgs);
    message.setArguments(args);
    MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");
    bus.callWithCallback(message, this, SLOT(dbusRequestCompleted(QDBusMessage)), SLOT(dbusErrorOccured(QDBusError,QDBusMessage)));
}

QString CalendarFeedPlugin::base64SerializedVariant(const QVariant &value) const
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << value;
    return ba.toBase64();
}




/*
  There are some rumors about mkcal problems at PR1.2 so let old code be here for easy access
  TODO: remove after PR1.2 release

void CalendarFeedPlugin::updateFeed()
{
    bool fillWithFuture = m_settings->isFilledWithFuture();

    QHash<QString, QList<QDateTime> > correctStartDateTimes;
    QHash<QString, bool> allDaysFromKCal;

    QOrganizerManager manager;
    QDateTime startDateTime = QDateTime::currentDateTime();
    QDateTime endDateTime = QDateTime::currentDateTime();
    endDateTime.setTime(QTime(23, 59, 59));

    QList<QOrganizerItem> events = manager.items(startDateTime, endDateTime);

    if (fillWithFuture || events.isEmpty()) {
        endDateTime = QDateTime();
        if (m_settings->isFutureLimited()) {
            endDateTime = QDateTime::currentDateTime().addDays(m_settings->futureLimit());
            endDateTime.setTime(QTime(23, 59, 59));
        }
        events = manager.items(startDateTime, endDateTime);
    }
    QDateTime todoStartTime = QDateTime::currentDateTime();
    todoStartTime.setTime(QTime(0,0));
    QList<QOrganizerItem> possibleToDos = manager.items(todoStartTime, startDateTime);

    QString firstFetchedEventGuid = "";
    if (!events.empty())
        firstFetchedEventGuid = events[0].guid();
    QList<QOrganizerItem> toDosToAdd;
    foreach(QOrganizerItem event, possibleToDos) {
        if (event.type().toLower() == "todo" && event.guid() != firstFetchedEventGuid)
            toDosToAdd << event;
    }
    while (!toDosToAdd.empty())
        events.prepend(toDosToAdd.takeLast());

    QList<QOrganizerItem> displayableEvents;
    int displayableCount = m_settings->eventsShown();

    for (int i = 0; i < displayableCount && i < events.size(); ++i)
        displayableEvents << events[i];

    QDate startDateForMKCal = QDate::currentDate();
    QDate endDateForMKCal = QDate::currentDate();

    if (displayableEvents.size()) {
        QOrganizerEventTime eventTimeDetail = displayableEvents[0].detail<QOrganizerEventTime>();
        if (eventTimeDetail.startDateTime().isValid())
            startDateForMKCal = eventTimeDetail.startDateTime().date().addDays(-2);
        eventTimeDetail = displayableEvents[displayableEvents.size()-1].detail<QOrganizerEventTime>();
        if (eventTimeDetail.endDateTime().isValid())
            endDateForMKCal = eventTimeDetail.endDateTime().date().addDays(1);
        else if (startDateForMKCal > QDate::currentDate())
            endDateForMKCal = startDateForMKCal;
    }

    //TODO: need to think more about it. Maybe full switch to mkcal will work better here
    mKCal::ExtendedCalendar::Ptr calendarBackend = mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));

    mKCal::ExtendedStorage::Ptr calendarStorage =
            mKCal::ExtendedStorage::Ptr(mKCal::ExtendedCalendar::defaultStorage(calendarBackend));
    calendarStorage->open();
    calendarStorage->load(startDateForMKCal, endDateForMKCal);
    calendarStorage->loadRecurringIncidences();

    KCalCore::Incidence::List incidences = calendarBackend->incidences(startDateForMKCal, endDateForMKCal);

    mKCal::ExtendedCalendar::ExpandedIncidenceList
            incidenceList = calendarBackend->expandRecurrences(&incidences,
                                                                    KDateTime(startDateForMKCal),
                                                                    KDateTime(endDateForMKCal));

    foreach(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident,
            incidenceList) {
        KCalCore::Incidence::Ptr incidence = expandedIncident.second;
        if (incidence->type() != KCalCore::IncidenceBase::TypeEvent)
            continue;
        KCalCore::Event::Ptr event = incidence.staticCast<KCalCore::Event>();
        correctStartDateTimes[event->uid()] << expandedIncident.first.dtStart;
        allDaysFromKCal[event->uid()] = event->allDay();
    }

    QList<CalendarEvent *> eventsToShow;
    foreach (QOrganizerItem event, displayableEvents) {
        CalendarEvent *toAdd = new CalendarEvent;

        if (event.type().toLower() == "todo") {
            QOrganizerTodoTime todoTimeDetail = event.detail<QOrganizerTodoTime>();
            toAdd->setStartDate(todoTimeDetail.dueDateTime());
            toAdd->setAllDay(true);
        } else {
            QOrganizerEventTime eventTimeDetail = event.detail<QOrganizerEventTime>();
            toAdd->setAllDay(eventTimeDetail.isAllDay());
            QDateTime eventStartDateTime = eventTimeDetail.startDateTime();
            QDateTime eventEndDateTime = eventTimeDetail.endDateTime();

            QString uid = event.guid();
            if (correctStartDateTimes.contains(uid)) {
                foreach (const QDateTime &dateTime, correctStartDateTimes[uid]) {
                    if (dateTime.date() == eventStartDateTime.date()) {
                        if (dateTime.time() != eventStartDateTime.time()) {
                            eventStartDateTime.setTime(dateTime.time());
                            eventEndDateTime = eventEndDateTime.addSecs(eventStartDateTime.time().secsTo(dateTime.time()));
                        }
                    }
                }
            }
            if (!toAdd->isAllDay() && allDaysFromKCal.contains(uid))
                toAdd->setAllDay(allDaysFromKCal[uid]);

            if (!toAdd->isAllDay() &&
                    eventStartDateTime.time().hour() == 0 &&
                    eventStartDateTime.time().minute() == 0 &&
                    eventEndDateTime.time().hour() == 0 &&
                    eventEndDateTime.time().minute() == 0 &&
                    eventStartDateTime.date().addDays(1) == eventEndDateTime.date())
                toAdd->setAllDay(true);
            toAdd->setStartDate(eventStartDateTime);
        }
        toAdd->setSummary(event.displayLabel());
        toAdd->setCalendarColor(manager.collection(event.collectionId())
                               .metaData(QOrganizerCollection::KeyColor).toString());
        eventsToShow << toAdd;
    }

    fillFeed(eventsToShow);
    qDeleteAll(eventsToShow);
}

*/
