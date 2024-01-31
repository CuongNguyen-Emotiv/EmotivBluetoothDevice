import QtQuick

Rectangle {
    id: buttonRoot
    radius: 10
    width: 100
    height: 50
    border.color: "gray"
    property alias text: textButton.text

    signal clicked()

    Text {
        id: textButton
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: buttonRoot.clicked()
    }
}
