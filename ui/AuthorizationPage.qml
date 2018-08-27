import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4



Page {
    id: pageId
    width: 600
    height: 400
    signal sigNextPage()

    function getFontSize() {
        return Qt.application.font.pixelSize * 1.5;
    }

    background: Rectangle {
        width: parent.width
        height: parent.width
        anchors.bottom: parent.bottom
        color: "whitesmoke"
    }

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    header: Label {
        id: heagerLabel
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
            text: client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
            font.family: sanFranciscoProRegular

        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }

    Connections {
        target: client
        onAuthStatusChanged: {
            heagerLabel.text = client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
            buttonRequestCode.visible = !buttonRequestCode.visible;
            console.log(buttonRequestCode.visible);
        }
    }

    GridLayout {
        id: gridLayoutAuthorizationPage

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        rows: 4
        columns: 2
        columnSpacing: 15
        rowSpacing: 20

        Label {
            id: labelPhone

            text: qsTr("Phone:")
            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        TextField {
            id: textEditPhone
            text: client.getPhone()
            horizontalAlignment: Text.Center

            anchors.top: labelPhone.top
            anchors.topMargin: -5

            background: Rectangle {
                radius: 8
                color: "lightgray"
                width: 145
            }

            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular

            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 2
        }

        RoundButton {
            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            id: buttonRequestCode
            visible: !client.getAuthStatus()

            contentItem: Text {
                color: "white"
                text: qsTr("Request Code")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular
                horizontalAlignment: Text.Center
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.row: 2
            Layout.column: 1

            width: 200

            onClicked: {
                client.setPhone(textEditPhone.text);
                client.sendRequestCode();
            }
        }

        Label {
            id: labelCode
            visible: !client.getAuthStatus()

            text: qsTr("Code:")
            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 1
        }

        TextField {
            id: textEditCode
            text: ""
            horizontalAlignment: Text.Center
            visible: !client.getAuthStatus()

            anchors.top: labelCode.top
            anchors.topMargin: -5

            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular

            background: Rectangle {
                radius: 8
                color: "lightgray"
                width: 145
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 2
        }

        RoundButton {
            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            width: 200

            id: buttonRegister
            visible: !client.getAuthStatus()


            contentItem: Text {
                color: "white"
                text: qsTr("Register")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular
                horizontalAlignment: Text.Center
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.row: 4
            Layout.column: 1

            onClicked: {
                client.setCode(textEditCode.text);
                client.sendRequestJWT();
            }
        }
    }




}
