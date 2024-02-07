#include "BluetoothUtility.h"

QString BluetoothUtility::uuidToString(const QBluetoothUuid &uuid)
{
    QString ret;
    bool success = false;
    quint16 result16 = uuid.toUInt16(&success);
    if (success)
        ret += (" 0x" + QString::number(result16, 16));

    quint32 result32 = uuid.toUInt32(&success);
    if (success)
        ret += (" 0x" + QString::number(result32, 16));

    ret += (" " + uuid.toString().remove('{').remove('}'));
    return ret;
}

QString BluetoothUtility::propertiesToString(const QLowEnergyCharacteristic::PropertyTypes &properties)
{
    QString result;
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

QString BluetoothUtility::valueToText(const QByteArray &value)
{
    QString result;
    if (value.isEmpty()) {
        result = "<none>";
        return result;
    }

    result = value;
    return result;
}

QString BluetoothUtility::valueToHex(const QByteArray &value)
{
    QString result;
    if (value.isEmpty()) {
        result = "<none>";
        return result;
    }

    result = value.toHex(':');
    return result;
}
