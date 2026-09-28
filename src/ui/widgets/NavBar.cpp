// ============================================================
// NavBar.cpp —— 侧边导航栏组件实现
// ============================================================

#include "NavBar.h"

#include <QButtonGroup>
#include <QToolButton>
#include <QVBoxLayout>
#include <icons/AppIcons.h>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>

NavBar::NavBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("navBar")); // 供 QSS：QWidget#navBar
    setFixedWidth(56);                       // 图标栏宽度固定

    m_group = new QButtonGroup(this);
    m_group->setExclusive(true);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(8, 12, 8, 12);
    m_mainLayout->setSpacing(6);

    m_itemsLayout = new QVBoxLayout;
    m_itemsLayout->setContentsMargins(0, 0, 0, 0);
    m_itemsLayout->setSpacing(6);
    m_mainLayout->addLayout(m_itemsLayout);

    m_mainLayout->addStretch(1);

    // 底部切换主题按钮
    m_themeButton = new QToolButton(this);
    m_themeButton->setObjectName(QStringLiteral("navItem"));
    m_themeButton->setIcon(AppIcons::get(QStringLiteral("settings")));
    m_themeButton->setIconSize(QSize(24, 24));
    m_themeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_themeButton->setToolTip(tr("切换深浅主题"));
    m_themeButton->setCursor(Qt::PointingHandCursor);
    m_themeButton->setFixedSize(40, 40);

    connect(m_themeButton, &QToolButton::clicked, this, []() {
        const Theme::Type nextTheme = (ThemeManager::instance().currentTheme() == Theme::Light)
                                          ? Theme::Dark
                                          : Theme::Light;
        ThemeManager::instance().applyTheme(nextTheme);
    });

    m_mainLayout->addWidget(m_themeButton);

    // 监听主题切换，刷新所有带 iconName 的图标
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &NavBar::onThemeChanged);
}

int NavBar::addItem(const QIcon &icon, const QString &toolTip)
{
    auto *button = new QToolButton(this);
    button->setObjectName(QStringLiteral("navItem"));
    button->setIcon(icon);
    button->setIconSize(QSize(24, 24)); // 设定图标大小
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setToolTip(toolTip);
    button->setCursor(Qt::PointingHandCursor);
    button->setCheckable(true);   // 允许选中状态
    button->setFixedSize(40, 40);

    // 每个按钮一个固定 id（= 索引），点谁就用 id 发信号
    const int id = static_cast<int>(m_group->buttons().size());
    m_group->addButton(button, id);
    m_itemsLayout->addWidget(button);

    // 默认选中第一项
    if (id == 0)
        button->setChecked(true);

    // 点击切换界面
    connect(button, &QToolButton::clicked, this,
            [this, id]() { emit currentChanged(id); });

    m_items.append({button, QString()});
    return id;
}

int NavBar::addItem(const QString &iconName, const QString &toolTip)
{
    const int id = addItem(AppIcons::get(iconName), toolTip);
    if (id >= 0 && id < m_items.size()) {
        m_items[id].iconName = iconName;
    }
    return id;
}

void NavBar::setCurrentIndex(int index)
{
    if (QAbstractButton *button = m_group->button(index))
        button->setChecked(true);
}

void NavBar::onThemeChanged()
{
    for (const auto &item : m_items) {
        if (item.button && !item.iconName.isEmpty()) {
            item.button->setIcon(AppIcons::get(item.iconName));
        }
    }
    if (m_themeButton) {
        m_themeButton->setIcon(AppIcons::get(QStringLiteral("settings")));
    }
}