import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UIConstants
import CalendarFeed 1.0

Item {
    property alias label: settingLabel.text
    property alias checked: settingControl.checked
    property alias key: gconfItem.key
    property alias defaultValue: gconfItem.defaultValue

    id: switchItem
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
    }

    Switch {
        id: settingControl
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        onCheckedChanged: gconfItem.value = checked
        checked: gconfItem.value
    }
}
