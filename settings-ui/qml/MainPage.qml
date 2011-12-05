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
import "UIConstants.js" as UIConstants

Page {
    function toolBarReturnBack() {
        if (pageStack.depth == 1)
            Qt.quit()
        else
            pageStack.pop()
    }

    id: mainPage
    tools: ToolBarLayout {
        id: fullToolBarLayout
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: toolBarReturnBack()
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (toolBarMenu.status == DialogStatus.Closed) ? toolBarMenu.open() : toolBarMenu.close()
        }
    }

    Menu {
        id: toolBarMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                //% "Refresh Feed Item"
                text: qsTrId("calendar_feed_refresh")
                onClicked: settingsHelper.refreshFeedItem()
            }
            MenuItem {
                //% "About"
                text: qsTrId("calendar_feed_about")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    Connections {
        target: settingsHelper
        onErrorOccured: {
            errorLabel.text = message
            errorContainer.visible = true
        }
    }

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
                //% "Calendar Feed"
                title: qsTrId("calendar_feed_title")
            }

            SettingsGroup {
                width: parent.width
                //% "Behavior"
                title: qsTrId("calendar_feed_setting_group_behavior")
            }

            SwitchSetting {
                id: enableFeedSetting
                width: parent.width
                //% "Publish to Feed"
                label: qsTrId("calendar_feed_setting_publish_to_feed")
                key: "/apps/ControlPanel/CalendarFeed/EnableFeed"
                defaultValue: true
            }

            SwitchSetting {
                width: parent.width
                //% "Fill with Future Events"
                label: qsTrId("calendar_feed_setting_fill_with_future")
                key: "/apps/ControlPanel/CalendarFeed/FillWithFuture"
                defaultValue: false
                enabled: enableFeedSetting.checked
            }

            SwitchableSliderSetting {
                width: parent.width
                //% "Limit Future Events"
                label: qsTrId("calendar_feed_setting_limit_future")
                switchKey: "/apps/ControlPanel/CalendarFeed/LimitFuture"
                defaultSwitchValue: true
                key: "/apps/ControlPanel/CalendarFeed/LimitDaysSize"
                defaultValue: 7
                maxValue: 31
                minValue: 1
                enabled: enableFeedSetting.checked
            }

            SettingsGroup {
                width: parent.width
                //% "Appearance"
                title: qsTrId("calendar_feed_setting_group_display")
            }

            SliderSetting {
                width: parent.width
                //% "Events Shown"
                label: qsTrId("calendar_feed_setting_events_number")
                key: "/apps/ControlPanel/CalendarFeed/FeedSize"
                defaultValue: 3
                maxValue: 5
                minValue: 1
                enabled: enableFeedSetting.checked
            }

            SwitchSetting {
                width: parent.width
                //% "Show Calendar Color"
                label: qsTrId("calendar_feed_setting_show_calendar_bar")
                key: "/apps/ControlPanel/CalendarFeed/ShowCalendarBar"
                defaultValue: false
                enabled: enableFeedSetting.checked
            }

            SwitchSetting {
                width: parent.width
                //% "Highlight Today Events"
                label: qsTrId("calendar_feed_setting_highlight_today")
                key: "/apps/ControlPanel/CalendarFeed/HighlightToday"
                defaultValue: false
                enabled: enableFeedSetting.checked
            }

            SelectionSetting {
                width: parent.width
                //% "Date Format"
                label: qsTrId("calendar_feed_setting_date_format")
                dialogTitle: qsTrId("calendar_feed_setting_date_format")
                key: "/apps/ControlPanel/CalendarFeed/DateFormat"
                defaultValue: "MMM, d"
                model: [{"name": settingsHelper.formatDate(new Date(), "MMM, d"), "value": "MMM, d"},
                    {"name": settingsHelper.formatDate(new Date(), "dd-MM"), "value": "dd-MM"},
                    {"name": settingsHelper.formatDate(new Date(), "dd.MM"), "value": "dd.MM"}]
                enabled: enableFeedSetting.checked
            }

            TextFieldSetting {
                width: parent.width
                //% "Date Format"
                label: qsTrId("calendar_feed_setting_date_format")
                key: "/apps/ControlPanel/CalendarFeed/DateFormat"
                defaultValue: "MMM, d"
                enabled: enableFeedSetting.checked
            }

        }

    }

    Item {
        id: errorContainer
        anchors.fill: flickableWrapper
        visible: false

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: (parent.visible) ? 0.0 : 0.4
        }

        Label {
            id: errorLabel
            font.italic: true
            anchors.centerIn: parent
            style: LabelStyle {
                textColor: "#ee4444"
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: parent.visible
        }
    }

}
