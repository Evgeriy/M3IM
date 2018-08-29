import QtQuick 2.9
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

    CustomHeader {
        id: headerSettingsPage
        text: "Settings"
    }

    Connections {
        target: client
        onSocketStatusChanged : {
            labelSocketStatus.text = String(client.getSocketStatus());
        }
    }

    GridLayout {
        id: gridLayoutSettingsPage
        width: 350

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        rows: 3
        columns: 3
        columnSpacing: 15
        rowSpacing: 15

        Label {
            id: hostAddressLabel

            text: qsTr("Host Adress:")
            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        TextArea {
            id: hostIP
            font.pixelSize: getFontSize()
            placeholderText: "192.168.0.113"

            anchors.top: hostAddressLabel.top
            anchors.topMargin: -5

            background: Rectangle {
                radius: 25
                color: "lightgray"
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 2
        }

        TextArea {
            id: hostPort
            font.pixelSize: getFontSize()
            placeholderText: "6000"

            anchors.top: hostAddressLabel.top
            anchors.topMargin: -5

            background: Rectangle {
                radius: 25
                color: "lightgray"
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 3
        }


        Label {
            id: labelSocketStatus

            text: ""
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: getFontSize()

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 3
            Layout.row: 2
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
            Layout.columnSpan: 3
            Layout.row: 3
            Layout.column: 1

            onClicked: {
                client.reconnect(hostIP.text, parseInt(hostPort.text));
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
            Layout.columnSpan: 3
            Layout.row: 4
            Layout.column: 1

            onClicked: {
                client.disconnect();
            }
        }
    }
}
