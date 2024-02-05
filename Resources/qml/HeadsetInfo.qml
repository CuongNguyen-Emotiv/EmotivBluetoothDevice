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
            text: "Received Signal Strength Indicator (RSSI): " + headsetController.rssi
        }

        Text {
            text: "Maximum Transmission Unit (MTU): " + headsetController.mtu
        }

        Text {
            text: qsTr("Services: ")
        }

        ListView {
            width: parent.width
            height: 600
            model: headsetController.serviceModel
            clip: true
            delegate: Row {
                spacing: 10
                Button {
                    text: "Read"
                    onClicked: {
                        stackview.push("qrc:/qml/ServiceInfo.qml",
                                       {
                                           "qLowEnergyServicePtr": model.qLowEnergyServicePtr,
                                           "qLowEnergyControllerPtr": headsetController.lowEnergyController
                                       }
                                       );
                    }
                }

                Text {
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: model.type
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Component.onCompleted: {
        headsetController.connectToHeadset(bluetoothDeviceInfo)
    }
}
