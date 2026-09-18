// ============================================================
// BleManager.h —— 蓝牙(BLE)连接管理器
// ============================================================
#pragma once

#include <QBluetoothAddress>
#include <QObject>
#include <QStringList>

class QBluetoothDeviceDiscoveryAgent;
class QBluetoothDeviceInfo;
class QLowEnergyController;

class BleManager : public QObject
{
    Q_OBJECT

public:
    explicit BleManager(QObject *parent = nullptr);
    ~BleManager() override;

    bool isScanning() const;
    bool isConnected() const;

    void startScan();   // 开始扫描 BLE 设备（异步）
    void stopScan();

    // 连接指定设备（address 由扫描结果提供，name 仅用于展示）
    void connectToDevice(const QBluetoothAddress &address, const QString &name);
    void disconnectFromDevice();

signals:
    // 发现一个设备（界面据此填充列表）
    void deviceDiscovered(const QString &name, const QString &address, qint16 rssi);
    void scanFinished();
    void scanError(const QString &message);

    // 连接状态
    void connected(const QString &deviceName);
    void disconnected();
    void servicesDiscovered(const QStringList &serviceUuids);
    void errorOccurred(const QString &message);

private:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);

    QBluetoothDeviceDiscoveryAgent *m_discovery = nullptr;
    QLowEnergyController *m_controller = nullptr;
    QString m_connectedName;
    QStringList m_serviceUuids;
};