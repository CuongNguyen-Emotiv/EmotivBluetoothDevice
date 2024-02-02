import QtQuick
import HeadsetController 1.0
import QtQuick.Controls

Rectangle {
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0
    color: "white"

    property var bluetoothDeviceInfo: null

    HeadsetController {
        id: headsetController
    }

    Column {
        Row {
            Button {
                text: "Connect"
                onClicked: {
                    headsetController.connectToHeadset(bluetoothDeviceInfo)
                }
                enabled: headsetController.connectStatus === "Disconnected"
            }

            Button {
                text: "Disconnect"
                onClicked: {
                    headsetController.disconnectFromHeadset()
                }
            }
        }

        Text {
            text: "Connect Status: " + headsetController.connectStatus
        }

        Text {
            text: "Received Signal Strength Indicator (RSSI): " + (headsetController.connectStatus !== "Connected" ? 0 : headsetController.rssi)
        }

        Text {
            text: "Maximum Transmission Unit (MTU): " + (headsetController.connectStatus !== "Connected" ? 0 : headsetController.mtu)
        }
    }

    Component.onCompleted: {
        headsetController.connectToHeadset(bluetoothDeviceInfo)
    }
}
