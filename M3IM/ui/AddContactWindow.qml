import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.3

Window {
    id: addContactPage

    // properties
    property int windowWidth: 450
    property int windowHeight: 350
    property int fontPixelSize: Qt.application.font.pixelSize * 1.5
    property string firstName: ""
    property string lastName: ""
    property string phone: ""

    // size settings
    width: windowWidth
    height: windowHeight

    minimumWidth: windowWidth
    minimumHeight: windowHeight

    maximumWidth: windowWidth
    maximumHeight: windowHeight

    modality: Qt.WindowModal

    // window header title
    CustomHeader {
        id: headerAddContactPage

        // size settings
        width: parent.width

        // content settings
        text: "Add Contact"
    }

    CustomLabel {
        id: labelFirstName

        // anchors settings
        anchors {
            top: parent.top
            topMargin: 100
            left: parent.left
            leftMargin: 50
        }

        // content settings
        text: "first name"
    }

    CustomInput {
        id: textEditFirstName

        anchors.top: labelFirstName.top
        anchors.left: labelFirstName.right
        anchors.leftMargin: 60

        // content settings
        text: ""
        textFont {
            pixelSize: getFontSize()
        }
    }

    CustomLabel {
        id: labelLastName

        // anchors settings
        anchors {
            top: labelFirstName.bottom
            topMargin: 25
            left: parent.left
            leftMargin: 50
        }

        // content settings
        text: "last name"
    }

    CustomInput {
        id: textEditLastName

        anchors.top: labelFirstName.bottom
        anchors.topMargin: 25
        anchors.left: labelLastName.right
        anchors.leftMargin: 60

        // content settings
        text: ""
        textFont {
            pixelSize: getFontSize()
        }
    }

    CustomLabel {
        id: labelPhone

        // anchors settings
        anchors {
            top: labelLastName.bottom
            topMargin: 25
            left: parent.left
            leftMargin: 50
        }

        // content settings
        text: "phone"
    }

    CustomInput {
        id: textEditPhone

        anchors.top: labelLastName.bottom
        anchors.topMargin: 25
        anchors.left: labelFirstName.right
        anchors.leftMargin: 60

        // content settings
        text: ""
        textFont {
            pixelSize: getFontSize()
        }
    }

    CustomButton {
        id: buttonCancel

        // anchors settings
        anchors {
            top: textEditPhone.bottom
            topMargin: 80
            left: parent.left
            leftMargin: 50
        }

        // content settings
        text: "Cancel"
        textFont {
            pixelSize: getFontSize()
        }

        // size settings
        width: 135
        height: 40

        // slots
        onClicked:  {
            // close contact window
            addContactPage.close();
        }
    }

    CustomButton {
        id: buttonAddContact

        // anchors settings
        anchors {
            top: textEditPhone.bottom
            topMargin: 80
            left: buttonCancel.right
            leftMargin: 45
        }

        // content settings
        text: "Add Contact"
        textFont {
            pixelSize: getFontSize()
        }

        // size settings
        width: 135
        height: 40

        // slots
        onClicked:  {
            // capture contact data
            firstName = textEditFirstName.text;
            lastName  = textEditLastName.text;
            phone     = textEditPhone.text;

            // try add new contact
            var result = client.addNewContact(firstName, lastName, phone);

            // configure popup message and popupAddContactResult optional button
            var popupTextMessage = "";
            switch (result) {
              case 1:
                popupTextMessage = "This person is not registered on Instance Messenger yet";
                buttonInvite.visible = true;
                break;
              case 2:
                popupTextMessage = "This person is already registered on Instance Messenger";
                buttonShowDialog.visible = true;
                break;
              case 3:
                popupTextMessage = "This person add to contact list";
                buttonShowContact.visible = true;
                break;
              default:
                break;
            }

            // check popup message
            if (popupTextMessage.length > 0) {
                popupText.text = popupTextMessage;
                popupAddContactResult.open();
            }
        }
    }

    Popup {
        id: popupAddContactResult
        x: 75
        y: 150
        width: 300
        height: 200
        modal: true
        focus: true

        TextField {
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

        CustomButton {
            id: buttonOk

            // anchors settings
            anchors {
                top: popupText.bottom
                topMargin: 15
                left: popupText.left
            }

            // content settings
            text: "Ok"
            textFont {
                pixelSize: getFontSize()
            }

            // size settings
            width: 130
            height: 40

            // slots
            onClicked:  {
                // close popup and add contact window
                popupAddContactResult.close();
                addContactPage.close();
            }
        }

        CustomButton {
            id: buttonShowDialog
            visible: false

            // anchors settings
            anchors {
                top: popupText.bottom
                topMargin: 15
                left: buttonOk.right
                leftMargin: 10
            }

            // content settings
            text: "Show Dialog"
            textFont {
                pixelSize: getFontSize()
            }

            // size settings
            width: 130
            height: 40

            // slots
            onClicked:  {
                popupAddContactResult.close();          // close popup
                addContactPage.close();                 // close contact window

                var userId = client.getUserId(firstName, phone); // get id of added user
                client.setActiveDialog(userId);                  // set active dialog
                contactListPage.sigNextPage();                   // show dialog page
            }
        }

        CustomButton {
            id: buttonShowContact
            visible: false

            // anchors settings
            anchors {
                top: popupText.bottom
                topMargin: 15
                left: buttonOk.right
                leftMargin: 10
            }

            // content settings
            text: "Show Contact"
            textFont {
                pixelSize: getFontSize()
            }

            // size settings
            width: 130
            height: 40

            // slots
            onClicked:  {
                popupAddContactResult.close();          // close popup
                addContactPage.close();                 // close contact window
            }
        }

        CustomButton {
            id: buttonInvite
            visible: false

            // anchors settings
            anchors {
                top: popupText.bottom
                topMargin: 15
                left: buttonOk.right
                leftMargin: 10
            }

            // content settings
            text: "Invite"
            textFont {
                pixelSize: getFontSize()
            }

            // size settings
            width: 130
            height: 40

            // slots
            onClicked:  {
                popupAddContactResult.close();          // close popup
                addContactPage.close();                 // close contact window
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
}
