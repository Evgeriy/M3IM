import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: page
    width: 600
    height: 400

    background: Rectangle {
        width: parent.width
        height: 50
        anchors.bottom: parent.bottom
        color: "whitesmoke"
    }

    FontLoader { id: sanFranciscoProRegular; source: "fonts/SF-Pro-Display-Regular.otf"; }

    CustomHeader {
        id: headerDialogPage
    }

    function setHeader(msg) {
        headerDialogPage.text = msg;
    }

    ListView {
        id: listView
        anchors.bottom: textMessage.top
        anchors.bottomMargin: 30
        topMargin: 10
        bottomMargin: 10
        anchors.rightMargin: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: headerDialogPage.bottom
        spacing: 25
        model: dialog


        delegate: Item {
            x: 10
            width: parent.width
            height: friendMessageText.paintedHeight
            Row {
                id: rowDialogItem
                width: parent.width

                Column {

                    id: friendMessage
                    width: rowDialogItem.width / 2 - 25

                    Text {
                        x: String(model.userId) === headerDialogPage.text ? parent.x : listView.width - paintedWidth - 37
                        leftPadding: 10
                        topPadding: 10

                        id: friendMessageText
                        horizontalAlignment: Text.AlignJustify

                        wrapMode: Text.Wrap
                        text: model.message
                        width: parent.width
                        height: paintedHeight
                        font.family: "Tahoma"
                        font.pixelSize: 14
                        font.bold: false

                        Rectangle {
                            radius: 25
                            width: parent.contentWidth + 20
                            height: parent.contentHeight + 20
                            color: String(model.userId) === headerDialogPage.text ? "#87cefa" : "#90ee90"
                            opacity: 0.5
                        }
                    }
                }
            }
        }
    }

    RoundButton {
        id: buttonAttach
        anchors.left: listView.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7

        background: Rectangle {
            radius: 25
            color: "lightgray"
            width: parent.width
            height: parent.height
        }

        width: 35
        height: 35
        font.pixelSize: 20
        radius: 100

        icon.width: 35
        icon.height: 35
        icon.source: "icons/attachment.png"
    }

    TextArea {
        leftPadding: 20
        topPadding: 9
        anchors.bottomMargin: 7

        id: textMessage
        anchors.left: buttonAttach.left
        anchors.leftMargin: 40
        anchors.bottom: parent.bottom
        width: parent.width - 115
        height: 35
        wrapMode: Text.Wrap
        text: qsTr("Text")
        font.pixelSize: 14

        background: Rectangle {
            x: 10
            radius: 25
            color: "lightgray"
            width: parent.width - 5
            height: parent.height
        }

        Keys.onEnterPressed: {
            client.sendMessage(textMessage.text, dialogHeaderLabel.text)
            textMessage.clear();
        }

        Keys.onReturnPressed: {
            client.sendMessage(textMessage.text, dialogHeaderLabel.text)
            textMessage.clear();
        }
    }

    RoundButton {
        id: buttonSend
        anchors.left: textMessage.right
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7

        background: Rectangle {
            radius: 25
            color: "lightgray"
            width: parent.width
            height: parent.height
        }

        width: 35
        height: 35
        font.pixelSize: 20
        radius: 100

        icon.width: 35
        icon.height: 35
        icon.source: "icons/send.png"

        onClicked: {
            client.sendMessage(textMessage.text, headerDialogPage.text)
            textMessage.clear();
            listView.positionViewAtEnd();
        }
    }
}
