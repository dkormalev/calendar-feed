#include "calendarevent.h"

CalendarEvent::CalendarEvent()
{
}

QDateTime CalendarEvent::startDate() const
{
    return m_startDate;
}

QString CalendarEvent::summary() const
{
    return m_summary;
}

QString CalendarEvent::calendarColor() const
{
    return m_calendarColor;
}

bool CalendarEvent::isAllDay() const
{
    return m_allDay;
}

void CalendarEvent::setStartDate(const QDateTime &arg)
{
    m_startDate = arg;
}

void CalendarEvent::setSummary(const QString &arg)
{
    m_summary = arg;
}

void CalendarEvent::setCalendarColor(const QString &arg)
{
    m_calendarColor = arg;
}

void CalendarEvent::setAllDay(bool arg)
{
    m_allDay = arg;
}
