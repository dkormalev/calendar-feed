import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    Component.onCompleted: theme.inverted = true

    MainPage {
        id: mainPage
    }

    ToolBarLayout {
        id: commonToolBarLayout
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: Qt.quit()
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (toolBarMenu.status == DialogStatus.Closed) ? toolBarMenu.open() : toolBarMenu.close()
        }
    }

    Menu {
        id: toolBarMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTrId("qtn_about")
                onClicked: pageStack.push("AboutPage.qml")
            }
        }
    }
}
