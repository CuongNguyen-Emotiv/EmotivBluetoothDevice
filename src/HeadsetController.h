#ifndef HEADSETCONTROLLER_H
#define HEADSETCONTROLLER_H

#include <QObject>
#include <QLowEnergyController>
#include <QTimer>

class HeadsetController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connectStatus READ connectStatus WRITE setConnectStatus NOTIFY connectStatusChanged)
    Q_PROPERTY(int mtu READ mtu NOTIFY mtuChanged)
    Q_PROPERTY(qint16 rssi READ rssi NOTIFY rssiChanged)
public:
    explicit HeadsetController(QObject *parent = nullptr);
    ~HeadsetController();
    Q_INVOKABLE void connectToHeadset(const QVariant& bluetoothDeviceInfo);
    Q_INVOKABLE void disconnectFromHeadset();

    QString connectStatus() const;
    void setConnectStatus(const QString &connectStatus);

    int mtu() const;

    qint16 rssi() const;

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
    void onRssiRead(qint16 rssi);
    void onServiceDiscovered(const QBluetoothUuid &service);
    void onStateChanged(QLowEnergyController::ControllerState state);

private:
    void setRssi(qint16 rssi);

    QLowEnergyController* mLowEnergyController = nullptr;
    QString mConnectStatus = "Unconnected";
    QTimer mConnectTimeout;

    QTimer mRssiReader;
    qint16 mRssi = 0;
};

#endif // HEADSETCONTROLLER_H
