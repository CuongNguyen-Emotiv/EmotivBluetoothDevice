import QtQuick
import CharacteristicController 1.0
import QtQuick.Controls

Rectangle {
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    property var qLowEnergyCharacteristicPtr: null
    property var qLowEnergyServicePtr: null

    CharacteristicController {
        id: characteristicController
        service: qLowEnergyServicePtr
        characteristic: qLowEnergyCharacteristicPtr
    }

    Column {
        width: parent.width
        Row {
            Text {
                text: "Characteristic UUID: "
            }
            TextInput {
                text: characteristicController.characteristicUuid
                selectByMouse: true
                readOnly: true
            }
        }
        Row {
            Text {
                text: "Characteristic Name: "
            }
            TextInput {
                text: characteristicController.characteristicName
                selectByMouse: true
                readOnly: true
            }
        }
        Row {
            Text {
                text: "Characteristic Properties: "
            }
            TextInput {
                text: characteristicController.characteristicProperties
                selectByMouse: true
                readOnly: true
            }
        }
        Row {
            Text {
                text: "Characteristic Text Value: "
            }
            TextInput {
                text: characteristicController.characteristicTextValue
                selectByMouse: true
                readOnly: true
            }
        }
        Row {
            Text {
                text: "Characteristic Hex Value: "
            }
            TextInput {
                text: characteristicController.characteristicHexValue
                selectByMouse: true
                readOnly: true
            }
        }

        Column {
            width: parent.width
            Text {
                text: qsTr("Descriptors: ")
            }
            ListView {
                width: parent.width
                height: 300
                clip: true
                model: characteristicController.descriptorModel
                delegate: Rectangle {
                    width: parent.width
                    height: childrenRect.height
                    border.width: 1
                    border.color: "gray"
                    Column {
                        width: parent.width
                        Row {
                            width: parent.width
                            Text {
                                text: "Descriptor UUID: "
                            }
                            TextInput {
                                text: model.uuid
                                selectByMouse: true
                                readOnly: true
                            }
                        }

                        Row {
                            width: parent.width
                            Text {
                                text: "Descriptor Name: "
                            }
                            TextInput {
                                text: model.name
                                selectByMouse: true
                                readOnly: true
                            }
                        }

                        Row {
                            width: parent.width
                            Text {
                                text: "Descriptor Type: "
                            }
                            TextInput {
                                text: model.type
                                selectByMouse: true
                                readOnly: true
                            }
                        }

                        Row {
                            width: parent.width
                            Text {
                                text: "Descriptor Value: "
                            }
                            TextInput {
                                text: model.value
                                selectByMouse: true
                                readOnly: true
                            }
                        }

                        Row {
                            width: parent.width
                            Text {
                                text: "Descriptor Is Valid: "
                            }
                            TextInput {
                                text: model.isValid
                                selectByMouse: true
                                readOnly: true
                            }
                        }
                    }
                }
            }
        }

        Column {
            visible: characteristicController.hasNotifyProperty
            Row {
                Button {
                    text: "Notify"
                    onClicked: characteristicController.startNotify()
                }
                Button {
                    text: "Stop Notify"
                    onClicked: characteristicController.stopNotify()
                }
            }

            Row {
                Text {
                    text: "Characteristic Notify Value (Text): "
                }
                TextInput {
                    text: characteristicController.notifiedTextValue
                    selectByMouse: true
                    readOnly: true
                }
            }

            Row {
                Text {
                    text: "Characteristic Notify Value (Hex): "
                }
                TextInput {
                    text: characteristicController.notifiedHexValue
                    selectByMouse: true
                    readOnly: true
                }
            }
        }

        Column {
            visible: characteristicController.hasIndicateProperty
            Row {
                Button {
                    text: "Indicate"
                    onClicked: characteristicController.startIndicate()
                }
                Button {
                    text: "Stop Indicate"
                    onClicked: characteristicController.stopIndicate()
                }
            }

            Row {
                Text {
                    text: "Characteristic Indicate Value (Text): "
                }
                TextInput {
                    text: characteristicController.notifiedTextValue
                    selectByMouse: true
                    readOnly: true
                }
            }

            Row {
                Text {
                    text: "Characteristic Indicate Value (Hex): "
                }
                TextInput {
                    text: characteristicController.notifiedHexValue
                    selectByMouse: true
                    readOnly: true
                }
            }
        }

        Column {
            visible: characteristicController.hasWriteProperty
            Row {
                Text {
                    text: "Write Characteristic: "
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle {
                    width: 200
                    height: childrenRect.height
                    clip: true
                    border.width: 1
                    border.color: "gray"
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 2
                    TextInput {
                        id: writeValueInput
                        text: ""
                        selectByMouse: true
                        width: parent.width
                        padding: 5
                    }
                }
                Button {
                    text: "Write"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: characteristicController.writeCharacteristic(writeValueInput.text)
                }
            }

            TextEdit {
                text: "(Hex Value. Example: 021E (MN8 start data stream))"
                readOnly: true
                selectByMouse: true
            }
        }

        Column {
            visible: characteristicController.hasReadProperty
            Button {
                text: "Read"
                onClicked: characteristicController.readCharacteristic()
            }
            Row {
                Text {
                    text: "Characteristic Read Value (Text): "
                }
                TextInput {
                    text: characteristicController.readTextValue
                    selectByMouse: true
                    readOnly: true
                }
            }
            Row {
                Text {
                    text: "Characteristic Read Value (Hex): "
                }
                TextInput {
                    text: characteristicController.readHexValue
                    selectByMouse: true
                    readOnly: true
                }
            }
        }
    }
}
