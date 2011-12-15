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

#ifndef CALENDARFEEDPLUGIN_H
#define CALENDARFEEDPLUGIN_H

#include <libsyncpluginmgr/ClientPlugin.h>
#include <libsyncprofile/SyncResults.h>
#include <QDBusMessage>
#include <QList>

class Settings;
class CalendarEvent;

class CalendarFeedPlugin : public Buteo::ClientPlugin
{
    Q_OBJECT
public:
    CalendarFeedPlugin( const QString &pluginName,
                         const Buteo::SyncProfile &profile,
                         Buteo::PluginCbInterface *cbInterface );

    virtual ~CalendarFeedPlugin();
    virtual bool init();
    virtual bool uninit();
    virtual bool startSync();
    virtual void abortSync(Sync::SyncStatus status = Sync::SYNC_ABORTED);
    virtual Buteo::SyncResults getSyncResults() const;
    virtual bool cleanUp();

public slots:
    virtual void connectivityStateChanged( Sync::ConnectivityType type,
                                           bool state );

protected slots:
    void syncSuccess();
    void syncFailed();
    void updateFeed();
    void dbusRequestCompleted(const QDBusMessage &reply);
    void dbusErrorOccured(const QDBusError &error, const QDBusMessage &message);

private:
    void updateResults(const Buteo::SyncResults &results);
    QString base64SerializedVariant(const QVariant &value) const;

    Buteo::SyncResults m_results;
    Settings *m_settings;
};

extern "C" CalendarFeedPlugin* createPlugin( const QString& pluginName,
                                              const Buteo::SyncProfile &profile,
                                              Buteo::PluginCbInterface *cbInterface );

extern "C" void destroyPlugin( CalendarFeedPlugin *client );

#endif // CALENDARFEEDPLUGIN_H
