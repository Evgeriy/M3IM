import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: pageId
    width: 600
    height: 400
    signal sigNextPage()

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

    function getHeader() {
        return String(client.getActiveDialog());
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 7
        model: contacts

        topMargin: 10
        bottomMargin: 10



        delegate: Item {
            x: 10
            width: parent.width
            height: 75



            Row {
                id: rowFriendItem
                width: parent.width
                height: 80
                spacing: 5

                Column {
                    id: columnAvatar
                    width: 75

                    RoundButton {
                        background: Rectangle {
                            color: model.isOnline ? model.unread ? "#87cefa": "#90ee90" : "#100000FF"
                            border.color: "#1e90ff"
                            border.width: 1
                            radius: 100
                        }
                        width: 75
                        height: 75
                    }
                }

                Column {
                    id: columnFriendInfo
                    width: rowFriendItem.width - (columnAvatar.width + columnStatus.width) - spacing * 3;

                    Text {
                        id: textContactId
                        text: model.userId
                        font.family: "Tahoma"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        id: textContactPhone
                        text: model.phone
                        font.family: "Tahoma"
                        font.pixelSize: 15
                        font.bold: true
                    }

                    Text {
                        id: textLastMessage
                        text: getLastMessage(model.lastMessage)
                        font.family: "Tahoma"
                        font.pixelSize: 15
                        color: "gray"
                    }

                    spacing: 5
                }

                Column {
                    id: columnStatus
                    width: 45
                    y: parent.y + 20

                    RoundButton {
                        background: Rectangle {
                            color: "#87cefa"
                            radius: 100
                        }

                        text: String(model.unread)
                        font.family: "Tahoma"
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
                anchors.leftMargin: 80
                anchors.bottomMargin: -6
                color: "lightgray"
                text: "_".repeat(separator.width)
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
