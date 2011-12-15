#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>

class Settings
{

public:
    Settings();

    bool isEnabled() const;
    bool isFilledWithFuture() const;
    bool isFutureLimited() const;
    int futureLimit() const;
    int eventsShown() const;
    bool isCalendarColorShown() const;
    bool isTodayHighlighted() const;
    QString dateFormat() const;
    void setEnabled(bool arg);
    void setFilledWithFuture(bool arg);
    void setFutureLimited(bool arg);
    void setFutureLimit(int arg);
    void setEventsShown(int arg);
    void setCalendarColorShown(bool arg);
    void setTodayHighlighted(bool arg);
    void setDateFormat(const QString &arg);

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

};

#endif // SETTINGS_H
