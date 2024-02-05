#include "HeadsetController.h"

HeadsetController::HeadsetController(QObject *parent)
    : QObject(parent)
{
    connect(&mConnectTimeout, &QTimer::timeout, this, [this]() {
        if (mLowEnergyController && mLowEnergyController->state() == QLowEnergyController::ConnectingState) {
            mLowEnergyController->disconnectFromDevice();
        }
    });
    mConnectTimeout.setInterval(10000);
    mConnectTimeout.setSingleShot(true);

    connect(&mRssiReader, &QTimer::timeout, this, [this]() {
        if (mLowEnergyController && mLowEnergyController->state() != QLowEnergyController::UnconnectedState) {
            mLowEnergyController->readRssi();
        }
    });
    mRssiReader.setInterval(1000);
    mRssiReader.setSingleShot(false);
}

HeadsetController::~HeadsetController()
{
    if (mLowEnergyController) {
        mLowEnergyController->disconnectFromDevice();
    }
}

void HeadsetController::connectToHeadset(const QVariant &bluetoothDeviceInfo)
{
    auto device = bluetoothDeviceInfo.value<QBluetoothDeviceInfo>();

    if (mLowEnergyController) {
        mLowEnergyController->disconnectFromDevice();
        delete mLowEnergyController;
        mLowEnergyController = nullptr;
    }

    mLowEnergyController = QLowEnergyController::createCentral(device, this);
    emit lowEnergyControllerChanged();

    connect(mLowEnergyController, &QLowEnergyController::connected,
            this, &HeadsetController::onConnected);
    connect(mLowEnergyController, &QLowEnergyController::connectionUpdated,
            this, &HeadsetController::onConnectionUpdated);
    connect(mLowEnergyController, &QLowEnergyController::disconnected,
            this, &HeadsetController::onDisconnected);
    connect(mLowEnergyController, &QLowEnergyController::discoveryFinished,
            this, &HeadsetController::onDiscoveryFinished);
    connect(mLowEnergyController, &QLowEnergyController::errorOccurred,
            this, &HeadsetController::onErrorOccurred);
    connect(mLowEnergyController, &QLowEnergyController::mtuChanged,
            this, &HeadsetController::onMtuChanged);
    connect(mLowEnergyController, &QLowEnergyController::rssiRead,
            this, &HeadsetController::onRssiRead);
    connect(mLowEnergyController, &QLowEnergyController::serviceDiscovered,
            this, &HeadsetController::onServiceDiscovered);
    connect(mLowEnergyController, &QLowEnergyController::stateChanged,
            this, &HeadsetController::onStateChanged);

    // Connect to the device
    mLowEnergyController->connectToDevice();
    mConnectTimeout.start();
}

void HeadsetController::disconnectFromHeadset()
{
    if (mLowEnergyController) {
        mLowEnergyController->disconnectFromDevice();
    }
}

void HeadsetController::onConnected()
{
    setConnectStatus("Connected");
    mRssiReader.start();
    if (mLowEnergyController) {
        mServiceModel.clear();
        mLowEnergyController->discoverServices();
    }
}

void HeadsetController::onConnectionUpdated(const QLowEnergyConnectionParameters &parameters)
{
    qInfo() << "Connection updated: " << parameters.latency() << ", " << parameters.maximumInterval()
            << ", " << parameters.minimumInterval() << ", " << parameters.supervisionTimeout();
}

void HeadsetController::onDisconnected()
{
    setConnectStatus("Disconnected");
    mRssiReader.stop();
}

void HeadsetController::onDiscoveryFinished()
{
    qInfo() << "Discovery finished";
}

void HeadsetController::onErrorOccurred(QLowEnergyController::Error error)
{
    qInfo() << "Error occurred: " << error;
}

void HeadsetController::onMtuChanged(int mtu)
{
    setMtu(mtu);
}

void HeadsetController::onServiceDiscovered(const QBluetoothUuid &serviceUuid)
{
    QLowEnergyService *service = mLowEnergyController->createServiceObject(serviceUuid, mLowEnergyController);
    if (!service) {
        qWarning() << "HeadsetController Cannot create service for uuid" << serviceUuid;
        return;
    }
    mServiceModel.addService(service);
}

void HeadsetController::onStateChanged(QLowEnergyController::ControllerState state)
{
    mConnectTimeout.stop();
    switch (state) {
    case QLowEnergyController::ControllerState::UnconnectedState:
        setConnectStatus("Disconnected");
        setRssi(0);
        setMtu(0);
        mServiceModel.clear();
        break;
    case QLowEnergyController::ControllerState::ConnectingState:
        setConnectStatus("Connecting...");
        setRssi(0);
        setMtu(0);
        mServiceModel.clear();
        break;
    case QLowEnergyController::ControllerState::ConnectedState:
        setConnectStatus("Connected");
        break;
    case QLowEnergyController::ControllerState::DiscoveringState:
        setConnectStatus("Discovering...");
        break;
    case QLowEnergyController::ControllerState::DiscoveredState:
        setConnectStatus("Discovered");
        break;
    case QLowEnergyController::ControllerState::ClosingState:
        setConnectStatus("Closing...");
        break;
    case QLowEnergyController::ControllerState::AdvertisingState:
        setConnectStatus("Advertising...");
        break;
    default:
        break;
    }
}

void HeadsetController::onRssiRead(qint16 rssi)
{
    setRssi(rssi);
}

void HeadsetController::setRssi(qint16 rssi)
{
    if (mRssi == rssi)
        return;
    mRssi = rssi;
    emit rssiChanged();
}

void HeadsetController::setMtu(int mtu)
{
    if (mMtu == mtu)
        return;
    mMtu = mtu;
    emit mtuChanged();
}

QLowEnergyController *HeadsetController::lowEnergyController() const
{
    return mLowEnergyController;
}

ServiceModel* HeadsetController::serviceModel()
{
    return &mServiceModel;
}

QString HeadsetController::connectStatus() const
{
    return mConnectStatus;
}

void HeadsetController::setConnectStatus(const QString &connectStatus)
{
    if (mConnectStatus == connectStatus)
        return;
    mConnectStatus = connectStatus;
    emit connectStatusChanged();
}

int HeadsetController::mtu() const
{
    return mMtu;
}

qint16 HeadsetController::rssi() const
{
    return mRssi;
}
