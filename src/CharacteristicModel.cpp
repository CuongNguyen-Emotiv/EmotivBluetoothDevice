#include "CharacteristicModel.h"

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

CharacteristicItem::CharacteristicItem(const QLowEnergyCharacteristic &characteristic)
    : mCharacteristic{characteristic}
{

}

QString CharacteristicItem::name() const
{
    return mCharacteristic.name();
}

QString CharacteristicItem::properties() const
{
    QString result;
    auto properties = mCharacteristic.properties();
    if (properties & QLowEnergyCharacteristic::Broadcasting)
        result += "Broadcasting, ";
    if (properties & QLowEnergyCharacteristic::Read)
        result += "Read, ";
    if (properties & QLowEnergyCharacteristic::WriteNoResponse)
        result += "WriteNoResponse, ";
    if (properties & QLowEnergyCharacteristic::Write)
        result += "Write, ";
    if (properties & QLowEnergyCharacteristic::Notify)
        result += "Notify, ";
    if (properties & QLowEnergyCharacteristic::Indicate)
        result += "Indicate, ";
    if (properties & QLowEnergyCharacteristic::WriteSigned)
        result += "WriteSigned, ";
    if (properties & QLowEnergyCharacteristic::ExtendedProperty)
        result += "ExtendedProperty, ";
    return result;
}

QString CharacteristicItem::uuid() const
{
    const QBluetoothUuid uuid = mCharacteristic.uuid();
    bool success = false;
    quint16 result16 = uuid.toUInt16(&success);
    if (success)
        return "0x" + QString::number(result16, 16);

    quint32 result32 = uuid.toUInt32(&success);
    if (success)
        return "0x" + QString::number(result32, 16);

    return uuid.toString().remove('{').remove('}');
}

QString CharacteristicItem::textValue() const
{
    QByteArray a = mCharacteristic.value();
    QString result;
    if (a.isEmpty()) {
        result = "<none>";
        return result;
    }

    result = a;
    return result;
}

QString CharacteristicItem::hexValue() const
{
    QByteArray value = mCharacteristic.value();
    QString result;
    if (value.isEmpty()) {
        result = "<none>";
        return result;
    }

    result = value.toHex(':');
    return result;
}
