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
import CalendarFeed 1.0

Item {
    property alias label: settingLabel.text
    property alias checked: settingControl.checked
    property alias key: gconfItem.key
    property alias defaultValue: gconfItem.defaultValue

    property bool loaded: false

    Component.onCompleted: loaded = true

    id: setting
    height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT

    GConfItem {
        id: gconfItem
    }

    Label {
        id: settingLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: settingControl.left
        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
        wrapMode: Text.Wrap
        font.bold: true
        height: (paintedHeight > parent.height) ? parent.height : paintedHeight
        clip: true

        style: LabelStyle {
            textColor: (setting.enabled) ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
            fontFamily: UIConstants.FONT_FAMILY
            fontPixelSize: UIConstants.FONT_SLARGE
        }

    }

    Switch {
        id: settingControl
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onCheckedChanged: {
            if (loaded)
                gconfItem.value = checked
        }
        checked: gconfItem.value
        enabled: parent.enabled
    }
}
