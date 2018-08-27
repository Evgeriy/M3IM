import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    minimumWidth: 640
    minimumHeight: 480

    title: qsTr("Instance Messenger")

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#100000FF"
    }


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
            color: "#87cefa"
        }


        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: qsTr("Authorization")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#87cefa"
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: qsTr("Friends")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#87cefa"
            }

            onClicked: {
                client.sendRequestContacts()
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: qsTr("Messages")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#87cefa"
            }
        }

        TabButton {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: qsTr("Settings")
            Rectangle {
                width: parent.width
                height: parent.height
                color: "#87cefa"
            }
        }
    }
}
