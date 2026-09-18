// ============================================================
// ThemeManager.h —— 主题管理器
// ============================================================
#pragma once

#include <QObject>

#include "Theme.h"

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    // 单例访问入口：程序内唯一实例
    static ThemeManager &instance();

    // 当前主题
    Theme::Type currentTheme() const;

    // 切换到指定主题：加载 qss → 应用到整个应用 → 发信号
    void applyTheme(Theme::Type type);

signals:
    // 主题切换通知。界面收到后可以刷新颜色/图标等
    void themeChanged(Theme::Type type);

private:
    // 构造函数私有：外部不能 new，保证唯一实例（单例模式）
    explicit ThemeManager(QObject *parent = nullptr);

    Theme::Type m_current = Theme::Light;
};