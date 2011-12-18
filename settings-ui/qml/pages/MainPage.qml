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
    id: mainPage
    orientationLock: PageOrientation.LockPortrait
    tools: backOnlyToolBarLayout

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

            SwitchSetting {
                id: enableFeedSetting
                width: parent.width
                //% "Publish to Feed"
                label: qsTrId("calendar_feed_setting_publish_to_feed")
                key: "/apps/ControlPanel/CalendarFeed/EnableFeed"
                defaultValue: true
            }

            LabelPageActivator {
                width: parent.width
                //% "Behavior"
                label: qsTrId("calendar_feed_setting_group_behavior")
                onActivated: pageStack.push(Qt.resolvedUrl("BehaviorPage.qml"))
                enabled: enableFeedSetting.checked
            }

            LabelPageActivator {
                width: parent.width
                //% "Appearance"
                label: qsTrId("calendar_feed_setting_group_display")
                onActivated: pageStack.push(Qt.resolvedUrl("AppearancePage.qml"))
                enabled: enableFeedSetting.checked
            }

            LabelPageActivator {
                width: parent.width
                //% "To-dos"
                label: qsTrId("calendar_feed_setting_group_todos")
                onActivated: pageStack.push(Qt.resolvedUrl("TodosPage.qml"))
                enabled: enableFeedSetting.checked
            }

            ButtonElement {
                width: parent.width
                //% "Refresh Feed Item"
                text: qsTrId("calendar_feed_refresh")
                onClicked: settingsHelper.refreshFeedItem()
                enabled: enableFeedSetting.checked
            }

            LabelPageActivator {
                width: parent.width
                //% "About"
                label: qsTrId("calendar_feed_about")
                onActivated: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
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
