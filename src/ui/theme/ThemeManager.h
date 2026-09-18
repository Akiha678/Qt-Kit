// ============================================================
// ThemeManager.h —— 主题管理器（单例）
// 职责：
//   1. 全局只此一份（单例）：任何地方都能 ThemeManager::instance() 换主题
//   2. 加载 qss 样式表并应用（qApp->setStyleSheet）
//   3. 换主题后发 themeChanged 信号，通知所有界面按需刷新
// 与 Theme.h 的分工：Theme.h 定义"有哪些主题"，
//                  ThemeManager 定义"怎么应用主题"。
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