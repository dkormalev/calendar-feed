#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QString>
#include <QDateTime>

class CalendarEvent
{
public:
    explicit CalendarEvent();

    QDateTime startDate() const;
    QString summary() const;
    QString calendarColor() const;
    bool isAllDay() const;
    void setStartDate(const QDateTime &arg);
    void setSummary(const QString &arg);
    void setCalendarColor(const QString &arg);
    void setAllDay(bool arg);

private:
    QDateTime m_startDate;
    QString m_summary;
    QString m_calendarColor;
    bool m_allDay;
};

#endif // CALENDAREVENT_H
