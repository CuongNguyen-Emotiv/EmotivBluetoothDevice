import QtQuick
import QtCore
import QtQuick.Window
import QtQuick.Controls

Window {
    width: Window.width
    height: Window.height
    visible: true
    title: qsTr("Hello World")

    Column {
        Row {
            Button {
                id: buttonScan
                text: qsTr("Scan")
                onClicked: {
                    HostDevice.startScan();
                }
            }

            Button {
                id: buttonStop
                text: qsTr("Stop")
                onClicked: {
                    HostDevice.stopScan();
                }
            }
        }
        Text {
            text: HostDevice.state
        }
        ListView {
            height: 600
            width: 600
            model: HostDevice.headsetModel
            delegate: Row {
                spacing: 20
                Button {
                    text: "Connect"
                }

                Text {
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "RSSI: " + model.rssi
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
