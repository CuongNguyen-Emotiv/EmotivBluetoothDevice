import QtQuick
import QtCore
import QtQuick.Window
import QtQuick.Controls

Window {
    id: appWindow
    width: Window.width
    height: Window.height
    visible: true
    title: qsTr("Hello World")

    Column {
        Button {
            id: buttonBack
            text: "Back"
            onClicked: {
                stackview.pop();
            }
        }
        StackView {
            id: stackview
            initialItem: mainView
            width: appWindow.width
            height: appWindow.height - buttonBack.height
        }
    }

    Component {
        id: mainView
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
                id: listviewHeadsets
                height: 600
                width: 600
                model: HostDevice.headsetModel
                delegate: Row {
                    spacing: 20
                    Button {
                        text: "Connect"
                        onClicked: {
                            stackview.push("qrc:/qml/HeadsetInfo.qml", { "bluetoothDeviceInfo": model.bluetoothDeviceInfo } );
                        }
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
}
