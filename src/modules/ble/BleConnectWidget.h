// ============================================================
// BleConnectWidget.h —— 蓝牙连接页面（modules/ble 的"界面层"）
// 只做展示与交互：把 BleManager 的信号变成界面状态，
// 把用户点击变成 BleManager 的调用。业务逻辑全在 BleManager。
// ============================================================
#pragma once

#include <QWidget>

class BleManager;
class QLabel;
class QListWidget;
class AppButton;

class BleConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BleConnectWidget(QWidget *parent = nullptr);
    ~BleConnectWidget() override;

private slots:
    void onScanClicked();       // 扫描 / 停止扫描（同一按钮两态）
    void onConnectClicked();    // 连接列表中选中的设备
    void onDisconnectClicked();

private:
    void updateButtons();       // 按当前状态启用/禁用按钮
    void setStatus(const QString &text);

    BleManager *m_manager = nullptr;
    QString m_connectedName;    // 当前连接的设备名（用于状态显示）
    QLabel *m_statusLabel = nullptr;
    QListWidget *m_deviceList = nullptr;
    AppButton *m_scanButton = nullptr;
    AppButton *m_connectButton = nullptr;
    AppButton *m_disconnectButton = nullptr;
};