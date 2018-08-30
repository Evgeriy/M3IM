import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: root

    // content properties
    property alias text: label.text
    property alias font: label.font

    // size settings
    width: label.paintedWidth
    height: label.paintedHeight

    Label {
        id: label

        // anchors settings
        anchors {
            fill: parent
        }
        horizontalAlignment: Text.Left
        verticalAlignment: Text.Center

        // size settings
        width: parent.width
        height: parent.height

        // content settings
        font {
            pixelSize: Qt.application.font.pixelSize * 1.5
        }
        color: "gray"
    }

}
