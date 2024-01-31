#ifndef HOSTDEVICE_H
#define HOSTDEVICE_H

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothDeviceDiscoveryAgent>

class HostDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
public:
    explicit HostDevice(QObject *parent = nullptr);

    Q_INVOKABLE void startScan();
    Q_INVOKABLE void stopScan();

    QString state() const;
    void setState(const QString &state);

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void onFinished();
    void onCanceled();

signals:
    void stateChanged();

private:
    QBluetoothDeviceDiscoveryAgent *mDiscoveryAgent;
    QString mState = "";
};

#endif // HOSTDEVICE_H
