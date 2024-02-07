import QtQuick
import ServiceController 1.0
import QtQuick.Controls

Rectangle {
    id: serviceInfoRoot
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0
    color: "white"

    property var qLowEnergyServicePtr: null
    property var qLowEnergyControllerPtr: null

    ServiceController {
        id: serviceController
        service: qLowEnergyServicePtr
        lowEnergyController: qLowEnergyControllerPtr
    }

    Column {
        width: parent.width
        Row {
            Text {
                text: "Service Name: "
            }
            TextInput {
                readOnly: true
                selectByMouse: true
                text: serviceController.serviceName
            }
        }

        Row {
            Text {
                text: "Service UUID: "
            }
            TextInput {
                readOnly: true
                selectByMouse: true
                text: serviceController.serviceUuid
            }
        }

        Row {
            Text {
                text: "Service Type: "
            }
            TextInput {
                readOnly: true
                selectByMouse: true
                text: serviceController.serviceType
            }
        }

        Row {
            Text {
                text: "Service State: "
            }
            TextInput {
                readOnly: true
                selectByMouse: true
                text: serviceController.serviceState
            }
        }

        Row {
            Text {
                text: "Service Status: "
            }
            TextInput {
                readOnly: true
                selectByMouse: true
                text: serviceController.serviceStatus
            }
        }

        Text {
            text: "Characteristics: "
        }
        ListView {
            width: parent.width
            height: 300
            clip: true
            model: serviceController.characteristicModel
            delegate: Rectangle {
                width: parent ? parent.width : 0
                height: childrenRect.height
                border.width: 1
                border.color: "gray"
                Column {
                    Row {
                        Text {
                            text: qsTr("Name: ")
                        }
                        TextInput {
                            readOnly: true
                            selectByMouse: true
                            text: model.name
                        }
                    }

                    Row {
                        Text {
                            text: qsTr("UUID: ")
                        }
                        TextInput {
                            readOnly: true
                            selectByMouse: true
                            text: model.uuid
                        }
                    }

                    Row {
                        Text {
                            text: qsTr("Properties: ")
                        }
                        TextInput {
                            readOnly: true
                            selectByMouse: true
                            text: model.properties
                        }
                    }

                    Row {
                        Text {
                            text: qsTr("Text Value: ")
                        }
                        TextInput {
                            readOnly: true
                            selectByMouse: true
                            text: model.textValue
                        }
                    }

                    Row {
                        Text {
                            text: qsTr("Hex Value: ")
                        }
                        TextInput {
                            readOnly: true
                            selectByMouse: true
                            text: model.hexValue
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackview.push("qrc:/qml/CharacteristicInfo.qml",
                                       {
                                           "qLowEnergyCharacteristicPtr": model.qLowEnergyCharacteristicPtr,
                                           "qLowEnergyServicePtr": serviceInfoRoot.qLowEnergyServicePtr
                                       }
                                       );
                    }
                }
            }
        }

        Text {
            text: "Included services: "
        }
        ListView {
            width: parent.width
            height: 300
            clip: true
            model: serviceController.includedServiceModel

            delegate: Row {
                spacing: 10
                Button {
                    text: "Read"
                    onClicked: {
                        stackview.push("qrc:/qml/ServiceInfo.qml",
                                       {
                                           "qLowEnergyServicePtr": model.qLowEnergyServicePtr,
                                           "qLowEnergyControllerPtr": serviceController.service
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
}
