import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UIConstants

Item {
    property alias title: titleLabel.text
    property alias description: descriptionLabel.text

    height: descriptionLabel.height + descriptionLabel.y

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
        color: "#333333"
    }

    Label {
        id: descriptionLabel
        anchors.top: titleDivider.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: UIConstants.DEFAULT_MARGIN
    }
}

