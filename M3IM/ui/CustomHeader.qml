import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: root

    // content properties
    property alias text: headerLabel.text

    // size settings
    width: parent.width
    height: 60

    Label {
        id: headerLabel

        // anchors settings
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        padding: 10

        // size settings
        width: parent.width
        height: parent.height


        // background settings
        background: Rectangle {
            radius: 0
            color: "white"

            // border settings
            border {
                width: 1
                color: "whitesmoke"
            }
        }

        // content settings
        font {
            pixelSize: Qt.application.font.pixelSize * 2
        }
        horizontalAlignment: Text.Center
        verticalAlignment: Text.Center
    }

}
