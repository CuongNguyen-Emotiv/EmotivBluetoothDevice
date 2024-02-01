#include "HeadsetModel.h"

HeadsetModel::HeadsetModel(QObject *parent)
    : QAbstractListModel{parent}
{}


int HeadsetModel::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return static_cast<int>(mHeadsetItems.size());
}

QVariant HeadsetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &device = mHeadsetItems.at(index.row());

    switch (role) {
    case static_cast<int>(EmotiveDeviceRole::NAME_ROLE):
        return device.mBluetoothDeviceInfo.name();
    case static_cast<int>(EmotiveDeviceRole::BLUETOOTH_DEVICE_INFO_ROLE):
        return QVariant::fromValue(device.mBluetoothDeviceInfo);
    case static_cast<int>(EmotiveDeviceRole::ADDRESS_ROLE):
        return device.address();
    case static_cast<int>(EmotiveDeviceRole::RSSI_ROLE):
        return device.mBluetoothDeviceInfo.rssi();
    default:
        return QVariant();
    }
}

void HeadsetModel::addHeadset(const HeadsetItem &device)
{
    if (mHeadsetItems.contains(device)) {
        const auto& row = mHeadsetItems.indexOf(device);
        mHeadsetItems[row] = device;
        emit dataChanged(index(row), index(row));
    }
    else {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        mHeadsetItems << device;
        endInsertRows();
    }
}

void HeadsetModel::removeHeadset(const HeadsetItem &device)
{
    if (mHeadsetItems.contains(device)) {
        const auto& row = mHeadsetItems.indexOf(device);
        beginRemoveRows(QModelIndex(), row, row);
        mHeadsetItems.removeAt(row);
        endRemoveRows();
    }
}

void HeadsetModel::clear()
{
    beginResetModel();
    mHeadsetItems.clear();
    endResetModel();
}


QHash<int, QByteArray> HeadsetModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(EmotiveDeviceRole::NAME_ROLE)] = "name";
    roles[static_cast<int>(EmotiveDeviceRole::BLUETOOTH_DEVICE_INFO_ROLE)] = "bluetoothDeviceInfo";
    roles[static_cast<int>(EmotiveDeviceRole::ADDRESS_ROLE)] = "address";
    roles[static_cast<int>(EmotiveDeviceRole::RSSI_ROLE)] = "rssi";
    return roles;
}
