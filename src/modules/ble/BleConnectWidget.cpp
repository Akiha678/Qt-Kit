// ============================================================
// BleConnectWidget.cpp —— 蓝牙连接页面实现
// 界面 ↔ 逻辑的接线示范：
//   · BleManager 信号 → 界面状态（列表/状态栏/按钮）
//   · 用户点击      → BleManager 调用
// ============================================================

#include "BleConnectWidget.h"

#include <QBluetoothAddress>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

#include <icons/AppIcons.h>
#include <widgets/AppButton.h>

#include "BleManager.h"

namespace
{
    // QListWidgetItem 里存业务数据的角色
    constexpr int kAddressRole = Qt::UserRole;     // 设备地址（QString）
    constexpr int kNameRole = Qt::UserRole + 1;    // 设备显示名（QString）
}

BleConnectWidget::BleConnectWidget(QWidget *parent)
    : QWidget(parent)
    , m_manager(new BleManager(this)) // 页面持有管理器（模块自包含）
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    // 标题
    auto *title = new QLabel(tr("蓝牙连接"), this);
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // 状态行
    m_statusLabel = new QLabel(tr("未连接"), this);
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);

    // 扫描按钮
    m_scanButton = new AppButton(tr("扫描设备"), AppButton::Variant::Primary, this);
    m_scanButton->setIcon(AppIcons::get(QStringLiteral("refresh")));
    connect(m_scanButton, &AppButton::clicked, this, &BleConnectWidget::onScanClicked);
    layout->addWidget(m_scanButton);

    // 设备列表
    m_deviceList = new QListWidget(this);
    m_deviceList->setMinimumHeight(200);
    layout->addWidget(m_deviceList, 1);

    // 连接 / 断开 按钮行
    auto *buttonRow = new QHBoxLayout;
    m_connectButton = new AppButton(tr("连接"), AppButton::Variant::Primary, this);
    m_connectButton->setIcon(AppIcons::get(QStringLiteral("bluetooth")));
    m_disconnectButton = new AppButton(tr("断开"), AppButton::Variant::Danger, this);
    m_disconnectButton->setIcon(AppIcons::get(QStringLiteral("bluetooth")));
    connect(m_connectButton, &AppButton::clicked, this, &BleConnectWidget::onConnectClicked);
    connect(m_disconnectButton, &AppButton::clicked, this, &BleConnectWidget::onDisconnectClicked);
    buttonRow->addWidget(m_connectButton);
    buttonRow->addWidget(m_disconnectButton);
    layout->addLayout(buttonRow);

    // ---------- 接线：BleManager 信号 → 界面 ----------
    connect(m_manager, &BleManager::deviceDiscovered, this,
            [this](const QString &name, const QString &address, qint16 rssi) {
                auto *item = new QListWidgetItem;
                item->setText(QStringLiteral("%1  (%2 dBm)\n%3").arg(name).arg(rssi).arg(address));
                item->setData(kAddressRole, address);
                item->setData(kNameRole, name);
                m_deviceList->addItem(item); // 只 add 一次（item 无父构造，不会重复加入）
                m_statusLabel->setText(tr("扫描中… 发现 %1 个设备").arg(m_deviceList->count()));
            });
    connect(m_manager, &BleManager::scanFinished, this, [this]() {
        setStatus(tr("扫描完成：发现 %1 个设备。请选择后点击“连接”。")
                      .arg(m_deviceList->count()));
        updateButtons();
    });
    connect(m_manager, &BleManager::scanError, this, [this](const QString &message) {
        setStatus(tr("扫描出错：%1").arg(message));
        updateButtons();
    });
    connect(m_manager, &BleManager::connected, this, [this](const QString &name) {
        m_connectedName = name;
        setStatus(tr("已连接：%1（正在发现服务…）").arg(name));
        updateButtons();
    });
    connect(m_manager, &BleManager::servicesDiscovered, this, [this](const QStringList &uuids) {
        setStatus(tr("已连接：%1，发现 %2 个服务").arg(m_connectedName).arg(uuids.size()));
    });
    connect(m_manager, &BleManager::disconnected, this, [this]() {
        m_connectedName.clear();
        setStatus(tr("已断开连接"));
        updateButtons();
    });
    connect(m_manager, &BleManager::errorOccurred, this, [this](const QString &message) {
        setStatus(tr("错误：%1").arg(message));
        updateButtons();
    });

    updateButtons();
}

BleConnectWidget::~BleConnectWidget() = default;

// ---------- 用户操作 ----------

void BleConnectWidget::onScanClicked()
{
    if (m_manager->isScanning()) {
        m_manager->stopScan();
        setStatus(tr("已手动停止扫描"));
    } else {
        m_deviceList->clear();
        m_manager->startScan();
        setStatus(tr("扫描中…"));
    }
    updateButtons();
}

void BleConnectWidget::onConnectClicked()
{
    auto *item = m_deviceList->currentItem();
    if (!item) {
        setStatus(tr("请先在列表中选择一个设备"));
        return;
    }
    m_manager->connectToDevice(
        QBluetoothAddress(item->data(kAddressRole).toString()),
        item->data(kNameRole).toString());
    setStatus(tr("正在连接…"));
    updateButtons();
}

void BleConnectWidget::onDisconnectClicked()
{
    m_manager->disconnectFromDevice();
}

// ---------- 界面状态 ----------

void BleConnectWidget::updateButtons()
{
    m_scanButton->setText(m_manager->isScanning() ? tr("停止扫描") : tr("扫描设备"));
    m_connectButton->setEnabled(!m_manager->isConnected() && m_deviceList->currentItem());
    m_disconnectButton->setEnabled(m_manager->isConnected());
}

void BleConnectWidget::setStatus(const QString &text)
{
    m_statusLabel->setText(text);
}