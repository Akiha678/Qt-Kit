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
    const QString kIconRoot = QStringLiteral(":/icons/");
}

QString AppIcons::path(const QString &name)
{
    // 深色主题时调用深色图标
    if (ThemeManager::instance().currentTheme() == Theme::Dark) {
        const QString dark = kIconRoot + name + QStringLiteral("_dark.svg");
        if (QFile::exists(dark))
            return dark;
    }
    // 不存在深色图标时，使用默认图标
    return kIconRoot + name + QStringLiteral(".svg");
}

QIcon AppIcons::get(const QString &name)
{
    const QString p = path(name);
    return QFile::exists(p) ? QIcon(p) : fallbackIcon();
}

// 占位图，图片加载失败时，调用此图
QIcon AppIcons::fallbackIcon()
{
    // 占位图懒生成一次，之后复用
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