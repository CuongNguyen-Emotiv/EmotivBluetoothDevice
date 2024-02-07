#ifndef CHARACTERISTICCONTROLLER_H
#define CHARACTERISTICCONTROLLER_H

#include "DescriptorModel.h"
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QObject>

class CharacteristicController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QLowEnergyCharacteristic *characteristic READ characteristic WRITE setCharacteristic NOTIFY characteristicChanged)
    Q_PROPERTY(QLowEnergyService *service READ service WRITE setService NOTIFY serviceChanged)
    Q_PROPERTY(QString characteristicUuid READ characteristicUuid NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicName READ characteristicName NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicProperties READ characteristicProperties NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicTextValue READ characteristicTextValue NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicHexValue READ characteristicHexValue NOTIFY characteristicChanged)
    Q_PROPERTY(bool hasNotifyProperty READ hasNotifyProperty NOTIFY characteristicChanged)
    Q_PROPERTY(QString notifiedHexValue READ notifiedHexValue NOTIFY notifiedValueChanged)
    Q_PROPERTY(QString notifiedTextValue READ notifiedTextValue NOTIFY notifiedValueChanged)
    Q_PROPERTY(bool hasWriteProperty READ hasWriteProperty NOTIFY characteristicChanged)
    Q_PROPERTY(DescriptorModel* descriptorModel READ descriptorModel CONSTANT)
    Q_PROPERTY(bool hasReadProperty READ hasReadProperty NOTIFY characteristicChanged)
    Q_PROPERTY(QString readTextValue READ readTextValue NOTIFY readValueChanged)
    Q_PROPERTY(QString readHexValue READ readHexValue NOTIFY readValueChanged)
    Q_PROPERTY(bool hasIndicateProperty READ hasIndicateProperty NOTIFY characteristicChanged)
public:
    explicit CharacteristicController(QObject *parent = nullptr);

    QLowEnergyCharacteristic *characteristic() const;
    void setCharacteristic(QLowEnergyCharacteristic *characteristic);

    QLowEnergyService *service() const;
    void setService(QLowEnergyService *service);

    QString characteristicUuid() const;
    QString characteristicName() const;
    QString characteristicProperties() const;
    QString characteristicTextValue() const;
    QString characteristicHexValue() const;

    bool hasNotifyProperty() const;
    bool hasWriteProperty() const;
    bool hasReadProperty() const;
    bool hasIndicateProperty() const;

    Q_INVOKABLE void startNotify();
    Q_INVOKABLE void stopNotify();

    Q_INVOKABLE void writeCharacteristic(const QString &value);

    Q_INVOKABLE void readCharacteristic();

    Q_INVOKABLE void startIndicate();
    Q_INVOKABLE void stopIndicate();

    DescriptorModel* descriptorModel();

    QString notifiedHexValue() const;
    QString notifiedTextValue() const;

    QString readHexValue() const;
    QString readTextValue() const;

signals:
    void notifiedValueChanged();
    void readValueChanged();
    void characteristicChanged();
    void serviceChanged();

private slots:
    void onCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void onCharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);

private:
    void updateDescriptorModel();

    QLowEnergyCharacteristic *mCharacteristic = nullptr;
    QLowEnergyService *mService = nullptr;

    QByteArray mNotifiedValue;
    QByteArray mReadValue;

    DescriptorModel mDescriptorModel;
};

#endif // CHARACTERISTICCONTROLLER_H
