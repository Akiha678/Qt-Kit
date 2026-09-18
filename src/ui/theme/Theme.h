// ============================================================
// Theme.h —— 主题的"定义"（枚举 + 名称 + 色板）
// 只放与界面无关的静态数据，不含任何应用逻辑。
// 分出头文件的好处：谁想了解"有哪些主题、用什么颜色"，
// 看这一个文件就够了；ThemeManager 只负责"怎么应用"。
// ============================================================
#pragma once

#include <QColor>
#include <QString>

namespace Theme
{
    // 内置主题：浅色 / 深色（以后要加"高对比度"等，在这里扩展）
    enum Type { Light, Dark };

    // 主题显示名（用于菜单、设置界面）
    inline QString typeName(Type t)
    {
        return t == Dark ? QStringLiteral("深色") : QStringLiteral("浅色");
    }

    // 主题强调色（accent color）：QSS 覆盖不到的"运行时绘制"场景
    // （自绘图标、paintEvent 里的画笔、装饰线）从这里取色。
    // 注意：能靠 QSS 解决的尽量用 QSS，这里只补 QSS 的盲区。
    inline QColor accentColor(Type t)
    {
        return t == Dark ? QColor(0x6a, 0x9b, 0xff) // 深色主题：亮蓝
                         : QColor(0x2f, 0x6f, 0xdc); // 浅色主题：标准蓝
    }
}