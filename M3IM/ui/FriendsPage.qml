import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2

Page {
    id: contactListPage
    width: 600
    height: 400

    signal sigNextPage()

    function getHeader() {
        return String(client.getActiveDialog());
    }

    property variant windowAddContact;

    CustomHeader {
        id: headerContactsPage
        text: "Contacts"
    }

    RoundButton {
        id: buttonAddContact
        anchors.top: headerContactsPage.bottom
        anchors.topMargin: 10
        anchors.left: headerContactsPage.left
        anchors.leftMargin: 20

        background: Rectangle {
            radius: 25
            color: "whitesmoke"
            width: parent.width
            height: parent.height
        }

        width: 35
        height: 35
        font.pixelSize: 20
        radius: 100

        icon.width: 35
        icon.height: 35
        icon.source: "icons/add.png"

        onClicked: {
            var component = Qt.createComponent("qrc:/ui/AddContactWindow.qml");
            windowAddContact = component.createObject(contactListPage);
            windowAddContact.show();
        }
    }

    TextArea {
        id: phoneNumber
        topPadding: 8
        anchors.top: headerContactsPage.bottom
        anchors.topMargin: 10
        anchors.left: buttonAddContact.right
        anchors.leftMargin: 15
        width: parent.width - 42
        height: 35
        font.pixelSize: 14

        placeholderText: "Find by number..."


        background: Rectangle {
            radius: 5
            color: "whitesmoke"
            width: parent.width - 42
            height: parent.height
        }

        Keys.onEnterPressed: {

        }
    }

    ListView {
        id: listView

        anchors {
            top: phoneNumber.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        //anchors.fill: parent
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
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        id: textContactPhone
                        text: model.phone
                        font.pixelSize: 15
                    }

                    Text {
                        id: textLastMessage
                        text: getLastMessage(model.lastMessage)
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
                color: "whitesmoke"
                text: "_".repeat(separator.width / 6.8)
            }

            MouseArea {
                anchors.fill: rowFriendItem
                onClicked: {
                    client.setActiveDialog(model.userId);
                    model.unread = 0;
                    contactListPage.sigNextPage();
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
