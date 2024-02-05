#ifndef SERVICEMODEL_H
#define SERVICEMODEL_H

#include <QLowEnergyService>
#include <QAbstractListModel>

class ServiceItem : public QObject
{
    Q_OBJECT
public:
    ServiceItem() = default;
    ServiceItem(const ServiceItem &other) : mService(other.mService) { mService->setParent(this); }
    ServiceItem &operator=(const ServiceItem &other) {
        mService = other.mService;
        mService->setParent(this);
        return *this;
    }
    ServiceItem(QLowEnergyService *service) : mService(service) {
        mService->setParent(this);
    }

    QString type() const;

    QLowEnergyService *service() const { return mService; }

private:
    QLowEnergyService *mService;
};

class ServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class ServiceRole {
        NAME_ROLE = Qt::UserRole + 1,
        TYPE_ROLE,
        Q_LOW_ENERGY_SERVICE_PTR_ROLE,
    };
    explicit ServiceModel(QObject *parent = nullptr);

    void addService(const ServiceItem &service);
    void clear();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<ServiceItem> mServices;
};

#endif // SERVICEMODEL_H
