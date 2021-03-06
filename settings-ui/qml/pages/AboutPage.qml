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

Page {
    id: aboutPage
    orientationLock: PageOrientation.LockPortrait
    tools: backOnlyToolBarLayout

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
                text: "Calendar Feed v0.6.1"
                anchors.horizontalCenter: parent.horizontalCenter
                platformStyle: LabelStyle {
                    fontFamily: UIConstants.FONT_FAMILY
                    fontPixelSize: UIConstants.FONT_XLARGE
                }
            }

            Label{
                //% "Calendar events at Feed screen"
                text: qsTrId("calendar_feed_subtitle")
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                font.italic: true
            }

            Label{
                text: "Copyright © 2011 Denis Kormalev"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
            }

            Label{
                //% "Project page at Gitorious"
                text: qsTrId("calendar_feed_about_gitorious_page") + ":<br /><a style='color:#a040ff' href='https://gitorious.org/calendar-feed/'>https://gitorious.org/calendar-feed/</a>"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label{
                //% "Contact e-mail"
                text: qsTrId("calendar_feed_about_contact_email") + ":<br /><a style='color:#a040ff' href='mailto:kormalev.denis@gmail.com'>kormalev.denis@gmail.com</a>"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label{
                //% "Help page"
                text: qsTrId("calendar_feed_about_help_page") + ": <a style='color:#a040ff' href='https://gitorious.org/calendar-feed/pages/Home'>Wiki</a>"
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
                onLinkActivated: Qt.openUrlExternally(link)
            }

            Label{
                //% "Special thanks to"
                text: qsTrId("calendar_feed_about_thanks_to") + ": Andrew Babkin, Valentin Brault, Krzysztof Drozd, Alexey Koniahin, Krzysztof Krzewniak, Milko Mati, Andri Oskarsson, Gerolf Reinwardt, Marko Vertainen, MeeGo Turkey community"
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
