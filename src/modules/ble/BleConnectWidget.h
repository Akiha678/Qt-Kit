// ============================================================
// BleConnectWidget.h —— 蓝牙连接页面（modules/ble）
// 本页属于"业务模块 ble"，因此放在模块目录里（模块自包含），
// 而不是 ui 层 —— 换/删模块时，它的界面一起走。
// 骨架阶段仅做布局；后续接 BleManager 提供扫描/连接能力。
// ============================================================
#pragma once

#include <QWidget>

class BleConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BleConnectWidget(QWidget *parent = nullptr);
};