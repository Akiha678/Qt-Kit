// ============================================================
// BleManager.cpp —— 蓝牙(BLE)连接管理器实现
// ============================================================

#include "BleManager.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QLowEnergyController>

BleManager::BleManager(QObject *parent)
    : QObject(parent)
{
    m_discovery = new QBluetoothDeviceDiscoveryAgent(this);
    m_discovery->setLowEnergyDiscoveryTimeout(8000); // 8 秒扫描窗口

    connect(m_discovery, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleManager::onDeviceDiscovered);
    connect(m_discovery, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BleManager::scanFinished);
    // Qt 5.14 的错误信号名是 error()；Qt 6 改名为 errorOccurred()。
    // 且 error 既是信号又是同名 getter（重载），必须用 QOverload 指明信号版本。
    connect(m_discovery,
            QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, [this](QBluetoothDeviceDiscoveryAgent::Error) {
                emit scanError(m_discovery->errorString());
            });
}

BleManager::~BleManager() = default;

bool BleManager::isScanning() const
{
    return m_discovery->isActive();
}

bool BleManager::isConnected() const
{
    return m_controller
        && m_controller->state() == QLowEnergyController::ConnectedState;
}

void BleManager::startScan()
{
    if (m_discovery->isActive())
        return; // 已在扫描
    m_discovery->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BleManager::stopScan()
{
    if (m_discovery->isActive())
        m_discovery->stop();
}

void BleManager::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    // 很多 BLE 外设不广播名字，显示地址也能让用户选
    QString name = info.name();
    if (name.isEmpty())
        name = tr("(未知设备)");
    emit deviceDiscovered(name, info.address().toString(), info.rssi());
}

void BleManager::connectToDevice(const QBluetoothAddress &address, const QString &name)
{
    // 上一次连接残留则先清理
    if (m_controller) {
        m_controller->deleteLater();
        m_controller = nullptr;
    }
    m_serviceUuids.clear();
    m_connectedName = name;

    // createCentral：以"中心设备"身份连接外设。
    // Qt 5.14 没有 createCentral(address, parent) 重载，需构造 QBluetoothDeviceInfo。
    QBluetoothDeviceInfo info(address, name, 0);
    m_controller = QLowEnergyController::createCentral(info, this);

    connect(m_controller, &QLowEnergyController::connected, this, [this]() {
        emit connected(m_connectedName);
        m_controller->discoverServices(); // 连接成功后自动发现服务
    });
    connect(m_controller, &QLowEnergyController::disconnected, this, [this]() {
        m_connectedName.clear();
        emit disconnected();
    });
    connect(m_controller, &QLowEnergyController::serviceDiscovered,
            this, [this](const QBluetoothUuid &uuid) {
                m_serviceUuids << uuid.toString();
            });
    connect(m_controller, &QLowEnergyController::discoveryFinished,
            this, [this]() {
                emit servicesDiscovered(m_serviceUuids);
            });
    connect(m_controller,
            QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, [this](QLowEnergyController::Error) {
                emit errorOccurred(m_controller->errorString());
            });

    m_controller->connectToDevice();
}

void BleManager::disconnectFromDevice()
{
    if (m_controller)
        m_controller->disconnectFromDevice();
}