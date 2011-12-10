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
    property alias title: titleLabel.text

    height: titleDivider.height + titleDivider.y

    id: pageTitle
    Label {
        id: titleLabel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: UIConstants.HEADER_DEFAULT_HEIGHT_PORTRAIT
        verticalAlignment: Text.AlignVCenter

        style: LabelStyle {
            fontFamily: UIConstants.FONT_FAMILY_LIGHT
            fontPixelSize: UIConstants.FONT_XLARGE
        }
    }

    Rectangle {
        id: titleDivider
        anchors.top: titleLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 1
        height: 2
        color: "#505050"
    }

}

