#include "CharacteristicController.h"
#include "BluetoothUtility.h"

CharacteristicController::CharacteristicController(QObject *parent)
    : QObject{parent}
{}

QLowEnergyCharacteristic *CharacteristicController::characteristic() const
{
    return mCharacteristic;
}

void CharacteristicController::setCharacteristic(QLowEnergyCharacteristic *characteristic)
{
    if (mCharacteristic == characteristic)
        return;
    mCharacteristic = characteristic;
    emit characteristicChanged();

    updateDescriptorModel();
}

QLowEnergyService *CharacteristicController::service() const
{
    return mService;
}

void CharacteristicController::setService(QLowEnergyService *service)
{
    if (mService == service)
        return;
    mService = service;
    emit serviceChanged();

    connect(mService, &QLowEnergyService::characteristicChanged, this, &CharacteristicController::onCharacteristicChanged);
    connect(mService, &QLowEnergyService::characteristicRead, this, &CharacteristicController::onCharacteristicRead);
}

QString CharacteristicController::characteristicUuid() const
{
    if (!mCharacteristic)
        return {};
    return BluetoothUtility::uuidToString(mCharacteristic->uuid());
}

QString CharacteristicController::characteristicName() const
{
    if (!mCharacteristic)
        return {};
    return mCharacteristic->name();
}

QString CharacteristicController::characteristicProperties() const
{
    if (!mCharacteristic)
        return {};
    return BluetoothUtility::propertiesToString(mCharacteristic->properties());
}

QString CharacteristicController::characteristicTextValue() const
{
    if (!mCharacteristic)
        return {};
    return BluetoothUtility::valueToText(mCharacteristic->value());
}

QString CharacteristicController::characteristicHexValue() const
{
    if (!mCharacteristic)
        return {};
    return BluetoothUtility::valueToHex(mCharacteristic->value());
}

bool CharacteristicController::hasNotifyProperty() const
{
    if (!mCharacteristic)
        return false;
    return mCharacteristic->properties() & QLowEnergyCharacteristic::Notify;
}

bool CharacteristicController::hasWriteProperty() const
{
    if (!mCharacteristic)
        return false;
    return mCharacteristic->properties() & QLowEnergyCharacteristic::Write;
}

bool CharacteristicController::hasReadProperty() const
{
    if (!mCharacteristic)
        return false;
    return mCharacteristic->properties() & QLowEnergyCharacteristic::Read;
}

bool CharacteristicController::hasIndicateProperty() const
{
    if (!mCharacteristic)
        return false;
    return mCharacteristic->properties() & QLowEnergyCharacteristic::Indicate;
}

void CharacteristicController::startNotify()
{
    if (!mCharacteristic)
        return;
    auto ccc = mCharacteristic->clientCharacteristicConfiguration();
    if (ccc.isValid() && mService) {
        mService->writeDescriptor(ccc, QLowEnergyCharacteristic::CCCDEnableNotification);
    }
}

void CharacteristicController::stopNotify()
{
    if (!mCharacteristic)
        return;
    auto ccc = mCharacteristic->clientCharacteristicConfiguration();
    if (ccc.isValid() && mService) {
        mService->writeDescriptor(ccc, QLowEnergyCharacteristic::CCCDDisable);
    }
}

void CharacteristicController::writeCharacteristic(const QString &value)
{
    if (!mCharacteristic || value.isEmpty())
        return;
    mService->writeCharacteristic(*mCharacteristic, QByteArray::fromHex(value.toUtf8()));
}

void CharacteristicController::readCharacteristic()
{
    if (!mCharacteristic)
        return;
    mService->readCharacteristic(*mCharacteristic);
}

void CharacteristicController::startIndicate()
{
    if (!mCharacteristic)
        return;
    auto ccc = mCharacteristic->clientCharacteristicConfiguration();
    if (ccc.isValid() && mService) {
        mService->writeDescriptor(ccc, QLowEnergyCharacteristic::CCCDEnableIndication);
    }
}

void CharacteristicController::stopIndicate()
{
    if (!mCharacteristic)
        return;
    auto ccc = mCharacteristic->clientCharacteristicConfiguration();
    if (ccc.isValid() && mService) {
        mService->writeDescriptor(ccc, QLowEnergyCharacteristic::CCCDDisable);
    }
}

void CharacteristicController::onCharacteristicChanged(
    const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    if (mCharacteristic->uuid() != characteristic.uuid())
        return;
    mNotifiedValue = value;
    emit notifiedValueChanged();
}

void CharacteristicController::onCharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    if (mCharacteristic->uuid() != characteristic.uuid())
        return;
    mReadValue = value;
    emit readValueChanged();
}

void CharacteristicController::updateDescriptorModel()
{
    mDescriptorModel.clear();
    if (!mCharacteristic)
        return;
    for (const auto &descriptor : mCharacteristic->descriptors()) {
        mDescriptorModel.addDescriptor(descriptor);
    }
}

DescriptorModel *CharacteristicController::descriptorModel()
{
    return &mDescriptorModel;
}

QString CharacteristicController::notifiedHexValue() const
{
    return BluetoothUtility::valueToHex(mNotifiedValue);
}

QString CharacteristicController::notifiedTextValue() const
{
    return BluetoothUtility::valueToText(mNotifiedValue);
}

QString CharacteristicController::readHexValue() const
{
    return BluetoothUtility::valueToHex(mReadValue);
}

QString CharacteristicController::readTextValue() const
{
    return BluetoothUtility::valueToText(mReadValue);
}
