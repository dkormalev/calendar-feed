import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UIConstants

Page {
    id: mainPage
    tools: commonToolBarLayout
    anchors.fill: parent

    Flickable {
        anchors.fill: parent
        anchors.leftMargin: UIConstants.DEFAULT_MARGIN
        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
        flickableDirection: Flickable.VerticalFlick
        contentWidth: width
        contentHeight: content.height

        Column {
            id: content
            width: mainPage.width

            PageTitle {
                width: parent.width
                title: qsTrId("calendar_feed_title")
                description: qsTrId("calendar_feed_subtitle")
            }

            SwitchSetting {
                width: parent.width
                label: qsTrId("calendar_feed_setting_publish_to_feed")
                key: "/apps/ControlPanel/CalendarFeed/EnableFeed"
                defaultValue: true
            }

            SwitchSetting {
                width: parent.width
                label: qsTrId("calendar_feed_setting_show_calendar_bar")
                key: "/apps/ControlPanel/CalendarFeed/ShowCalendarBar"
                defaultValue: true
            }

//            SwitchItem {
//                label: qsTrId("qtn_enable_event_feeds")
//                checked: false
//            }

        }
    }


}
