#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QStringList>

class Settings
{
public:
    enum TodosMode {
        ShownInSameItem,
        ShownInSeparateItem,
        NotShown
    };

    Settings();

    bool isEnabled() const;
    bool isFilledWithFuture() const;
    bool isFutureLimited() const;
    int futureLimit() const;
    int eventsShown() const;
    bool isCalendarColorShown() const;
    bool isTodayHighlighted() const;
    QString dateFormat() const;
    bool isCalendarFilterUsed() const;
    QStringList shownCalendars() const;
    bool areStartedEventsShown() const;
    TodosMode todosMode() const;
    bool areNonDatedTodosShown() const;
    bool areOverdueTodosShown() const;

    void setEnabled(bool arg);
    void setFilledWithFuture(bool arg);
    void setFutureLimited(bool arg);
    void setFutureLimit(int arg);
    void setEventsShown(int arg);
    void setCalendarColorShown(bool arg);
    void setTodayHighlighted(bool arg);
    void setDateFormat(const QString &arg);
    void setCalendarFilterUsed(bool arg);
    void setShownCalendars(const QStringList &arg);
    void setStartedEventsShown(bool arg);
    void setTodosMode(TodosMode arg);
    void setNonDatedTodosShown(bool arg);
    void setOverdueTodosShown(bool arg);

private:
    QVariant readConfItem(const QString &key, const QVariant &defaultValue);
    void setConfItem(const QString &key, const QVariant &value);

    bool m_enabled;
    bool m_filledWithFuture;
    bool m_futureLimited;
    int m_futureLimit;
    int m_eventsShown;
    bool m_calendarColorShown;
    bool m_todayHighlighted;
    QString m_dateFormat;
    bool m_calendarFilterUsed;
    QStringList m_shownCalendars;
    bool m_startedEventsShown;
    TodosMode m_todosMode;
    bool m_nonDatedTodosShown;
    bool m_overdueTodosShown;
};

#endif // SETTINGS_H
