// ============================================================
// SerialConnectWidget.h —— 串口连接页面（modules/serial）
// 与 BleConnectWidget 同理：属于业务模块 serial，页面跟随模块。
// ============================================================
#pragma once

#include <QWidget>

class SerialConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialConnectWidget(QWidget *parent = nullptr);
};