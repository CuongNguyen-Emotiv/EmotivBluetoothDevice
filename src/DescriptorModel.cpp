#include "DescriptorModel.h"
#include "BluetoothUtility.h"

DescriptorModel::DescriptorModel(QObject *parent)
    : QAbstractListModel{parent}
{}


int DescriptorModel::rowCount(const QModelIndex &parent) const
{
    return mDescriptors.count();
}

QVariant DescriptorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= mDescriptors.count())
        return {};

    const auto &descriptor = mDescriptors.at(index.row());
    switch (role) {
    case IS_VALID_ROLE:
        return descriptor.isValid();
    case NAME_ROLE:
        return descriptor.name();
    case TYPE_ROLE:
        return descriptorType(descriptor.type());
    case UUID_ROLE:
        return BluetoothUtility::uuidToString(descriptor.uuid());
    case VALUE_ROLE:
        return BluetoothUtility::valueToHex(descriptor.value());
    }
    return {};
}

QHash<int, QByteArray> DescriptorModel::roleNames() const
{
    return {
        {IS_VALID_ROLE, "isValid"},
        {NAME_ROLE, "name"},
        {TYPE_ROLE, "type"},
        {UUID_ROLE, "uuid"},
        {VALUE_ROLE, "value"},
    };
}

void DescriptorModel::clear()
{
    beginResetModel();
    mDescriptors.clear();
    endResetModel();
}

void DescriptorModel::addDescriptor(const QLowEnergyDescriptor &descriptor)
{
    beginInsertRows({}, mDescriptors.count(), mDescriptors.count());
    mDescriptors.append(descriptor);
    endInsertRows();
}

QString DescriptorModel::descriptorType(const QBluetoothUuid::DescriptorType &descriptorType) const
{
    switch (descriptorType) {
    case QBluetoothUuid::DescriptorType::UnknownDescriptorType:
        return "Unknown";
    case QBluetoothUuid::DescriptorType::CharacteristicExtendedProperties:
        return "Characteristic Extended Properties";
    case QBluetoothUuid::DescriptorType::CharacteristicUserDescription:
        return "Characteristic User Description";
    case QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration:
        return "Client Characteristic Configuration";
    case QBluetoothUuid::DescriptorType::ServerCharacteristicConfiguration:
        return "Server Characteristic Configuration";
    case QBluetoothUuid::DescriptorType::CharacteristicPresentationFormat:
        return "Characteristic Presentation Format";
    case QBluetoothUuid::DescriptorType::CharacteristicAggregateFormat:
        return "Characteristic Aggregate Format";
    case QBluetoothUuid::DescriptorType::ValidRange:
        return "Valid Range";
    case QBluetoothUuid::DescriptorType::ExternalReportReference:
        return "External Report Reference";
    case QBluetoothUuid::DescriptorType::ReportReference:
        return "Report Reference";
    case QBluetoothUuid::DescriptorType::EnvironmentalSensingConfiguration:
        return "Environmental Sensing Configuration";
    case QBluetoothUuid::DescriptorType::EnvironmentalSensingMeasurement:
        return "Environmental Sensing Measurement";
    case QBluetoothUuid::DescriptorType::EnvironmentalSensingTriggerSetting:
        return "Environmental Sensing Trigger Setting";
    default:
        return "";
    }
}
