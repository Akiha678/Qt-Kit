// ============================================================
// AppDialog.h —— 通用弹窗组件（ui 层共享控件）
// ============================================================
#pragma once

#include <QDialog>
#include <QDialogButtonBox>

class QLabel;
class QPushButton;

class AppDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppDialog(QWidget *parent = nullptr);

    void setTitleText(const QString &text);   // 标题
    void setContentWidget(QWidget *content);  // 内容区
    QPushButton *addButton(const QString &text, QDialogButtonBox::ButtonRole role); // 底部按钮

private:
    // 主题刷新：换肤后更新头部图标（AppIcons 按主题自动换变体）
    // + 底部状态文案（这两块 QSS 覆盖不到，需要代码响应）
    void refreshThemeUi();
    void updateHeaderIcon(); // 从 AppIcons 取图标（app.svg ↔ app_dark.svg）

    QLabel *m_iconLabel   = nullptr;
    QLabel *m_titleLabel  = nullptr;
    QLabel *m_statusLabel = nullptr;  // 底部主题状态
    QWidget *m_contentHost = nullptr; // 内容容器
    QDialogButtonBox *m_buttons = nullptr;
};