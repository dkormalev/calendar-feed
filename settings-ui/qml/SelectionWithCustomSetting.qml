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
    property variant model: []
    property alias label: settingLabel.text
    property alias dialogTitle: selectionDialog.titleText
    property alias value: gconfItem.value
    property alias key: gconfItem.key
    property alias defaultValue: gconfItem.defaultValue

    property bool loaded: false

    Component.onCompleted: {
        gconfItem.updateValue()
        var isCustom = true
        var i
        for (i = 0; i < model.length; ++i) {
            selectionDialog.model.append(model[i])
            if (model[i].value == value) {
                selectionDialog.selectedIndex = i
                valueLabel.text = model[i].name
                isCustom = false
            }
        }
        //% "Custom"
        selectionDialog.model.append({"name": qsTrId("calendar_feed_setting_custom_selection"), "value": ""})
        if (isCustom) {
            selectionDialog.selectedIndex = i
            valueLabel.text = selectionDialog.model.get(i).name
        }

        loaded = true
    }

    id: setting
    height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT + customPart.height + UIConstants.DEFAULT_MARGIN*2

    Item {
        property bool customSelected: selectionDialog.selectedIndex === selectionDialog.model.count-1

        id: selectionPart
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: UIConstants.DEFAULT_MARGIN
        height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT

        SelectionDialog {
            id: selectionDialog
            model: ListModel {}
            onSelectedIndexChanged: {
                if (loaded) {
                    if (selectedIndex != selectionDialog.model.count-1) {
                        gconfItem.value = selectionDialog.model.get(selectedIndex).value
                    }
                    valueLabel.text = selectionDialog.model.get(selectedIndex).name
                }
            }
        }


        GConfItem {
            id: gconfItem
        }

        Rectangle {
            id: background
            anchors.fill: parent
            color: "white"
            opacity: mouseArea.pressed ? 0.25 : 0.0
        }

        Label {
            id: settingLabel
            anchors.bottom: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: selectionImage.left
            anchors.rightMargin: UIConstants.DEFAULT_MARGIN
            font.bold: true
            clip: true

            style: LabelStyle {
                textColor: (setting.enabled) ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                fontFamily: UIConstants.FONT_FAMILY
                fontPixelSize: UIConstants.FONT_SLARGE
            }
        }

        Label {
            id: valueLabel
            anchors.top: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: selectionImage.left
            anchors.rightMargin: UIConstants.DEFAULT_MARGIN
            clip: true

            style: LabelStyle {
                textColor: UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                fontFamily: UIConstants.FONT_FAMILY
                fontPixelSize: UIConstants.FONT_SMALL
            }
        }

        Image {
            id: selectionImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "image://theme/icon-m-textinput-combobox-arrow"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            enabled: setting.enabled
            onClicked: selectionDialog.open()
        }
    }

    TextField {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: selectionPart.bottom
        id: customPart
        enabled: setting.enabled && selectionPart.customSelected
        text: selectionPart.customSelected ? setting.value : ""
        onTextChanged: {
            if (loaded && selectionPart.customSelected)
                gconfItem.value = text
        }

    }
}
