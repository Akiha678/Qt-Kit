// ============================================================
// BleConnectWidget.h —— 蓝牙连接页面（三段式）
// 布局：
//   ① 左侧导航栏（MainWindow 的 NavBar，本页只管内容区）
//   ② 蓝牙设备列表：设备名 | MAC 地址 | 信号强度 | 操作
//      操作列按钮两态：未连接="连接"，已连接="断开连接"
//   ③ 设备信息面板：连接成功后显示设备详情
// 业务逻辑全部在 BleManager，本类只做展示与交互。
// ============================================================
#pragma once

#include <QWidget>

class BleManager;
class AppButton;
class QLabel;
class QTableWidget;

class BleConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BleConnectWidget(QWidget *parent = nullptr);
    ~BleConnectWidget() override;

private slots:
    void onScanClicked(); // 扫描 / 停止扫描（同一按钮两态）

private:
    // 列表操作
    void appendDeviceRow(const QString &name, const QString &address, qint16 rssi);
    void connectRow(int row); // 该行按钮：未连接→连接；已连接(本行)→断开
    void setAllRowButtonsEnabled(bool enabled);
    void setRowButtonConnected(int row, bool connected); // 按钮切到"断开/连接"外观

    // 信息面板
    void resetInfoPanel();
    void setStatus(const QString &text);

    BleManager *m_manager = nullptr;
    QLabel *m_statusLabel = nullptr;
    AppButton *m_scanButton = nullptr;
    QTableWidget *m_deviceTable = nullptr;
    QList<AppButton *> m_rowButtons; // 每行一个操作按钮，便于批量启停

    // 设备信息面板控件
    QLabel *m_nameValue = nullptr;
    QLabel *m_macValue = nullptr;
    QLabel *m_rssiValue = nullptr;
    QLabel *m_serviceValue = nullptr;
    QLabel *m_stateValue = nullptr;

    // 当前连接目标（点击连接时记录，供信息面板显示）
    QString m_pendingName;
    QString m_pendingAddress;
    int m_pendingRssi = 0;
    int m_connectedRow = -1; // 当前已连接的行号；-1 = 未连接
};