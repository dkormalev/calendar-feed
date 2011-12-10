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
    property int value: 0
    property alias maxValue: settingControl.maximumValue
    property alias minValue: settingControl.minimumValue

    property bool loaded: false

    Component.onCompleted: {
        if (value < minValue)
            value = minValue
        if (value > maxValue)
            value = maxValue
        settingControl.value = value
        loaded = true
    }

    id: setting
    height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT*1.5
    clip: true

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

    Label {
        id: minValueLabel
        anchors.left: parent.left
        anchors.verticalCenter: settingControl.verticalCenter
        text: minValue
        style: LabelStyle {
            textColor: "#606060"
            fontFamily: UIConstants.FONT_FAMILY_LIGHT
            fontPixelSize: UIConstants.FONT_LSMALL
        }
    }

    Label {
        id: maxValueLabel
        anchors.right: parent.right
        anchors.verticalCenter: settingControl.verticalCenter
        text: maxValue
        style: LabelStyle {
            textColor: "#606060"
            fontFamily: UIConstants.FONT_FAMILY_LIGHT
            fontPixelSize: UIConstants.FONT_LSMALL
        }
    }

    Slider {
        id: settingControl
        anchors.top: settingLabel.bottom
        anchors.right: maxValueLabel.left
        anchors.left: minValueLabel.right
        anchors.margins: UIConstants.DEFAULT_MARGIN
        onPressedChanged: {
            if (loaded && !pressed && value != setting.value)
                setting.value = value

        }
        stepSize: 1
        valueIndicatorVisible: true
        enabled: parent.enabled
    }

}
