#include "settings.h"
#include <gconfitem.h>

Settings::Settings()
{
    m_enabled = readConfItem("/apps/ControlPanel/CalendarFeed/EnableFeed", true).toBool();
    m_filledWithFuture = readConfItem("/apps/ControlPanel/CalendarFeed/FillWithFuture", false).toBool();
    m_futureLimited = readConfItem("/apps/ControlPanel/CalendarFeed/LimitFuture", false).toBool();
    m_futureLimit = readConfItem("/apps/ControlPanel/CalendarFeed/LimitDaysSize", 7).toInt();
    m_eventsShown = readConfItem("/apps/ControlPanel/CalendarFeed/FeedSize", 3).toInt();
    m_calendarColorShown = readConfItem("/apps/ControlPanel/CalendarFeed/ShowCalendarBar", false).toBool();
    m_todayHighlighted = readConfItem("/apps/ControlPanel/CalendarFeed/HighlightToday", false).toBool();
    m_dateFormat = readConfItem("/apps/ControlPanel/CalendarFeed/DateFormat", "MMM, d").toString();

    m_calendarFilterUsed = readConfItem("/apps/ControlPanel/CalendarFeed/FilterCalendars", false).toBool();
    m_shownCalendars = readConfItem("/apps/ControlPanel/CalendarFeed/CalendarsShown", "").toString().split(";");
    m_startedEventsShown = readConfItem("/apps/ControlPanel/CalendarFeed/ShowStartedEvents", true).toBool();
    m_todosMode = (TodosMode) readConfItem("/apps/ControlPanel/CalendarFeed/TodosMode", ShowTodosInSameItem).toInt();
    m_nonDatedTodosShown = readConfItem("/apps/ControlPanel/CalendarFeed/ShowNonDatedTodos", false).toBool();
    m_overdueTodosShown = readConfItem("/apps/ControlPanel/CalendarFeed/ShowOverdueTodos", false).toBool();
}

bool Settings::isEnabled() const
{
    return m_enabled;
}

bool Settings::isFilledWithFuture() const
{
    return m_filledWithFuture;
}

bool Settings::isFutureLimited() const
{
    return m_futureLimited;
}

int Settings::futureLimit() const
{
    return m_futureLimit;
}

int Settings::eventsShown() const
{
    return m_eventsShown;
}

bool Settings::isCalendarColorShown() const
{
    return m_calendarColorShown;
}

bool Settings::isTodayHighlighted() const
{
    return m_todayHighlighted;
}

QString Settings::dateFormat() const
{
    return m_dateFormat;
}

bool Settings::isCalendarFilterUsed() const
{
    return m_calendarFilterUsed;
}

QStringList Settings::shownCalendars() const
{
    return m_shownCalendars;
}

bool Settings::areStartedEventsShown() const
{
    return m_startedEventsShown;
}

Settings::TodosMode Settings::todosMode() const
{
    return m_todosMode;
}

bool Settings::areNonDatedTodosShown() const
{
    return m_nonDatedTodosShown;
}

bool Settings::areOverdueTodosShown() const
{
    return m_overdueTodosShown;
}


void Settings::setEnabled(bool arg)
{
    m_enabled = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/EnableFeed", m_enabled);
}

void Settings::setFilledWithFuture(bool arg)
{
    m_filledWithFuture = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/FillWithFuture", m_filledWithFuture);
}

void Settings::setFutureLimited(bool arg)
{
    m_futureLimited = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/LimitFuture", m_futureLimited);
}

void Settings::setFutureLimit(int arg)
{
    m_futureLimit = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/LimitDaysSize", m_futureLimit);
}

void Settings::setEventsShown(int arg)
{
    m_eventsShown = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/FeedSize", m_eventsShown);
}

void Settings::setCalendarColorShown(bool arg)
{
    m_calendarColorShown = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/ShowCalendarBar", m_calendarColorShown);
}

void Settings::setTodayHighlighted(bool arg)
{
    m_todayHighlighted = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/HighlightToday", m_todayHighlighted);
}

void Settings::setDateFormat(const QString &arg)
{
    m_dateFormat = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/DateFormat", m_dateFormat);
}

void Settings::setCalendarFilterUsed(bool arg)
{
    m_calendarFilterUsed = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/FilterCalendars", m_calendarFilterUsed);
}

void Settings::setShownCalendars(const QStringList &arg)
{
    m_shownCalendars = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/CalendarsShown", m_shownCalendars.join(";"));
}

void Settings::setStartedEventsShown(bool arg)
{
    m_startedEventsShown = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/ShowStartedEvents", m_startedEventsShown);
}

void Settings::setTodosMode(TodosMode arg)
{
    m_todosMode = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/TodosMode", m_todosMode);
}

void Settings::setNonDatedTodosShown(bool arg)
{
    m_nonDatedTodosShown = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/ShowNonDatedTodos", m_nonDatedTodosShown);
}

void Settings::setOverdueTodosShown(bool arg)
{
    m_overdueTodosShown = arg;
    setConfItem("/apps/ControlPanel/CalendarFeed/ShowOverdueTodos", m_overdueTodosShown);
}


void Settings::setConfItem(const QString &key, const QVariant &value)
{
    GConfItem confItem(key);
    confItem.set(value);
}

QVariant Settings::readConfItem(const QString &key, const QVariant &defaultValue)
{
    GConfItem confItem(key);
    QVariant variant = confItem.value();
    if (variant.isValid())
        return variant;
    confItem.set(defaultValue);
    return defaultValue;
}
