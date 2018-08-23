﻿import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        id: dialogHeaderLabel
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
        text: ""
    }

    function setHeader(msg) {
        dialogHeaderLabel.text = msg;
    }

    ListView {
        id: listView
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 64
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
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
                        x: model.userId === dialogHeaderLabel.text ? parent.x : listView.width - paintedWidth - 37
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
                            border.color: model.userId === dialogHeaderLabel.text ? "#1e90ff" : "lightgray"
                            border.width: 3
                            radius: 15
                            width: parent.contentWidth + 20
                            height: parent.contentHeight + 20
                            color: "#100000FF"
                        }
                    }
                }
            }
        }
    }

    TextArea {
        leftPadding: 15
        topPadding: 15
        anchors.bottomMargin: 15

        id: textMessage
        anchors.left: listView.left
        anchors.top: listView.bottom
        anchors.bottom: parent.bottom
        width: parent.width - 90
        wrapMode: Text.Wrap
        text: qsTr("Text")
        font.pixelSize: 14

        Rectangle {
            x: 10
            border.color: "lightgray"
            border.width: 3
            radius: 15
            width: parent.width - 5
            height: parent.height
            color: "transparent"
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
        anchors.leftMargin: 15
        anchors.top: listView.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        width: 70
        text: qsTr("Send")
        font.pixelSize: 14
        radius: 15

        Rectangle {
            border.color: "lightgray"
            border.width: 3
            radius: 15
            width: parent.width
            height: parent.height
            color: "transparent"
        }

        onClicked: {
            client.sendMessage(textMessage.text, dialogHeaderLabel.text)
            textMessage.clear();
        }
    }


}

/*##^## Designer {
    D{i:2;anchors_height:153}
}
 ##^##*/