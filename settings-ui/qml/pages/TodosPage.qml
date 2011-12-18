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
                //% "To-dos"
                title: qsTrId("calendar_feed_setting_group_todos")
            }

            SettingsGroup {
                width: parent.width
                //% "Calendar Feed"
                title: qsTrId("calendar_feed_title")
            }

            SelectionSetting {
                id: todosDisplayMode
                width: parent.width
                //% "To-dos Display Mode"
                label: qsTrId("calendar_feed_setting_todos_display_mode")
                dialogTitle: qsTrId("calendar_feed_setting_todos_display_mode")
                key: "/apps/ControlPanel/CalendarFeed/TodosMode"
                defaultValue: 0
                model:
                    //% "Show in same item"
                    [{"name": qsTrId("calendar_feed_setting_todos_display_mode_same_item"), "value": 0},
                    //% "Show in separate item"
                    {"name": qsTrId("calendar_feed_setting_todos_display_mode_separate_item"), "value": 1},
                    //% "Don't show"
                    {"name": qsTrId("calendar_feed_setting_todos_display_mode_nothing"), "value": 2}]
            }

            SwitchSetting {
                width: parent.width
                //% "Show Overdue To-dos"
                label: qsTrId("calendar_feed_setting_show_overdue_todos")
                key: "/apps/ControlPanel/CalendarFeed/ShowOverdueTodos"
                defaultValue: false
                enabled: todosDisplayMode.value != 2
            }

            SwitchSetting {
                width: parent.width
                //% "Show To-dos Without Due Date"
                label: qsTrId("calendar_feed_setting_show_nondated_todos")
                key: "/apps/ControlPanel/CalendarFeed/ShowNonDatedTodos"
                defaultValue: false
                enabled: todosDisplayMode.value != 2
            }

        }

    }


}
