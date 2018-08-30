import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: root

    // content properties
    property alias text: input.text
    property alias textColor: input.color
    property alias textFont: input.font
    property alias textHorizontalAlignment: input.horizontalAlignment
    property alias textVerticalAlignment: input.verticalAlignment

    // background properties
    property alias radius: background.radius
    property alias borderWidth: backgroundBorder.width
    property alias borderColor: backgroundBorder.color
    property alias backgroundColor: background.color

    // styled round button
    TextField {
        id: input

        // size settings
        width: 145
        height: 40

        // background settings
        background: Rectangle {
            id: background

            radius: 8
            color: "white"

            // size settings
            width: parent.width
            height: parent.height

            // border settings
            border {
                id: backgroundBorder
                width: 0
                color: "#87cefa"
            }
        }

        // content settings
        text: ""
        horizontalAlignment: Text.Center
        verticalAlignment: Text.Center
    }

    Label {
        id: separator

        // anchors settings
        anchors {
            top: background.bottom
        }

        leftPadding: -5
        padding: 20

        // size settings
        width: input.width

        color: "lightgray"
        text: "_".repeat(28)
    }
}
