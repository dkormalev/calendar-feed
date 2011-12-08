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

#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QObject>
#include <QDBusMessage>
#include <QDateTime>

class SettingsHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int refreshInterval READ refreshInterval WRITE setRefreshInterval NOTIFY refreshIntervalChanged)
public:
    explicit SettingsHelper(QObject *parent = 0);

    Q_INVOKABLE QString formatDate(const QDateTime &date, const QString &format);

    int refreshInterval() const;
    void setRefreshInterval(int arg);

signals:
    void errorOccured(const QString &message);

    void refreshIntervalChanged(int arg);

public slots:
    void refreshFeedItem();

protected slots:
    void dbusRequestCompleted(const QDBusMessage &reply);
    void dbusErrorOccured(const QDBusError &error, const QDBusMessage &message);

private:
    int m_refreshInterval;
};

#endif // SETTINGSHELPER_H
