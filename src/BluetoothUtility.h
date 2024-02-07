#ifndef BLUETOOTHUTILITY_H
#define BLUETOOTHUTILITY_H

#include <QString>
#include <QBluetoothUuid>
#include <QLowEnergyCharacteristic>

class BluetoothUtility
{
public:
    static QString uuidToString(const QBluetoothUuid &uuid);
    static QString propertiesToString(const QLowEnergyCharacteristic::PropertyTypes &properties);
    static QString valueToText(const QByteArray &value);
    static QString valueToHex(const QByteArray &value);
};

#endif // BLUETOOTHUTILITY_H
