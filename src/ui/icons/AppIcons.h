// ============================================================
// AppIcons.h —— 图标统一入口
// ============================================================
#pragma once

#include <QIcon>
#include <QString>

namespace AppIcons
{
    // 取图标：自动适配当前主题，资源缺失时返回占位图
    QIcon get(const QString &name);

    // 取资源路径：需要把图标写成路径的场景用
    QString path(const QString &name);

    // 缺失图标占位图
    QIcon fallbackIcon();
}