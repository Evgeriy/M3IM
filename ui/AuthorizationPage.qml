import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Page {
    id: pageId
    width: 600
    height: 400
    signal sigNextPage()

    CustomHeader {
        id: headerAddContactPage
        text: "Registration"
    }

    Label {
        id: labelPhone

        // anchors settings
        anchors {
            top: headerAddContactPage.bottom
            topMargin: 50
            left: parent.left
            leftMargin: parent.width / 3
        }

        // content settings
        text: "phone"
        color: "gray"
        font.pixelSize: getFontSize()
    }

    CustomInput {
        id: textEditPhone

        // anchors settings
        anchors {
            top: labelPhone.top
            left: labelPhone.right
            leftMargin: 25
        }

        // content settings
        text: client.getPhone()
        textFont {
            pixelSize: getFontSize()
        }
    }

    CustomButton {
        id: buttonRequestCode

        // anchors settings
        anchors {
            top: labelPhone.bottom
            topMargin: 30
            left: labelPhone.left
            leftMargin: 10
        }

        // content settings
        text: "Request Code"
        textFont {
            pixelSize: getFontSize()
        }

        // size settings
        width: 220
        height: 40

        // slots
        onClicked:  {
            client.setPhone(textEditPhone.text);
            client.sendRequestCode();
        }
    }

    Label {
        id: labelCode

        // anchors settings
        anchors {
            top: buttonRequestCode.bottom
            topMargin: 30
            left: labelPhone.left
        }

        // content settings
        text: "code"
        color: "gray"
        font.pixelSize: getFontSize()
    }

    CustomInput {
        id: textEditCode

        // anchors settings
        anchors {
            top: labelCode.top
            left: labelCode.right
            leftMargin: 25
        }

        // content settings
        text: ""
        textFont {
            pixelSize: getFontSize()
        }
    }

    CustomButton {
        id: buttonRegister

        // anchors settings
        anchors {
            top: labelCode.bottom
            topMargin: 30
            left: labelCode.left
            leftMargin: 10
        }

        // content settings
        text: "Register"
        textFont {
            pixelSize: getFontSize()
        }

        // size settings
        width: 220
        height: 40

        // slots
        onClicked:  {
            client.setCode(textEditCode.text);
            client.sendRequestJWT();
        }
    }
}
