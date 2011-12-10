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
    id: appearancePage
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
                //% "Appearance"
                title: qsTrId("calendar_feed_setting_group_display")
            }

            SettingsGroup {
                width: parent.width
                //% "Calendar Feed"
                title: qsTrId("calendar_feed_title")
            }


            SliderSetting {
                width: parent.width
                //% "Events Shown"
                label: qsTrId("calendar_feed_setting_events_number")
                key: "/apps/ControlPanel/CalendarFeed/FeedSize"
                defaultValue: 3
                maxValue: 5
                minValue: 1
            }

            SwitchSetting {
                width: parent.width
                //% "Show Calendar Color"
                label: qsTrId("calendar_feed_setting_show_calendar_bar")
                key: "/apps/ControlPanel/CalendarFeed/ShowCalendarBar"
                defaultValue: false
            }

            SwitchSetting {
                width: parent.width
                //% "Highlight Today Events"
                label: qsTrId("calendar_feed_setting_highlight_today")
                key: "/apps/ControlPanel/CalendarFeed/HighlightToday"
                defaultValue: false
            }

            SelectionWithCustomSetting {
                width: parent.width
                //% "Date Format"
                label: qsTrId("calendar_feed_setting_date_format")
                dialogTitle: qsTrId("calendar_feed_setting_date_format")
                key: "/apps/ControlPanel/CalendarFeed/DateFormat"
                defaultValue: "MMM, d"
                model: [{"name": settingsHelper.formatDate(new Date(), "MMM, d"), "value": "MMM, d"},
                    {"name": settingsHelper.formatDate(new Date(), "dd MMM"), "value": "d MMM"},
                    {"name": settingsHelper.formatDate(new Date(), "dd-MM"), "value": "dd-MM"},
                    {"name": settingsHelper.formatDate(new Date(), "MM/dd"), "value": "MM/dd"},
                    {"name": settingsHelper.formatDate(new Date(), "dd.MM"), "value": "dd.MM"}]
            }

        }

    }


}
