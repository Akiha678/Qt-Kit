// ============================================================
// NavBar.h —— 侧边导航栏组件
// ============================================================
#pragma once

#include <QList>
#include <QWidget>

class QButtonGroup;
class QIcon;
class QToolButton;
class QVBoxLayout;

class NavBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavBar(QWidget *parent = nullptr);

    // 追加一个导航项（QIcon 形式）
    int addItem(const QIcon &icon, const QString &toolTip);
    // 追加一个导航项（图标名称形式，支持主题切换时自动更新）
    int addItem(const QString &iconName, const QString &toolTip);

    // 程序化选中某项
    void setCurrentIndex(int index);

signals:
    // 当前选中页面
    void currentChanged(int index);

private slots:
    void onThemeChanged();

private:
    struct NavItem {
        QToolButton *button = nullptr;
        QString iconName;
    };

    QButtonGroup *m_group = nullptr;
    QVBoxLayout *m_mainLayout = nullptr;
    QVBoxLayout *m_itemsLayout = nullptr;
    QList<NavItem> m_items;
    QToolButton *m_themeButton = nullptr;
};