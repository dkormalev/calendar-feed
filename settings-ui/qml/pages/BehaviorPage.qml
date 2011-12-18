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

import QtQuick 1.1
import com.nokia.meego 1.0
import "../logic/UIConstants.js" as UIConstants
import "../elements"

Page {
    id: behaviorPage
    orientationLock: PageOrientation.LockPortrait
    tools: backOnlyToolBarLayout

    anchors.fill: parent

    Flickable {
        id: flickableWrapper
        anchors.fill: parent
        anchors.leftMargin: UIConstants.DEFAULT_MARGIN
        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
        flickableDirection: Flickable.VerticalFlick
        contentWidth: width
        contentHeight: content.height

        Column {
            id: content
            width: parent.width

            PageTitle {
                width: parent.width
                //% "Behavior"
                title: qsTrId("calendar_feed_setting_group_behavior")
            }

            SettingsGroup {
                width: parent.width
                //% "Calendar Feed"
                title: qsTrId("calendar_feed_title")
            }

            SwitchSetting {
                width: parent.width
                //% "Fill with Future Events"
                label: qsTrId("calendar_feed_setting_fill_with_future")
                key: "/apps/ControlPanel/CalendarFeed/FillWithFuture"
                defaultValue: false
            }

            SwitchableSliderSetting {
                width: parent.width
                //% "Limit Future Events (days)"
                label: qsTrId("calendar_feed_setting_limit_future")
                switchKey: "/apps/ControlPanel/CalendarFeed/LimitFuture"
                defaultSwitchValue: true
                key: "/apps/ControlPanel/CalendarFeed/LimitDaysSize"
                defaultValue: 7
                maxValue: 31
                minValue: 1
            }

            SwitchSetting {
                width: parent.width
                //% "Show Started Events"
                label: qsTrId("calendar_feed_setting_show_started_events")
                key: "/apps/ControlPanel/CalendarFeed/ShowStartedEvents"
                defaultValue: true
            }

            SwitchSetting {
                id: filterCalendarsSetting
                width: parent.width
                //% "Filter Calendars"
                label: qsTrId("calendar_feed_setting_filter_calendars")
                key: "/apps/ControlPanel/CalendarFeed/FilterCalendars"
                defaultValue: false
            }

            MultiSelectionSetting {
                width: parent.width
                //% "Calendars Shown"
                label: qsTrId("calendar_feed_setting_calendars_shown")
                //% "Select Calendars"
                dialogTitle: qsTrId("calendar_feed_setting_calendars_shown_dialog_title")
                key: "/apps/ControlPanel/CalendarFeed/CalendarsShown"
                defaultValue: ""
                model: settingsHelper.calendars()
                enabled: filterCalendarsSetting.checked
            }

            SliderElement {
                width: parent.width
                //% "Refresh Interval (minutes)"
                label: qsTrId("calendar_feed_setting_refresh_interval")
                maxValue: 60
                minValue: 1
                value: settingsHelper.refreshInterval
                onValueChanged: settingsHelper.refreshInterval = value
            }

        }

    }


}
