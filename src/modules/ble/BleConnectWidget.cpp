// ============================================================
// BleConnectWidget.cpp —— 蓝牙连接页面实现
// ============================================================

#include "BleConnectWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <icons/AppIcons.h>
#include <widgets/AppButton.h>

BleConnectWidget::BleConnectWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    // 页面标题
    auto *title = new QLabel(tr("蓝牙连接"), this);
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // 占位说明
    auto *hint = new QLabel(tr("扫描、连接、数据收发的界面将在这里展开。\n"
                               "对应的业务逻辑放在 modules/ble（后续的 BleManager），"
                               "界面只负责展示与交互。"), this);
    hint->setWordWrap(true);
    layout->addWidget(hint);

    // 骨架阶段按钮先禁用，等 BleManager 落地后启用并接信号
    auto *scanButton = new AppButton(tr("扫描蓝牙设备"), AppButton::Variant::Primary, this);
    scanButton->setIcon(AppIcons::get(QStringLiteral("bluetooth")));
    scanButton->setEnabled(false);
    layout->addWidget(scanButton);

    layout->addStretch();
}