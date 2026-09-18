// ============================================================
// AppImages.h —— 应用图片资源统一入口（ui 层）
// 与 AppIcons 分工：
//   AppIcons  → 小图标（SVG，主题变体、换肤）
//   AppImages → 大图/品牌图（Logo、插图、宣传图）
// 铁律同 AppIcons：禁止业务代码手写 ":/images/xxx" 字符串。
// 素材统一放 resources/images/，由 images.qrc 编译进程序（app 目标）。
// ============================================================
#pragma once

#include <QIcon>
#include <QPixmap>

namespace AppImages
{
    // 应用 Logo（原始尺寸，1024x1024；需要显示时自行 scaled）
    QPixmap logo();

    // 应用 Logo 图标：用作窗口/任务栏图标（可缩放）
    QIcon logoIcon();
}