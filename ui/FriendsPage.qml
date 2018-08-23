import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: pageId
    width: 600
    height: 400
    signal sigNextPage()

    header: Label {
        text: qsTr("Contacts")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    function getHeader() {
        return client.getActiveDialog();
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 4
        model: contacts

        delegate: Item {
            x: 10
            width: parent.width
            height: 80

            Row {
                id: rowFriendItem
                width: parent.width
                height: parent.height
                spacing: 10

                Column {
                    id: columnAvatar
                    width: rowFriendItem.height / 1.2

                    RoundButton {
                        background: Rectangle {
                            color: "blue"
                            border.color: "#dd0cbcbc"
                            border.width: 4
                            radius: 100
                        }
                        width: rowFriendItem.height / 1.2
                        height: rowFriendItem.height / 1.2
                    }
                }

                Column {
                    id: columnFriendInfo
                    width: rowFriendItem.width - (columnAvatar.width) - spacing * 3;

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
                        text: "Last message"
                        font.family: "Tahoma"
                        font.pixelSize: 15
                    }

                    spacing: 5
                }
            }

            MouseArea {
                anchors.fill: rowFriendItem
                onClicked: {
                    client.setActiveDialog(model.userId);
                    console.log(client.getActiveDialog());
                    client.sendRequestDialog(model.userId);
                    pageId.sigNextPage();
                }
            }
        }
    }
}
