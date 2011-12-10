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
import CalendarFeed 1.0

Item {
    property alias label: settingLabel.text
    property alias value: settingControl.text
    property alias key: gconfItem.key
    property alias defaultValue: gconfItem.defaultValue

    property bool loaded: false

    Component.onCompleted: loaded = true

    id: setting
    height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT*1.2

    GConfItem {
        id: gconfItem
    }

    Label {
        id: settingLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        font.bold: true
        clip: true

        style: LabelStyle {
            textColor: (setting.enabled) ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
            fontFamily: UIConstants.FONT_FAMILY
            fontPixelSize: UIConstants.FONT_SLARGE
        }

    }

    TextField {
        id: settingControl
        anchors.top: settingLabel.bottom
        anchors.topMargin: UIConstants.DEFAULT_MARGIN/2
        anchors.left: parent.left
        anchors.right: parent.right
        onTextChanged: {
            if (loaded)
                gconfItem.value = text
        }
        text: gconfItem.value
        enabled: parent.enabled
    }
}
