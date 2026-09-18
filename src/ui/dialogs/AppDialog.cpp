// ============================================================
// AppDialog.cpp —— 通用弹窗组件实现
// ============================================================

#include "AppDialog.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <icons/AppImages.h>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>

AppDialog::AppDialog(QWidget *parent) : QDialog(parent)
{
    // ---------- 头部：图标 + 标题 ----------
    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(32, 32);

    m_titleLabel = new QLabel(this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(11);
    m_titleLabel->setFont(titleFont);

    auto *header = new QWidget(this);
    header->setObjectName(QStringLiteral("appDialogHeader")); // 供 QSS 用 ID 选择器定位
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->addWidget(m_iconLabel);
    headerLayout->addSpacing(8);
    headerLayout->addWidget(m_titleLabel, 1);

    // ---------- 内容区：留一个"容器"，内容由 setContentWidget 填入 ----------
    m_contentHost = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(m_contentHost);
    contentLayout->setContentsMargins(16, 0, 16, 0);

    // ---------- 底部：主题状态 + 按钮 ----------
    m_statusLabel = new QLabel(this);
    m_buttons = new QDialogButtonBox(this);
    // 按钮角色 → 自动触发 accept/reject（QDialogButtonBox 的约定）
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *footer = new QWidget(this);
    auto *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(16, 8, 4, 12);
    footerLayout->addWidget(m_statusLabel);
    footerLayout->addStretch();
    footerLayout->addWidget(m_buttons);

    // ---------- 组装 ----------
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(4);
    root->addWidget(header);
    root->addWidget(m_contentHost, 1); // 内容区占满剩余空间
    root->addWidget(footer);

    // 订阅主题切换：弹窗打开期间换肤也会实时刷新
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this]() { refreshThemeUi(); });

    refreshThemeUi();
}

// ---------- 三步用法实现 ----------

void AppDialog::setTitleText(const QString &text)
{
    m_titleLabel->setText(text);
    setWindowTitle(text); // 原生标题栏也同步
}

void AppDialog::setContentWidget(QWidget *content)
{
    if (auto *layout = m_contentHost->layout()) {
        while (QLayoutItem *item = layout->takeAt(0)) {
            if (QWidget *w = item->widget())
                w->deleteLater();
            delete item;
        }
    }
    if (content)
        m_contentHost->layout()->addWidget(content);
}

QPushButton *AppDialog::addButton(const QString &text, QDialogButtonBox::ButtonRole role)
{
    return m_buttons->addButton(text, role);
}

// ---------- 主题响应 ----------

void AppDialog::refreshThemeUi()
{
    updateHeaderIcon(); // 图标随主题换变体（AppIcons 内部处理）
    m_statusLabel->setText(tr("主题：%1")
                               .arg(Theme::typeName(ThemeManager::instance().currentTheme())));
}

void AppDialog::updateHeaderIcon()
{
    // 头部图标 = 应用真实 Logo（AppImages 统一入口）
    m_iconLabel->setPixmap(AppImages::logo().scaled(32, 32,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation));
}