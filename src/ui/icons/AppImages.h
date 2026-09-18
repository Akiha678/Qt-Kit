// ============================================================
// AppImages.h —— 应用图片资源统一入口
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