// ============================================================
// BleConnectWidget.cpp —— 蓝牙连接页面实现（三段式）
// 操作列按钮两态：
//   未连接 → "连接"（Secondary）
//   已连接 → 该行变 "断开连接"（Danger），其余行按钮禁用
// ============================================================

#include "BleConnectWidget.h"

#include <QBluetoothAddress>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

#include <icons/AppIcons.h>
#include <widgets/AppButton.h>

#include "BleManager.h"

namespace
{
    // 表格列
    constexpr int kColName = 0;      // 设备名
    constexpr int kColMac = 1;       // MAC 地址
    constexpr int kColRssi = 2;      // 信号强度
    constexpr int kColAction = 3;    // 操作（连接/断开 按钮）

    // 表格单元格里存业务数据的角色
    constexpr int kAddressRole = Qt::UserRole;
    constexpr int kRssiRole = Qt::UserRole + 1;
}

BleConnectWidget::BleConnectWidget(QWidget *parent)
    : QWidget(parent)
    , m_manager(new BleManager(this))
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(12);

    // ================= 段②：蓝牙设备列表 =================
    auto *listSection = new QVBoxLayout;
    listSection->setSpacing(8);

    // 工具栏：扫描按钮 + 状态
    auto *toolRow = new QHBoxLayout;
    m_scanButton = new AppButton(tr("扫描设备"), AppButton::Variant::Primary, this);
    m_scanButton->setIcon(AppIcons::get(QStringLiteral("refresh")));
    m_scanButton->setCompact(true);
    connect(m_scanButton, &AppButton::clicked, this, &BleConnectWidget::onScanClicked);
    toolRow->addWidget(m_scanButton);

    m_statusLabel = new QLabel(tr("未连接"), this);
    m_statusLabel->setWordWrap(true);
    toolRow->addWidget(m_statusLabel, 1);
    listSection->addLayout(toolRow);

    // 设备表格：设备名 | MAC 地址 | 信号强度 | 操作
    m_deviceTable = new QTableWidget(0, 4, this);
    m_deviceTable->setHorizontalHeaderLabels(
        {tr("设备名"), tr("MAC 地址"), tr("信号强度"), tr("操作")});
    m_deviceTable->horizontalHeader()->setSectionResizeMode(kColName, QHeaderView::Stretch);
    m_deviceTable->horizontalHeader()->setSectionResizeMode(kColMac, QHeaderView::ResizeToContents);
    m_deviceTable->horizontalHeader()->setSectionResizeMode(kColRssi, QHeaderView::ResizeToContents);
    m_deviceTable->horizontalHeader()->setSectionResizeMode(kColAction, QHeaderView::Fixed);
    m_deviceTable->setColumnWidth(kColAction, 92);
    m_deviceTable->verticalHeader()->setDefaultSectionSize(34);
    m_deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_deviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_deviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_deviceTable->setAlternatingRowColors(true);
    listSection->addWidget(m_deviceTable, 1);
    root->addLayout(listSection, 1);

    // ================= 段③：设备信息面板 =================
    auto *infoPanel = new QWidget(this);
    infoPanel->setObjectName(QStringLiteral("bleInfoPanel")); // 预留 qss 定位
    infoPanel->setFixedWidth(250);
    auto *panelLayout = new QVBoxLayout(infoPanel);
    panelLayout->setContentsMargins(12, 12, 12, 12);
    panelLayout->setSpacing(8);

    auto *panelTitle = new QLabel(tr("设备信息"), infoPanel);
    QFont titleFont = panelTitle->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    panelTitle->setFont(titleFont);
    panelLayout->addWidget(panelTitle);

    auto *form = new QFormLayout;
    m_nameValue = new QLabel("—", infoPanel);
    m_macValue = new QLabel("—", infoPanel);
    m_rssiValue = new QLabel("—", infoPanel);
    m_serviceValue = new QLabel("—", infoPanel);
    m_stateValue = new QLabel(tr("未连接"), infoPanel);
    form->addRow(tr("设备名"), m_nameValue);
    form->addRow(tr("MAC 地址"), m_macValue);
    form->addRow(tr("信号强度"), m_rssiValue);
    form->addRow(tr("服务数量"), m_serviceValue);
    form->addRow(tr("连接状态"), m_stateValue);
    panelLayout->addLayout(form);
    panelLayout->addStretch(1);

    root->addWidget(infoPanel);

    // ================= 接线：BleManager 信号 → 界面 =================
    connect(m_manager, &BleManager::deviceDiscovered, this,
            [this](const QString &name, const QString &address, qint16 rssi) {
                appendDeviceRow(name, address, rssi);
                setStatus(tr("扫描中… 发现 %1 个设备").arg(m_deviceTable->rowCount()));
            });
    connect(m_manager, &BleManager::scanFinished, this, [this]() {
        m_scanButton->setText(tr("扫描设备"));
        setStatus(tr("扫描完成：发现 %1 个设备，点击“连接”开始连接")
                      .arg(m_deviceTable->rowCount()));
    });
    connect(m_manager, &BleManager::scanError, this, [this](const QString &message) {
        m_scanButton->setText(tr("扫描设备"));
        setStatus(tr("扫描出错：%1").arg(message));
    });
    connect(m_manager, &BleManager::connected, this, [this](const QString &name) {
        m_nameValue->setText(name.isEmpty() ? m_pendingName : name);
        m_macValue->setText(m_pendingAddress);
        m_rssiValue->setText(tr("%1 dBm").arg(m_pendingRssi));
        m_stateValue->setText(tr("已连接"));
        setRowButtonConnected(m_connectedRow, true);
        setStatus(tr("已连接 %1，正在发现服务…").arg(m_nameValue->text()));
    });
    connect(m_manager, &BleManager::servicesDiscovered, this, [this](const QStringList &uuids) {
        m_serviceValue->setText(QString::number(uuids.size()));
        setStatus(tr("已连接 %1：发现 %2 个服务").arg(m_nameValue->text()).arg(uuids.size()));
    });
    connect(m_manager, &BleManager::disconnected, this, [this]() {
        setRowButtonConnected(m_connectedRow, false);
        m_connectedRow = -1;
        resetInfoPanel();
        setAllRowButtonsEnabled(true);
        setStatus(tr("已断开连接"));
    });
    connect(m_manager, &BleManager::errorOccurred, this, [this](const QString &message) {
        m_stateValue->setText(tr("错误"));
        setRowButtonConnected(m_connectedRow, false);
        m_connectedRow = -1;
        setAllRowButtonsEnabled(true);
        setStatus(tr("错误：%1").arg(message));
    });
}

BleConnectWidget::~BleConnectWidget() = default;

// ---------- 用户操作 ----------

void BleConnectWidget::onScanClicked()
{
    if (m_manager->isScanning()) {
        m_manager->stopScan();
        m_scanButton->setText(tr("扫描设备"));
        setStatus(tr("已手动停止扫描"));
        return;
    }

    // 重新扫描前先断开当前连接，避免状态错乱
    if (m_manager->isConnected())
        m_manager->disconnectFromDevice();

    m_deviceTable->setRowCount(0); // 清空旧列表（行按钮随行删除）
    m_rowButtons.clear();
    m_connectedRow = -1;
    m_manager->startScan();
    m_scanButton->setText(tr("停止扫描"));
    setStatus(tr("扫描中…"));
}

// ---------- 列表 ----------

void BleConnectWidget::appendDeviceRow(const QString &name, const QString &address, qint16 rssi)
{
    const int row = m_deviceTable->rowCount();
    m_deviceTable->insertRow(row);

    auto *nameItem = new QTableWidgetItem(name);
    auto *macItem = new QTableWidgetItem(address);
    macItem->setData(kAddressRole, address);
    auto *rssiItem = new QTableWidgetItem(tr("%1 dBm").arg(rssi));
    rssiItem->setData(kRssiRole, rssi);

    m_deviceTable->setItem(row, kColName, nameItem);
    m_deviceTable->setItem(row, kColMac, macItem);
    m_deviceTable->setItem(row, kColRssi, rssiItem);

    // 每行一个操作按钮（紧凑尺寸）：默认"连接"，连接成功后切"断开"
    auto *button = new AppButton(tr("连接"), AppButton::Variant::Secondary, m_deviceTable);
    button->setCompact(true);
    button->setFixedSize(80, 28);
    connect(button, &AppButton::clicked, this, [this, row]() { connectRow(row); });
    m_deviceTable->setCellWidget(row, kColAction, button);
    m_rowButtons.append(button);

    if (m_connectedRow >= 0)
        button->setEnabled(false); // 已连接其他设备时禁止再连
}

void BleConnectWidget::connectRow(int row)
{
    // 该行已是"断开"状态（当前连接着它）→ 点击就是断开
    if (row == m_connectedRow && m_manager->isConnected()) {
        m_manager->disconnectFromDevice();
        setStatus(tr("正在断开…"));
        return;
    }

    auto *nameItem = m_deviceTable->item(row, kColName);
    auto *macItem = m_deviceTable->item(row, kColMac);
    auto *rssiItem = m_deviceTable->item(row, kColRssi);
    if (!nameItem || !macItem || !rssiItem)
        return;

    m_pendingName = nameItem->text();
    m_pendingAddress = macItem->data(kAddressRole).toString();
    m_pendingRssi = rssiItem->data(kRssiRole).toInt();
    m_connectedRow = row; // 乐观记录，连接失败由 error/disconnected 复位

    m_manager->connectToDevice(QBluetoothAddress(m_pendingAddress), m_pendingName);
    setAllRowButtonsEnabled(false);
    m_stateValue->setText(tr("连接中…"));
    setStatus(tr("正在连接 %1 …").arg(m_pendingName));
}

void BleConnectWidget::setAllRowButtonsEnabled(bool enabled)
{
    for (AppButton *button : m_rowButtons)
        button->setEnabled(enabled);
}

void BleConnectWidget::setRowButtonConnected(int row, bool connected)
{
    AppButton *button = m_rowButtons.value(row);
    if (!button)
        return;
    button->setText(connected ? tr("断开连接") : tr("连接"));
    button->setVariant(connected ? AppButton::Variant::Danger
                                 : AppButton::Variant::Secondary);
    button->setEnabled(true); // 断开后恢复可点
}

// ---------- 设备信息面板 ----------

void BleConnectWidget::resetInfoPanel()
{
    m_nameValue->setText("—");
    m_macValue->setText("—");
    m_rssiValue->setText("—");
    m_serviceValue->setText("—");
    m_stateValue->setText(tr("未连接"));
}

void BleConnectWidget::setStatus(const QString &text)
{
    m_statusLabel->setText(text);
}