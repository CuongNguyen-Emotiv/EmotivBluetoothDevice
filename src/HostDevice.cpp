#include "HostDevice.h"

HostDevice::HostDevice(QObject *parent)
    : QObject{parent}
{
    mDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    mDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &HostDevice::onDeviceDiscovered);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &HostDevice::onErrorOccurred);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &HostDevice::onFinished);
    connect(mDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &HostDevice::onCanceled);
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
    if (device.name() == "MN8 65D8") {
        qDebug() << "Found new device -------------:";
        qDebug() << "Address:" << device.address();
        qDebug() << "Core config:" << device.coreConfigurations();
        qDebug() << "Device uuid:" << device.deviceUuid();
        qDebug() << "Is cached:" << device.isCached();
        qDebug() << "Is valid:" << device.isValid();
        qDebug() << "Major device class:" << device.majorDeviceClass();
        qDebug() << "Manufacturer Data:" << device.manufacturerData();
        qDebug() << "Manufacturer Ids:" << device.manufacturerIds();
        qDebug() << "Minor Device Class:" << device.minorDeviceClass();
        qDebug() << "Name:" << device.name();
        qDebug() << "rssi:" << device.rssi();
        qDebug() << "Service classes:" << device.serviceClasses();
        qDebug() << "Service data:" << device.serviceData();
        qDebug() << "Service Ids:" << device.serviceIds();
        qDebug() << "Service uuids:" << device.serviceUuids();
    }
}

void HostDevice::onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    setState("Error: " + mDiscoveryAgent->errorString() + " (" + QString::number(error) + ")");
}

void HostDevice::onFinished()
{
    setState("Finished");
}

void HostDevice::onCanceled()
{
    setState("Canceled");
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
