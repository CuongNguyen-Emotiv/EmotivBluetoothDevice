#include "ServiceController.h"
#include "BluetoothUtility.h"

ServiceController::ServiceController(QObject *parent)
    : QObject{parent}
{}

QLowEnergyService *ServiceController::service() const
{
    return mService;
}

void ServiceController::setService(QLowEnergyService *service)
{
    if (mService == service)
        return;
    mService = service;
    emit serviceChanged();

    if (mService) {
        onStateChanged(mService->state());
        onErrorOccurred(mService->error());
    }
    else {
        onStateChanged(QLowEnergyService::InvalidService);
        onErrorOccurred(QLowEnergyService::UnknownError);
        mIncludedServiceModel.clear();
        mCharacteristicModel.clear();
        return;
    }

    connect(mService, &QLowEnergyService::characteristicRead, this, &ServiceController::onCharacteristicRead);
    connect(mService, &QLowEnergyService::characteristicWritten, this, &ServiceController::onCharacteristicWritten);
    connect(mService, &QLowEnergyService::descriptorRead, this, &ServiceController::onDescriptorRead);
    connect(mService, &QLowEnergyService::descriptorWritten, this, &ServiceController::onDescriptorWritten);
    connect(mService, &QLowEnergyService::errorOccurred, this, &ServiceController::onErrorOccurred);
    connect(mService, &QLowEnergyService::stateChanged, this, &ServiceController::onStateChanged);
}

QString ServiceController::serviceName() const
{
    if (!mService)
        return {};
    return mService->serviceName();
}

QString ServiceController::serviceUuid() const
{
    if (!mService)
        return QString();

    auto uuid = mService->serviceUuid();
    return BluetoothUtility::uuidToString(uuid);
}

QString ServiceController::serviceType() const
{
    if (!mService)
        return {};
    QString result;
    if (mService->type() & QLowEnergyService::PrimaryService)
        result += "Primary";
    else
        result += "Secondary";

    if (mService->type() & QLowEnergyService::IncludedService)
        result += " Included";

    return result;
}

QString ServiceController::serviceState() const
{
    switch (mServiceState) {
    case QLowEnergyService::ServiceState::InvalidService:
        return "Invalid Service";
    case QLowEnergyService::ServiceState::RemoteService:
        return "Remote Service";
    case QLowEnergyService::ServiceState::RemoteServiceDiscovering:
        return "Remote Service Discovering";
    case QLowEnergyService::ServiceState::RemoteServiceDiscovered:
        return "Remote Service Discovered";
    case QLowEnergyService::ServiceState::LocalService:
        return "Local Service";
    default:
        Q_ASSERT_X(false, "Invalid state ", QString::number(static_cast<int>(mServiceState)).toUtf8().constData());
        return "";
    }
}

QString ServiceController::serviceStatus() const
{
    switch (mServiceError) {
    case QLowEnergyService::ServiceError::NoError:
        return "No Error";
    case QLowEnergyService::ServiceError::OperationError:
        return "Operation Error";
    case QLowEnergyService::ServiceError::CharacteristicWriteError:
        return "Characteristic Write Error";
    case QLowEnergyService::ServiceError::DescriptorWriteError:
        return "Descriptor Write Error";
    case QLowEnergyService::ServiceError::UnknownError:
        return "Unknown Error";
    case QLowEnergyService::ServiceError::CharacteristicReadError:
        return "Characteristic Read Error";
    case QLowEnergyService::ServiceError::DescriptorReadError:
        return "Descriptor Read Error";
    default:
        Q_ASSERT_X(false, "Invalid error ", QString::number(static_cast<int>(mServiceError)).toUtf8().constData());
        return "";
    }
}

void ServiceController::onStateChanged(QLowEnergyService::ServiceState state)
{
    switch (state) {
    case QLowEnergyService::ServiceState::RemoteService:
        mService->discoverDetails();
        break;
    case QLowEnergyService::ServiceState::RemoteServiceDiscovered:
        updateIncludedServiceModel();
        updateCharacteristicModel();
        break;
    default:
        break;
    }
    mServiceState = state;
    emit serviceStateChanged();
}

void ServiceController::updateIncludedServiceModel()
{
    for(const auto& serviceUuid : mService->includedServices()) {
        auto includedService = mLowEnergyController->createServiceObject(serviceUuid, this);
        if (!includedService) {
            qWarning() << "ServiceController Cannot create service for uuid" << serviceUuid;
            return;
        }
        mIncludedServiceModel.addService(includedService);
    }
}

void ServiceController::updateCharacteristicModel()
{
    mCharacteristicModel.clear();
    for (auto& characteristic : mService->characteristics()) {
        mCharacteristicModel.addCharacteristic(characteristic);
    }
}

QLowEnergyController *ServiceController::lowEnergyController() const
{
    return mLowEnergyController;
}

void ServiceController::setLowEnergyController(QLowEnergyController *lowEnergyController)
{
    if (mLowEnergyController == lowEnergyController)
        return;
    mLowEnergyController = lowEnergyController;
    emit lowEnergyControllerChanged();
}

ServiceModel* ServiceController::includedServiceModel()
{
    return &mIncludedServiceModel;
}

CharacteristicModel *ServiceController::characteristicModel()
{
    return &mCharacteristicModel;
}

void ServiceController::onErrorOccurred(QLowEnergyService::ServiceError error)
{
    mServiceError = error;
    emit serviceStatusChanged();
}

void ServiceController::onDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue)
{
    qInfo() << "Descriptor written: " << descriptor.name()
            << descriptor.isValid() << descriptor.type() << descriptor.uuid() << descriptor.value()
            << " new value: " << newValue;
}

void ServiceController::onDescriptorRead(const QLowEnergyDescriptor &descriptor, const QByteArray &value)
{
    qInfo() << "Descriptor read: " << descriptor.name()
            << descriptor.isValid() << descriptor.type() << descriptor.uuid() << descriptor.value()
            << " value: " << value;
}

void ServiceController::onCharacteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
{
    qInfo() << "Characteristic written: " << characteristic.name()
            << characteristic.isValid() << characteristic.uuid() << characteristic.value()
            << " new value: " << newValue;
}

void ServiceController::onCharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qInfo() << "Characteristic read: " << characteristic.name()
            << characteristic.isValid() << characteristic.uuid() << characteristic.value()
            << " value: " << value;
}
