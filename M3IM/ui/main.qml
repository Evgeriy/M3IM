import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480

    function getFontSize() {
        return Qt.application.font.pixelSize * 1.5;
    }


    title: "Instance Messenger"

    Connections {
        target: tabAuthorization
        onSigNextPage: {
            swipeView.setCurrentIndex(1);
        }
    }

    Connections {
        target: tabFriends
        onSigNextPage: {
            swipeView.setCurrentIndex(2);
            tabMessages.setHeader(tabFriends.getHeader());
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        AuthorizationPage {
            id: tabAuthorization
        }

        FriendsPage {
            id: tabFriends
        }

        MessagesPage {
            id: tabMessages
        }

        Settings {
            id: tabSettings
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        Rectangle {
            width: parent.width
            height: parent.height
            color: "whitesmoke"
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            font.family: sanFranciscoProRegular

            text: qsTr("Authorization")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#00a0ff"
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            font.family: sanFranciscoProRegular

            text: qsTr("Friends")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#00a0ff"
            }

            onClicked: {
                client.sendRequestContacts()
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            font.family: sanFranciscoProRegular
            text: qsTr("Messages")

            background: Rectangle {
                width: parent.width
                height: parent.height
                color: "#00a0ff"
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            font.family: sanFranciscoProRegular
            text: qsTr("Settings")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#00a0ff"
            }
        }
    }
}
