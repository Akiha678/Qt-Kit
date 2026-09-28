#include "WifiManageWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <icons/AppIcons.h>
#include <widgets/AppButton.h>

WifiManageWidget::WifiManageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    auto *title = new QLabel(tr("WiFi管理"), this);
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    auto *hint = new QLabel(tr("WiFi 扫描、热点连接与网络状态配置的界面将在这里展开。\n"
                               "对应的业务逻辑放在 modules/wifi。"), this);
    hint->setWordWrap(true);
    layout->addWidget(hint);

    auto *scanButton = new AppButton(tr("扫描热点"), AppButton::Variant::Primary, this);
    scanButton->setIcon(AppIcons::get(QStringLiteral("wifi")));
    scanButton->setEnabled(false);
    layout->addWidget(scanButton);

    layout->addStretch();
}