#include "ServiceModel.h"

ServiceModel::ServiceModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ServiceModel::addService(const ServiceItem &service)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mServices << service;
    endInsertRows();
}

void ServiceModel::clear()
{
    beginResetModel();
    mServices.clear();
    endResetModel();
}


int ServiceModel::rowCount([[maybe_unused]] const QModelIndex &parent) const
{
    return static_cast<int>(mServices.size());
}

QVariant ServiceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &service = mServices.at(index.row());

    switch (role) {
    case static_cast<int>(ServiceRole::NAME_ROLE):
        return service.service()->serviceName();
    case static_cast<int>(ServiceRole::TYPE_ROLE):
        return service.type();
    case static_cast<int>(ServiceRole::Q_LOW_ENERGY_SERVICE_PTR_ROLE):
        return QVariant::fromValue(service.service());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ServiceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(ServiceRole::NAME_ROLE)] = "name";
    roles[static_cast<int>(ServiceRole::TYPE_ROLE)] = "type";
    roles[static_cast<int>(ServiceRole::Q_LOW_ENERGY_SERVICE_PTR_ROLE)] = "qLowEnergyServicePtr";
    return roles;
}

QString ServiceItem::type() const
{
    QString result;
    if (mService->type() & QLowEnergyService::PrimaryService)
        result += "primary";
    else
        result += "secondary";

    if (mService->type() & QLowEnergyService::IncludedService)
        result += " included";

    result.prepend('<').append('>');

    return result;
}
