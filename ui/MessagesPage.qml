﻿import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        id: dialogHeaderLabel
        height: 60

        RoundButton {
            id:dialogHeaderLabelButton
            background: Rectangle {
                color: "#87cefa"
                radius: 0
            }
            width: parent.width
            height: parent.height
            focusPolicy: Qt.NoFocus
            anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: Qt.application.font.pixelSize * 2
            text: ""
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }

    function setHeader(msg) {
        dialogHeaderLabelButton.text = msg;
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
                        x: String(model.userId) === dialogHeaderLabel.text ? parent.x : listView.width - paintedWidth - 37
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
                            border.color: String(model.userId) === dialogHeaderLabel.text ? "#70ee90" : "lightgray"
                            border.width: 1
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
        anchors.bottom: parent.bottom
        width: parent.width - 90
        height: 70
        wrapMode: Text.Wrap
        text: qsTr("Text")
        font.pixelSize: 14

        Rectangle {
            x: 10
            border.color: "#1e90ff"
            border.width: 1
            radius: 15
            width: parent.width - 5
            height: parent.height
            color: "#100000FF"
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
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        width: 70
        height: 70
        text: qsTr("Send")
        font.pixelSize: 14
        radius: 15

        Rectangle {
            border.color: "#1e90ff"
            border.width: 1
            radius: 15
            width: parent.width
            height: parent.height
            color: "#100000FF"
        }

        onClicked: {
            client.sendMessage(textMessage.text, dialogHeaderLabel.text)
            textMessage.clear();
            listView.positionViewAtEnd();
        }
    }
}
