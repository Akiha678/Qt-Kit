// ============================================================
// NavBar.cpp —— 侧边导航栏组件实现
// ============================================================

#include "NavBar.h"

#include <QButtonGroup>
#include <QToolButton>
#include <QVBoxLayout>

NavBar::NavBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("navBar")); // 供 QSS：QWidget#navBar（两侧主题已配好）
    setFixedWidth(56);                       // 图标栏宽度固定

    m_group = new QButtonGroup(this);
    m_group->setExclusive(true);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(8, 12, 8, 12);
    m_layout->setSpacing(6);
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
    m_layout->addWidget(button);

    // 默认
    if (id == 0)
        button->setChecked(true);

    // 点击切换界面
    connect(button, &QToolButton::clicked, this,
            [this, id]() { emit currentChanged(id); });

    return id;
}

void NavBar::setCurrentIndex(int index)
{
    if (QAbstractButton *button = m_group->button(index))
        button->setChecked(true);
}