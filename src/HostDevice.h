#ifndef HOSTDEVICE_H
#define HOSTDEVICE_H

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothDeviceDiscoveryAgent>
#include "HeadsetModel.h"

class HostDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(HeadsetModel* headsetModel READ headsetModel CONSTANT)
public:
    explicit HostDevice(QObject *parent = nullptr);

    Q_INVOKABLE void startScan();
    Q_INVOKABLE void stopScan();

    QString state() const;
    void setState(const QString &state);

    HeadsetModel* headsetModel();

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void onFinished();
    void onCanceled();
    void onDeviceUpdated(const QBluetoothDeviceInfo &info, QBluetoothDeviceInfo::Fields updatedFields);

signals:
    void stateChanged();

private:
    bool isEmotivHeadset(const QBluetoothDeviceInfo &device) const;

    QBluetoothDeviceDiscoveryAgent *mDiscoveryAgent;
    QString mState = "";
    HeadsetModel mDeviceModel;
    QList<HeadsetItem> mOldFoundDevices;
    QList<HeadsetItem> mNewFoundDevices;
};

#endif // HOSTDEVICE_H
