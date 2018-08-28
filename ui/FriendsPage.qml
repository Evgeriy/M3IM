import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: pageId
    width: 600
    height: 400
    signal sigNextPage()

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    function getHeader() {
        return String(client.getActiveDialog());
    }

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
            text: qsTr("Contacts")
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }

    TextArea {
        id: phoneNumber
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: parent.width
        height: 35
        font.pixelSize: 14

        placeholderText: "Find by number..."


        background: Rectangle {

            radius: 25
            color: "lightgray"
            width: parent.width - 35
            height: parent.height
        }

        Keys.onEnterPressed: {

        }
    }

    ListView {
        id: listView
        anchors.top: phoneNumber.bottom
        anchors.topMargin: 50
        anchors.bottom: parent.bottom
        anchors.fill: parent
        spacing: 5
        model: contacts

        topMargin: 10
        bottomMargin: 10

        delegate: Item {
            x: 10
            width: parent.width
            height: 65

            Row {
                id: rowFriendItem
                width: parent.width
                height: 80
                spacing: 10

                Column {
                    id: columnAvatar
                    width: 60

                    RoundButton {
                        background: Rectangle {
                            border.width: 1
                            border.color: "#87cefa"
                            color: model.isOnline ? model.unread ? "#87cefa": "#90ee90" : "lightgray"
                            radius: 100
                            opacity: 0.7
                        }
                        width: 60
                        height: 60
                    }
                }

                Column {
                    id: columnFriendInfo
                    width: rowFriendItem.width - (columnAvatar.width + columnStatus.width) - spacing * 5;

                    Text {
                        id: textContactId
                        text: model.userId
                        font.family: sanFranciscoProRegular.name
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        id: textContactPhone
                        text: model.phone
                        font.family: sanFranciscoProRegular.name
                        font.pixelSize: 15
                    }

                    Text {
                        id: textLastMessage
                        text: getLastMessage(model.lastMessage)
                        font.family: sanFranciscoProRegular.name
                        font.pixelSize: 15
                        color: "gray"
                    }

                    spacing: 5
                }

                Column {
                    id: columnStatus
                    width: 55
                    y: parent.y + 20

                    RoundButton {
                        background: Rectangle {
                            color: "#87cefa"
                            radius: 100
                        }

                        text: String(model.unread)
                        font.family: sanFranciscoProRegular.name
                        font.pixelSize: 12
                        font.bold: true

                        width: model.unread ? 30 : 0
                        height: model.unread ? 30 : 0
                    }
                }
            }

            Text {
                id: separator
                width: parent.width
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 70
                anchors.bottomMargin: -6
                color: "lightgray"
                text: "_".repeat(separator.width / 6.8)
            }

            MouseArea {
                anchors.fill: rowFriendItem
                onClicked: {
                    client.setActiveDialog(model.userId);
                    model.unread = 0;
                    pageId.sigNextPage();
                }
            }
        }
    }

    function getLastMessage(msg) {
        var retValue = "";
        if (msg.length > 20) {
            retValue = msg.substring(0, 17) + "...";
        } else {
            retValue = msg;
        }
        return retValue;
    }
}
