import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: root

    // content properties
    property alias text: content.text
    property alias textColor: content.color
    property alias textFont: content.font
    property alias textHorizontalAlignment: content.horizontalAlignment
    property alias textVerticalAlignment: content.verticalAlignment

    // background properties
    property alias radius: background.radius
    property alias borderWidth: backgroundBorder.width
    property alias borderColor: backgroundBorder.color
    property alias backgroundColor: background.color

    // signals
    signal clicked()

    // styled round button
    RoundButton {
        id: roundButton

        // anchors settings
        anchors {
            fill: parent
        }

        // background settings
        background: Rectangle {
            id: background

            radius: 8
            color: "#87cefa"
            border {
                id: backgroundBorder

                width: 2
                color: "lightgray"
            }
        }

        // text settings
        contentItem: Text {
            id: content

            text: "Conent Text"
            color: "white"

            horizontalAlignment: Text.Center
            verticalAlignment: Text.Center
        }

        // slots
        onClicked: {
            root.clicked()
        }
    }
}
