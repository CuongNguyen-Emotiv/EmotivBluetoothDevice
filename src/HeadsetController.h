#ifndef HEADSETCONTROLLER_H
#define HEADSETCONTROLLER_H

#include "ServiceModel.h"
#include <QObject>
#include <QLowEnergyController>
#include <QTimer>

class HeadsetController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connectStatus READ connectStatus WRITE setConnectStatus NOTIFY connectStatusChanged)
    Q_PROPERTY(int mtu READ mtu NOTIFY mtuChanged)
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
    Q_PROPERTY(ServiceModel* serviceModel READ serviceModel CONSTANT)
public:
    explicit HeadsetController(QObject *parent = nullptr);
    ~HeadsetController();
    Q_INVOKABLE void connectToHeadset(const QVariant& bluetoothDeviceInfo);
    Q_INVOKABLE void disconnectFromHeadset();

    QString connectStatus() const;
    void setConnectStatus(const QString &connectStatus);

    int mtu() const;

    qint16 rssi() const;

    ServiceModel* serviceModel();

signals:

    void connectStatusChanged();
    void mtuChanged();
    void rssiChanged();

private slots:
    void onConnected();
    void onConnectionUpdated(const QLowEnergyConnectionParameters &parameters);
    void onDisconnected();
    void onDiscoveryFinished();
    void onErrorOccurred(QLowEnergyController::Error error);
    void onMtuChanged(int mtu);
    void onRssiRead(qint16 rssi);
    void onServiceDiscovered(const QBluetoothUuid &service);
    void onStateChanged(QLowEnergyController::ControllerState state);

private:
    void setRssi(qint16 rssi);
    void setMtu(int mtu);

    QLowEnergyController* mLowEnergyController = nullptr;
    QString mConnectStatus = "Unconnected";
    QTimer mConnectTimeout;

    QTimer mRssiReader;
    qint16 mRssi = 0;

    int mMtu = 0;

    ServiceModel mServiceModel;
};

#endif // HEADSETCONTROLLER_H
