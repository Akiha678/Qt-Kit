// ============================================================
// SerialConnectWidget.cpp —— 串口连接页面实现
// ============================================================

#include "SerialConnectWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <icons/AppIcons.h>
#include <widgets/AppButton.h>

SerialConnectWidget::SerialConnectWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    auto *title = new QLabel(tr("串口连接"), this);
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    auto *hint = new QLabel(tr("端口选择、波特率配置、打开/关闭串口的界面将在这里展开。\n"
                               "对应的业务逻辑放在 modules/serial（后续的 SerialManager）。"), this);
    hint->setWordWrap(true);
    layout->addWidget(hint);

    // 骨架阶段按钮先禁用，等 SerialManager 落地后再启用
    auto *openButton = new AppButton(tr("打开串口"), AppButton::Variant::Primary, this);
    openButton->setIcon(AppIcons::get(QStringLiteral("serial")));
    openButton->setEnabled(false);
    layout->addWidget(openButton);

    layout->addStretch();
}