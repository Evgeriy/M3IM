import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.3

Window {
    id: addContactPage
    width: 450
    height: 350
    minimumHeight: addContactPage.height
    minimumWidth: addContactPage.width
    maximumHeight: addContactPage.height
    maximumWidth: addContactPage.width

    property var firstName;
    property var lastName;
    property var phone;

    function getFontSize() {
        return Qt.application.font.pixelSize * 1.5;
    }

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    Connections {
        target: client
        onAuthStatusChanged: {
            heagerLabel.text = client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
            buttonRequestCode.visible = !buttonRequestCode.visible;
            console.log(buttonRequestCode.visible);
        }
    }

    TextField {
        id: header

        background: Rectangle {
            color: "#87cefa"
            radius: 0
        }

        horizontalAlignment: Text.horizontalCenter

        enabled: false

        width: parent.width
        height: 60
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter

        padding: 10
    }

    Label {
        id: headerText

        horizontalAlignment: Text.horizontalCenter

        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter

        padding: 10

        text: qsTr("Add Contact")
        font.pixelSize: Qt.application.font.pixelSize * 2
        font.family: sanFranciscoProRegular.name
    }

    Label {
        id: labelFirstName

        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 50

        text: qsTr("First Name:")
        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name

    }

    TextField {
        id: textEditFirstName
        text: ""
        horizontalAlignment: Text.Center
        width: 200

        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: labelFirstName.right
        anchors.leftMargin: 50

        background: Rectangle {
            radius: 8
            color: "lightgray"
            width: 200

        }

        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name
    }

    Label {
        id: labelLastName

        anchors.top: labelFirstName.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 50

        text: qsTr("Last Name:")
        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name

    }

    TextField {
        id: textEditLastName
        text: ""
        horizontalAlignment: Text.Center
        width: 200

        anchors.top: labelFirstName.bottom
        anchors.topMargin: 25
        anchors.left: labelFirstName.right
        anchors.leftMargin: 50

        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name

        background: Rectangle {
            radius: 8
            color: "lightgray"
            width: 200
        }

    }

    Label {
        id: labelPhone

        anchors.top: labelLastName.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 50

        text: qsTr("Phone:")
        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name

    }

    TextField {
        id: textEditPhone
        text: ""
        horizontalAlignment: Text.Center
        width: 200

        anchors.top: labelLastName.bottom
        anchors.topMargin: 25
        anchors.left: labelFirstName.right
        anchors.leftMargin: 50

        font.pixelSize: getFontSize()
        font.family: sanFranciscoProRegular.name

        background: Rectangle {
            radius: 8
            color: "lightgray"
            width: 200
        }
    }

    RoundButton {
        background: Rectangle {
            color: "#87cefa"
            radius: 8
        }

        id: buttonCancel

        anchors.top: textEditPhone.bottom
        anchors.topMargin: 35
        anchors.left: parent.left
        anchors.leftMargin: 50

        contentItem: Text {
            color: "white"
            text: qsTr("Cancel")
            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular.name
            horizontalAlignment: Text.Center
        }

        width: 150

        onClicked: {
            addContactPage.close();
        }
    }

    RoundButton {
        background: Rectangle {
            color: "#87cefa"
            radius: 8
        }

        id: buttonAdd

        anchors.top: textEditPhone.bottom
        anchors.topMargin: 35
        anchors.left: buttonCancel.right
        anchors.leftMargin: 45

        contentItem: Text {
            color: "white"
            text: qsTr("Add Contact")
            font.pixelSize: getFontSize()
            font.family: sanFranciscoProRegular.name
            horizontalAlignment: Text.Center
        }

        width: 150

        onClicked: {
            firstName = textEditFirstName.text;
            lastName = textEditLastName.text;
            phone = textEditPhone.text;

            var result = client.addNewContact(firstName, lastName, phone);

            if (result === 1) {
                popupText.text = "This person is not registered on Instance Messenger yet.\nYou will be able to send them a IM message as sonn as they sign up."
                popup.open();
            } else if (result === 2) {
                popupText.text = "This person is already registered on Instance Messenger";
                buttonSend.visible = true;
                popup.open();
            } else if (result === 3) {
                popupText.text = "This person add to contact list";
                buttonSend.visible = true;
                popup.open();
            }
        }
    }

    Popup {
        id: popup
        x: 75
        y: 150
        width: 300
        height: 200
        modal: true
        focus: true

        TextArea {
            id: popupText
            text: ""
            verticalAlignment: Text.Center
            horizontalAlignment: Text.Center
            width: 270
            height: 100

            wrapMode: "WordWrap"

            enabled: false

            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: 13
            font.family: sanFranciscoProRegular.name

            background: Rectangle {
                radius: 8
                color: "lightgray"
                width: 270
            }
        }

        RoundButton {
            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            id: buttonOk

            anchors.top: popupText.bottom
            anchors.topMargin: 15
            anchors.left: popupText.left

            contentItem: Text {
                color: "white"
                text: qsTr("Ok")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular.name
                horizontalAlignment: Text.Center
            }

            width: 130

            onClicked: {
                popup.close();
                addContactPage.close();
            }
        }

        RoundButton {
            background: Rectangle {
                color: "#87cefa"
                radius: 8
            }

            id: buttonSend
            visible: false

            anchors.top: popupText.bottom
            anchors.topMargin: 15
            anchors.left: buttonOk.right
            anchors.leftMargin: 10

            contentItem: Text {
                color: "white"
                text: qsTr("Send")
                font.pixelSize: getFontSize()
                font.family: sanFranciscoProRegular.name
                horizontalAlignment: Text.Center
            }

            width: 130

            onClicked: {
                popup.close();
                addContactPage.close();



                client.setActiveDialog(client.getUserId(firstName, phone));
                pageId.sigNextPage();
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
}