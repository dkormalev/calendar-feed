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
#include <kcalcoren/ksystemtimezone.h>
#include <kcalcoren/event.h>

#include "settings.h"
#include "calendarevent.h"

#include <QFile>


QTM_USE_NAMESPACE

extern "C" CalendarFeedPlugin *createPlugin(const QString& pluginName,
                                            const Buteo::SyncProfile &profile,
                                            Buteo::PluginCbInterface *cbInterface)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    return new CalendarFeedPlugin(pluginName, profile, cbInterface);
}

extern "C" void destroyPlugin(CalendarFeedPlugin *client)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    delete client;
}

CalendarFeedPlugin::CalendarFeedPlugin(const QString &pluginName,
                                       const Buteo::SyncProfile &profile,
                                       Buteo::PluginCbInterface *cbInterface) :
    ClientPlugin(pluginName, profile, cbInterface)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
}

CalendarFeedPlugin::~CalendarFeedPlugin()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
}

bool CalendarFeedPlugin::init()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    m_locale.installTrCatalog("calendarfeed");
    m_settings = new Settings();
    return true;
}

bool CalendarFeedPlugin::uninit()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    delete m_settings;
    return true;
}

bool CalendarFeedPlugin::startSync()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
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
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    Q_UNUSED(status);
}

bool CalendarFeedPlugin::cleanUp()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    return true;
}

Buteo::SyncResults CalendarFeedPlugin::getSyncResults() const
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    return m_results;
}

void CalendarFeedPlugin::connectivityStateChanged(Sync::ConnectivityType type,
                                                  bool state)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    Q_UNUSED(type);
    Q_UNUSED(state);
}

void CalendarFeedPlugin::syncSuccess()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    updateResults(Buteo::SyncResults(QDateTime::currentDateTime(), Buteo::SyncResults::SYNC_RESULT_SUCCESS, Buteo::SyncResults::NO_ERROR));
    emit success(getProfileName(), "Success!!");
}

void CalendarFeedPlugin::syncFailed()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    updateResults(Buteo::SyncResults(QDateTime::currentDateTime(),
                                     Buteo::SyncResults::SYNC_RESULT_FAILED, Buteo::SyncResults::ABORTED));
    emit error(getProfileName(), "Error!!", Buteo::SyncResults::SYNC_RESULT_FAILED);
}

void CalendarFeedPlugin::updateFeed()
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    bool fillWithFuture = m_settings->isFilledWithFuture();
    bool futureLimited = m_settings->isFutureLimited();
    int futureLimit = m_settings->futureLimit();
    QDateTime startDateTime = QDateTime::currentDateTime();
    int displayableCount = m_settings->eventsShown();

    bool useCalendarFilter = m_settings->isCalendarFilterUsed();
    QStringList shownCalendars = m_settings->shownCalendars();
    bool showAlreadyStarted = m_settings->areStartedEventsShown();
    Settings::TodosMode todosMode = m_settings->todosMode();
    bool showNonDatedTodos = m_settings->areNonDatedTodosShown();
    bool showOverdueTodos = m_settings->areOverdueTodosShown();


    QList<CalendarEvent *> eventsToShow;
    QList<CalendarEvent *> todosToShow;

    m_calendarBackend = mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));
    m_calendarStorage = mKCal::ExtendedStorage::Ptr(mKCal::ExtendedCalendar::defaultStorage(m_calendarBackend));

    m_calendarStorage->open();

    m_calendarStorage->loadRecurringIncidences();

    if (todosMode != Settings::DontShowTodos) {
        m_calendarStorage->loadUncompletedTodos();
        if (showNonDatedTodos) {
            KCalCore::Todo::List rawTodos = m_calendarBackend->uncompletedTodos(false, -1);
            foreach(KCalCore::Todo::Ptr todo, rawTodos) {
                if (todo->isCompleted())
                    continue;
                QString calendarUid = m_calendarBackend->notebook(todo->uid());
                mKCal::Notebook::Ptr eventCalendar = m_calendarStorage->notebook(calendarUid);
                if (!eventCalendar || !eventCalendar->isVisible() || !eventCalendar->eventsAllowed())
                    continue;
                if (useCalendarFilter && !shownCalendars.contains(eventCalendar->uid()))
                    continue;
                CalendarEvent *toAdd = new CalendarEvent;
                toAdd->setSummary(todo->summary());
                toAdd->setStartDate(QDateTime::currentDateTime());
                toAdd->setAllDay(true);
                toAdd->setCalendarColor(eventCalendar->color());
                todosToShow << toAdd;
                if (todosToShow.count() >= displayableCount)
                    break;
            }
        }
        QList<CalendarEvent *> overdueTodos;
        if (todosToShow.count() < displayableCount) {
            QDate endTodoDate;
            if (futureLimited)
                endTodoDate = startDateTime.date().addDays(futureLimit);

            KCalCore::Todo::List rawTodos = m_calendarBackend->uncompletedTodos(true, -1);
            foreach(KCalCore::Todo::Ptr todo, rawTodos) {
                if (todo->isCompleted() || !todo->dtDue().isValid())
                    continue;
                if (!showOverdueTodos && todo->dtDue().date() < startDateTime.date())
                    continue;
                if (todosMode == Settings::ShowTodosInSameItem) {
                    if (todo->dtDue().date() >= startDateTime.date())
                        break;
                } else {
                    if (!fillWithFuture && todosToShow.count() && todo->dtDue().date() > startDateTime.date())
                        break;
                    if (futureLimited && todo->dtDue().date() > endTodoDate)
                        break;
                }
                QString calendarUid = m_calendarBackend->notebook(todo->uid());
                mKCal::Notebook::Ptr eventCalendar = m_calendarStorage->notebook(calendarUid);
                if (!eventCalendar || !eventCalendar->isVisible() || !eventCalendar->eventsAllowed())
                    continue;
                if (useCalendarFilter && !shownCalendars.contains(eventCalendar->uid()))
                    continue;
                CalendarEvent *toAdd = new CalendarEvent;
                toAdd->setSummary(todo->summary());
                toAdd->setStartDate(todo->dtDue().toLocalZone().dateTime());
                toAdd->setAllDay(true);
                toAdd->setCalendarColor(eventCalendar->color());
                if (todo->dtDue().date() < startDateTime.date())
                    overdueTodos << toAdd;
                else
                    todosToShow << toAdd;
                if (todosToShow.count() + overdueTodos.count() >= displayableCount)
                    break;
            }
        }
        if (todosMode == Settings::ShowTodosInSameItem) {
            eventsToShow.append(overdueTodos);
            eventsToShow.append(todosToShow);
            todosToShow.clear();
        } else {
            QList<CalendarEvent *> tempTodosList;
            tempTodosList.append(overdueTodos);
            tempTodosList.append(todosToShow);
            todosToShow = tempTodosList;
        }
    }

    QDateTime currentStartDateTime = startDateTime;
    QDateTime maxDateTime = currentStartDateTime.addYears(4);
    int currentPeriod = 7;

    while (eventsToShow.count() < displayableCount) {
        QDate startDate = currentStartDateTime.date().addDays(-1);
        QDate endDate;
        if (fillWithFuture) {
            if (futureLimited) {
                endDate = currentStartDateTime.date().addDays(futureLimit + 1);
            } else {
                endDate = currentStartDateTime.date().addDays(currentPeriod);
            }
        } else {
            endDate = currentStartDateTime.date().addDays(1);
        }

        mKCal::ExtendedCalendar::ExpandedIncidenceList incidenceList =
                expandedIncidences(startDate, endDate);


        foreach(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident,
                incidenceList) {
            KCalCore::Incidence::Ptr incidence = expandedIncident.second;
            QString calendarUid = m_calendarBackend->notebook(incidence);
            mKCal::Notebook::Ptr eventCalendar = m_calendarStorage->notebook(calendarUid);
            QString calendarColor;

            if (!eventCalendar || !eventCalendar->isVisible() || !eventCalendar->eventsAllowed())
                continue;

            if (useCalendarFilter && !shownCalendars.contains(eventCalendar->uid()))
                continue;

            calendarColor = eventCalendar->color();

            if (incidence->type() == KCalCore::IncidenceBase::TypeEvent) {
                KCalCore::Event::Ptr event = incidence.staticCast<KCalCore::Event>();
                QDateTime checkableDateTime = (showAlreadyStarted || event->allDay()) ?
                            expandedIncident.first.dtEnd :
                            expandedIncident.first.dtStart;

                if (expandedIncident.first.dtStart.isValid() &&
                        expandedIncident.first.dtEnd >= expandedIncident.first.dtStart &&
                        checkableDateTime > currentStartDateTime) {
                    CalendarEvent *toAdd = new CalendarEvent;
                    toAdd->setSummary(event->summary());
                    toAdd->setStartDate(expandedIncident.first.dtStart);
                    toAdd->setAllDay(event->allDay());
                    toAdd->setCalendarColor(calendarColor);
                    eventsToShow << toAdd;
                }
            } else if (incidence->type() == KCalCore::IncidenceBase::TypeTodo) {
                if (todosMode != Settings::ShowTodosInSameItem)
                    continue;
                KCalCore::Todo::Ptr todo = incidence.staticCast<KCalCore::Todo>();
                if (!todo->isCompleted() &&
                        todo->dtDue().isValid() &&
                        todo->dtDue().date() >= currentStartDateTime.date()) {
                    CalendarEvent *toAdd = new CalendarEvent;
                    toAdd->setSummary(todo->summary());
                    toAdd->setStartDate(todo->dtDue().dateTime());
                    toAdd->setAllDay(true);
                    toAdd->setCalendarColor(calendarColor);
                    eventsToShow << toAdd;
                }
            }

            if (eventsToShow.count() >= displayableCount)
                break;
        }

        if (!fillWithFuture || futureLimited || currentStartDateTime > maxDateTime) {
            break;
        }

        currentPeriod *= 2;
        currentStartDateTime = QDateTime(endDate, QTime(0, 0));
    }

    addItemsToFeed(eventsToShow, todosToShow);
    qDeleteAll(eventsToShow);
    qDeleteAll(todosToShow);
}

void CalendarFeedPlugin::dbusRequestCompleted(const QDBusMessage &reply)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    int replyId = reply.arguments().first().toInt();
    if(replyId < 0)
        syncFailed();
    else
        syncSuccess();
}

void CalendarFeedPlugin::dbusErrorOccured(const QDBusError &error, const QDBusMessage &message)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    Q_UNUSED(error);
    Q_UNUSED(message);
    syncFailed();
}

void CalendarFeedPlugin::updateResults(const Buteo::SyncResults &results)
{
    QFile file("/home/user/calendarfeed.log");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(Q_FUNC_INFO);
    file.write("\n");
    file.close();
    m_results = results;
    m_results.setScheduled(true);
}

void CalendarFeedPlugin::addItemsToFeed(const QList<CalendarEvent *> &events, const QList<CalendarEvent *> &todos)
{

    QDBusConnection bus = QDBusConnection::sessionBus();

    MEventFeed::instance()->removeItemsBySourceName("SyncFW-calendarfeed");

    QDBusMessage eventsMessage =
            createAddItemMessage(events,
                                 m_locale.translate("", "calendar_feed_item_title"),
                                 QString("com.nokia.Calendar / com.nokia.maemo.meegotouch.CalendarInterface showMonthView %1 %2 %3")
                                 .arg(base64SerializedVariant(firstEventDate.year()))
                                 .arg(base64SerializedVariant(firstEventDate.month()))
                                 .arg(base64SerializedVariant(firstEventDate.day())));
    bus.callWithCallback(eventsMessage, this, SLOT(dbusRequestCompleted(QDBusMessage)), SLOT(dbusErrorOccured(QDBusError,QDBusMessage)));

    if (todos.count()) {
        QDBusMessage todosMessage =
                createAddItemMessage(todos,
                                     m_locale.translate("", "calendar_feed_todos_item_title"),
                                     QString("com.nokia.Calendar / com.nokia.maemo.meegotouch.CalendarInterface showTodoListView"));
        bus.callWithCallback(todosMessage, this, SLOT(dbusRequestCompleted(QDBusMessage)), SLOT(dbusErrorOccured(QDBusError,QDBusMessage)));
    }

}

QDBusMessage CalendarFeedPlugin::createAddItemMessage(const QList<CalendarEvent *> &events, const QString &title, const QString &action)
{
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
            greyOutThisEvent = (startDate > QDate::currentDate()) && highlightToday;
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
        body = m_locale.translate("", "calendar_feed_item_no_events");
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
    itemArgs.insert("title", title);
    itemArgs.insert("icon", icon);
    itemArgs.insert("body", body);
    itemArgs.insert("timestamp", QDateTime::currentDateTime().addDays(1).toString("yyyy-MM-dd hh:mm:ss"));
    itemArgs.insert("sourceName", "SyncFW-calendarfeed");
    itemArgs.insert("sourceDisplayName", m_locale.translate("", "calendar_feed_title"));
    itemArgs.insert("action", action);
    args.append(itemArgs);
    message.setArguments(args);

    return message;
}

mKCal::ExtendedCalendar::ExpandedIncidenceList CalendarFeedPlugin::expandedIncidences(QDate startDate, QDate endDate) {
    m_calendarStorage->load(startDate, endDate);
    KCalCore::Incidence::List incidences = m_calendarBackend->incidences(startDate, endDate);
    return m_calendarBackend->expandRecurrences(&incidences, KDateTime(startDate), KDateTime(endDate));
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
