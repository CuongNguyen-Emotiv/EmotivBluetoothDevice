#include "CharacteristicModel.h"
#include "BluetoothUtility.h"

CharacteristicModel::CharacteristicModel(QObject *parent)
    : QAbstractListModel{parent}
{}


int CharacteristicModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(mCharacteristics.size());
}

QVariant CharacteristicModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &characteristic = mCharacteristics.at(index.row());
    switch (role) {
    case NAME_ROLE:
        return characteristic.name();
    case PROPERTIES_ROLE:
        return characteristic.properties();
    case UUID_ROLE:
        return characteristic.uuid();
    case TEXT_VALUE_ROLE:
        return characteristic.textValue();
    case HEX_VALUE_ROLE:
        return characteristic.hexValue();
    case Q_LOWENERGY_CHARACTERISTIC_PTR_ROLE:
        return QVariant::fromValue(characteristic.characteristic());
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> CharacteristicModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(CharacteristicRoles::NAME_ROLE)] = "name";
    roles[static_cast<int>(CharacteristicRoles::PROPERTIES_ROLE)] = "properties";
    roles[static_cast<int>(CharacteristicRoles::UUID_ROLE)] = "uuid";
    roles[static_cast<int>(CharacteristicRoles::TEXT_VALUE_ROLE)] = "textValue";
    roles[static_cast<int>(CharacteristicRoles::HEX_VALUE_ROLE)] = "hexValue";
    roles[static_cast<int>(CharacteristicRoles::Q_LOWENERGY_CHARACTERISTIC_PTR_ROLE)] = "qLowEnergyCharacteristicPtr";
    return roles;
}

void CharacteristicModel::clear()
{
    beginResetModel();
    mCharacteristics.clear();
    endResetModel();
}

void CharacteristicModel::addCharacteristic(const CharacteristicItem &characteristic)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mCharacteristics << characteristic;
    endInsertRows();
}

CharacteristicItem::CharacteristicItem(const QLowEnergyCharacteristic& characteristic)
{
    mCharacteristic = characteristic;
}

QString CharacteristicItem::name() const
{
    return mCharacteristic.name();
}

QString CharacteristicItem::properties() const
{
    auto properties = mCharacteristic.properties();
    return BluetoothUtility::propertiesToString(properties);
}

QString CharacteristicItem::uuid() const
{
    auto uuid = mCharacteristic.uuid();
    return BluetoothUtility::uuidToString(uuid);
}

QString CharacteristicItem::textValue() const
{
    auto value = mCharacteristic.value();
    return BluetoothUtility::valueToText(value);
}

QString CharacteristicItem::hexValue() const
{
    auto value = mCharacteristic.value();
    return BluetoothUtility::valueToHex(value);
}

QLowEnergyCharacteristic *CharacteristicItem::characteristic() const
{
    return const_cast<QLowEnergyCharacteristic*>(&mCharacteristic);
}
