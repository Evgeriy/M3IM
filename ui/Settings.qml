﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: pageSettings
    width: 600
    height: 400

    function getFontSize() {
        return Qt.application.font.pixelSize * 1.5;
    }

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    header: Label {
        height: 60
        RoundButton {
            background: Rectangle {
                color: "#87cefa"
                radius: 0
            }
            width: parent.width
            height: parent.height
            focusPolicy: Qt.NoFocus
            anchors.horizontalCenter: parent.horizontalCenter


            font.pixelSize: Qt.application.font.pixelSize * 2
            text: "Settings"
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }

    Connections {
        target: client
        onSocketStatusChanged : {
            labelSocketStatus.text = String(client.getSocketStatus());
        }
    }

    GridLayout {
        id: gridLayoutSettingsPage
        width: 200

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        rows: 3
        columns: 1
        columnSpacing: 0
        rowSpacing: 15

        Label {
            id: labelSocketStatus

            text: ""
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: getFontSize()

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        RoundButton {
            id: buttonReconnect

            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            contentItem: Text {
                color: "white"
                text: qsTr("Reconnect")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular
                horizontalAlignment: Text.Center
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 2
            Layout.column: 1

            onClicked: {
                client.reconnect();
            }
        }

        RoundButton {
            id: buttonDisconnect

            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            contentItem: Text {
                color: "white"
                text: qsTr("Disconnect")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular
                horizontalAlignment: Text.Center
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 1

            onClicked: {
                client.disconnect();
            }
        }
    }

}
