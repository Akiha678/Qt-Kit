// ============================================================
// AppImages.cpp —— 应用图片资源统一入口实现
// ============================================================

#include "AppImages.h"

namespace
{
    // 与 resources/images.qrc 的 prefix="/images" 对应
    const QString kLogoPath = QStringLiteral(":/images/iris_logo.png");
}

QPixmap AppImages::logo()
{
    return QPixmap(kLogoPath);
}

QIcon AppImages::logoIcon()
{
    return QIcon(QPixmap(kLogoPath));
}