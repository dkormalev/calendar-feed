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

    id: aboutPage
    tools: ToolBarLayout {
        id: backOnlyToolBarLayout
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: toolBarReturnBack()
        }
    }

    anchors.fill: parent

    Flickable{
        id: flick
        anchors.fill: parent
        anchors.leftMargin: UIConstants.DEFAULT_MARGIN
        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
        flickableDirection: Flickable.VerticalFlick
        contentWidth: width
        contentHeight: content.height

        Column{
            id: content
            width: parent.width
            spacing: UIConstants.DEFAULT_MARGIN
            Image{
                source: "qrc:/calendar-feed-icon_256.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label{
                //% "Calendar Feed"
                text: qsTrId("calendar_feed_title")
                anchors.horizontalCenter: parent.horizontalCenter
                platformStyle: LabelStyle {
                    fontFamily: UIConstants.FONT_FAMILY
                    fontPixelSize: UIConstants.FONT_XLARGE
                }
            }

            Label{
                text: "Copyright © 2011 Denis Kormalev &lt;<font color='#7010cf'><a href='mailto:?kormalev.denis@gmail.com'>kormalev.denis@gmail.com</a></font>&gt;"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label{
                text: "<font color='#7010cf'><a href='https://gitorious.org/calendar-feed/'>Project page at Gitorious</a></font>"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label{
                //% "Special thanks to"
                text: qsTrId("calendar_feed_about_thanks_to") + ": Andrew Babkin, Bruno Calendreau, Milko Mati, Andri Óskarsson, Gerolf Reinwardt, Antti Siiskonen"
                wrapMode: Text.WordWrap
                width: parent.width
            }
        }
    }
    ScrollDecorator {
        flickableItem: flick
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
    }
}
