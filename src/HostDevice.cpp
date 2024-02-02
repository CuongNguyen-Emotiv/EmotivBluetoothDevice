#include "HostDevice.h"

static constexpr auto UUID_SERVICE = "81072f40-9f3d-11e3-a9dc-0002a5d5c51b";
static constexpr auto UUID_SERVICE_MN8 = "5d1c1b00-863c-11e6-ae22-56b6b6499611";

HostDevice::HostDevice(QObject *parent)
    : QObject{parent}
{
    mDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    mDiscoveryAgent->setLowEnergyDiscoveryTimeout(10000);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &HostDevice::onDeviceDiscovered);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &HostDevice::onErrorOccurred);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &HostDevice::onFinished);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &HostDevice::onCanceled);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated,
            this, &HostDevice::onDeviceUpdated);
}

void HostDevice::startScan()
{
    mDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    setState("Scanning...");
}

void HostDevice::stopScan()
{
    if (mDiscoveryAgent->isActive())
        mDiscoveryAgent->stop();
}

void HostDevice::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if (isEmotivHeadset(device)) {
        mDeviceModel.addHeadset(device);
        mNewFoundDevices.append(device);
    }
}

void HostDevice::onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    setState("Error: " + mDiscoveryAgent->errorString() + " (" + QString::number(error) + ")");
}

void HostDevice::onFinished()
{
    for (const auto& oldDevice : mOldFoundDevices) {
        if (!mNewFoundDevices.contains(oldDevice)) {
            mDeviceModel.removeHeadset(oldDevice);
        }
    }
    mOldFoundDevices = mNewFoundDevices;
    mNewFoundDevices.clear();
    setState("Finished");
    startScan();
}

void HostDevice::onCanceled()
{
    setState("Canceled");
}

void HostDevice::onDeviceUpdated(const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields)
{
    if (isEmotivHeadset(info)) {
        mDeviceModel.addHeadset(info);
    }
}

bool HostDevice::isEmotivHeadset(const QBluetoothDeviceInfo &device) const
{
    const auto& serviceUuids = device.serviceUuids();
    return serviceUuids.contains(QBluetoothUuid(UUID_SERVICE))
           || serviceUuids.contains(QBluetoothUuid(UUID_SERVICE_MN8))
        ;
}

HeadsetModel *HostDevice::headsetModel()
{
    return &mDeviceModel;
}

QString HostDevice::state() const
{
    return mState;
}

void HostDevice::setState(const QString &state)
{
    mState = state;
    emit stateChanged();
}
