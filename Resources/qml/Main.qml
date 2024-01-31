import QtQuick
import QtCore
import QtQuick.Window

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
    }
}
