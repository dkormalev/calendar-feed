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

Item {
    property alias label: titleLabel.text

    signal activated()

    height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT

    id: labelPageActivator

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
        opacity: mouseArea.pressed ? 0.25 : 0.0
    }

    Label {
        id: titleLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: activatorImage.left
        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
        font.bold: true
        clip: true
        wrapMode: Text.Wrap
        height: parent.height
        verticalAlignment: Text.AlignVCenter

        style: LabelStyle {
            textColor: (labelPageActivator.enabled) ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
            fontFamily: UIConstants.FONT_FAMILY
            fontPixelSize: UIConstants.FONT_SLARGE
        }
    }

    Image {
        id: activatorImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        source: "image://theme/icon-m-common-drilldown-arrow-inverse"
        opacity: (labelPageActivator.enabled) ? 1.0 : 0.5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: labelPageActivator.enabled
        onClicked: activated()
    }

}

