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
import "pages"

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    Component.onCompleted: theme.inverted = true

    MainPage {
        id: mainPage
    }

    function toolBarReturnBack() {
        if (pageStack.depth == 1)
            Qt.quit()
        else
            pageStack.pop()
    }


    ToolBarLayout {
        id: backOnlyToolBarLayout
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: toolBarReturnBack()
        }
    }
}
