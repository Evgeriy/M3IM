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

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    CustomHeader {
        id: headerAddContactPage
        text: "Registration"
    }

    Connections {
        target: client
        onAuthStatusChanged: {
            heagerLabel.text = client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
            buttonRequestCode.visible = !buttonRequestCode.visible;
            console.log(buttonRequestCode.visible);
        }
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


//    GridLayout {
//        id: gridLayoutAuthorizationPage

//        anchors {
//            verticalCenter: parent.verticalCenter
//            horizontalCenter: parent.horizontalCenter
//        }

//        rows: 4
//        columns: 2
//        columnSpacing: 15
//        rowSpacing: 30

//        Label {
//            id: labelPhone

//            text: qsTr("Phone:")
//            font.pixelSize: getFontSize()
//            font.family: sanFranciscoProRegular

//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 1
//            Layout.row: 1
//            Layout.column: 1
//        }

//        CustomInput {
//            id: textEditPhone

//            // content settings
//            text: client.getPhone()
//            textFont {
//                pixelSize: getFontSize()
//            }

//            // layout settings
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 1
//            Layout.row: 1
//            Layout.column: 2
//        }

//        CustomButton {
//            id: buttonRequestCode

//            // anchors settings
////            anchors {
////                top: textEditPhone.bottom
////                topMargin: 35
////                left: parent.left
////                leftMargin: 50
////            }

//            // content settings
//            text: "Request Code"
//            textFont {
//                pixelSize: getFontSize()
//            }

//            // size settings
//            width: 200
//            height: 40

//            // layout settings
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 2
//            Layout.row: 2
//            Layout.column: 1

//            // slots
//            onClicked:  {
//                client.setPhone(textEditPhone.text);
//                client.sendRequestCode();
//            }
//        }

//        Label {
//            id: labelCode
//            visible: !client.getAuthStatus()

//            text: qsTr("Code:")
//            font.pixelSize: getFontSize()
//            font.family: sanFranciscoProRegular

//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 1
//            Layout.row: 3
//            Layout.column: 1
//        }

//        CustomInput {
//            id: textEditCode

//            // content settings
//            text: ""
//            textFont {
//                pixelSize: getFontSize()
//            }

//            // layout settings
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 1
//            Layout.row: 3
//            Layout.column: 2
//        }

//        CustomButton {
//            id: buttonRegister

//            // anchors settings
////            anchors {
////                top: textEditPhone.bottom
////                topMargin: 35
////                left: parent.left
////                leftMargin: 50
////            }

//            // content settings
//            text: "Register"
//            textFont {
//                pixelSize: getFontSize()
//            }

//            // size settings
//            width: 200
//            height: 40

//            // layout settings
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.rowSpan: 1
//            Layout.columnSpan: 2
//            Layout.row: 4
//            Layout.column: 1

//            // slots
//            onClicked:  {
//                client.setCode(textEditCode.text);
//                client.sendRequestJWT();
//            }
//        }
//    }
}
