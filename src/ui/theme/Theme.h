// ============================================================
// Theme.h —— 主题的"定义"
// ============================================================
#pragma once

#include <QColor>
#include <QString>

namespace Theme
{
    // 内置主题：浅色 / 深色
    enum Type { Light, Dark };

    // 主题显示名
    inline QString typeName(Type t)
    {
        return t == Dark ? QStringLiteral("深色") : QStringLiteral("浅色");
    }

    inline QColor accentColor(Type t)
    {
        return t == Dark ? QColor(0x6a, 0x9b, 0xff) // 深色主题
                         : QColor(0x2f, 0x6f, 0xdc); // 浅色主题
    }
}