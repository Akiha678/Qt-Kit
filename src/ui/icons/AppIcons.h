// ============================================================
// AppIcons.h —— 图标统一入口（ui 层）
// 企业标准做法，两条铁律：
//   1. 【禁止手写资源字符串】业务代码一律 AppIcons::get("name")，
//      不允许散落 ":/icons/xxx.svg" 字面量（改名/迁移时全项目遭殃）
//   2. 【资源统一归口】图标素材统一放 resources/icons/，
//      由 icons.qrc 编译进程序（前缀 /icons），新增图标只需两步：
//         放 SVG 文件 → 在 icons.qrc 里登记
// 主题适配约定：深色变体命名 name_dark.svg（app.svg ↔ app_dark.svg），
//               AppIcons 按当前主题自动挑选。
// ============================================================
#pragma once

#include <QIcon>
#include <QString>

namespace AppIcons
{
    // 取图标：自动适配当前主题，资源缺失时返回占位图（绝不返回空图标）
    QIcon get(const QString &name);

    // 取资源路径：需要把图标写成路径的场景用
    // （如 QSS 的 background-image: url(...)、QSvgRenderer 等）
    QString path(const QString &name);

    // 缺失图标占位图（灰色圆角方块），保证界面永远有图可显
    QIcon fallbackIcon();
}