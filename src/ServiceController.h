#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <QLowEnergyController>
#include "CharacteristicModel.h"
#include "ServiceModel.h"
#include <QLowEnergyService>
#include <QObject>

class ServiceController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QLowEnergyService *service READ service WRITE setService NOTIFY serviceChanged FINAL)
    Q_PROPERTY(QLowEnergyController *lowEnergyController READ lowEnergyController
                   WRITE setLowEnergyController NOTIFY lowEnergyControllerChanged)
    Q_PROPERTY(QString serviceName READ serviceName NOTIFY serviceChanged)
    Q_PROPERTY(QString serviceUuid READ serviceUuid NOTIFY serviceChanged)
    Q_PROPERTY(QString serviceType READ serviceType NOTIFY serviceChanged)
    Q_PROPERTY(QString serviceState READ serviceState NOTIFY serviceStateChanged)
    Q_PROPERTY(QString serviceStatus READ serviceStatus NOTIFY serviceStatusChanged)
    Q_PROPERTY(ServiceModel* includedServiceModel READ includedServiceModel CONSTANT)
    Q_PROPERTY(CharacteristicModel* characteristicModel READ characteristicModel CONSTANT)
public:
    explicit ServiceController(QObject *parent = nullptr);

    QLowEnergyService *service() const;
    void setService(QLowEnergyService *service);

    QString serviceName() const;

    QString serviceUuid() const;

    QString serviceType() const;

    QString serviceState() const;

    QString serviceStatus() const;

    ServiceModel *includedServiceModel();
    CharacteristicModel* characteristicModel();

    QLowEnergyController *lowEnergyController() const;
    void setLowEnergyController(QLowEnergyController *lowEnergyController);

private slots:
    void onCharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void onCharacteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
    void onDescriptorRead(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void onDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);
    void onErrorOccurred(QLowEnergyService::ServiceError error);
    void onStateChanged(QLowEnergyService::ServiceState state);

signals:

    void serviceChanged();
    void serviceStateChanged();
    void serviceStatusChanged();
    void lowEnergyControllerChanged();

private:
    void updateIncludedServiceModel();
    void updateCharacteristicModel();

    QLowEnergyService *mService = nullptr;
    QLowEnergyController *mLowEnergyController = nullptr;

    QLowEnergyService::ServiceState mServiceState = QLowEnergyService::ServiceState::InvalidService;

    QLowEnergyService::ServiceError mServiceError = QLowEnergyService::ServiceError::NoError;

    ServiceModel mIncludedServiceModel;
    CharacteristicModel mCharacteristicModel;
};

#endif // SERVICECONTROLLER_H
