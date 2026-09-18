// ============================================================
// AppIcons.cpp —— 图标统一入口实现
// ============================================================

#include "AppIcons.h"

#include <QFile>
#include <QPainter>
#include <QPixmap>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>

namespace
{
    // 资源根：与 resources/icons/icons.qrc 的 prefix="/icons" 对应。
    // 注意：资源 URL 必须带扩展名（:/icons/bluetooth.svg），
    //        QFile::exists 不会帮你补全。
    const QString kIconRoot = QStringLiteral(":/icons/");
}

QString AppIcons::path(const QString &name)
{
    // 深色主题优先用 name_dark 变体；不存在则回退常规图标
    if (ThemeManager::instance().currentTheme() == Theme::Dark) {
        const QString dark = kIconRoot + name + QStringLiteral("_dark.svg");
        if (QFile::exists(dark))
            return dark;
    }
    return kIconRoot + name + QStringLiteral(".svg");
}

QIcon AppIcons::get(const QString &name)
{
    const QString p = path(name);
    return QFile::exists(p) ? QIcon(p) : fallbackIcon();
}

QIcon AppIcons::fallbackIcon()
{
    // 占位图懒生成一次，之后复用（不每帧重画）
    static QIcon icon;
    if (icon.isNull()) {
        QPixmap pm(24, 24);
        pm.fill(Qt::transparent);

        QPainter painter(&pm);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x99, 0x99, 0x99)); // 中性灰
        painter.drawRoundedRect(0, 0, 24, 24, 5, 5);
        painter.end();

        icon = QIcon(pm);
    }
    return icon;
}