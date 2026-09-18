// ============================================================
// MainWindow.h —— 主窗口类声明
// ============================================================
#pragma once

#include <QMainWindow>

class NavBar;
class QIcon;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    // 添加一个导航页：图标 + 悬停提示 + 页面控件（所有权转移给本窗口）。
    // 返回页索引（从 0 开始，与导航项一一对应）
    int addPage(const QIcon &icon, const QString &title, QWidget *page);

private:
    void buildNavigation();    // 组装 NavBar + QStackedWidget
    void createMenus();        // 菜单栏
    void createViewMenu();     // "视图"菜单：主题切换演示
    void createToolsMenu();    // "工具"菜单：弹窗组件演示
    void createStatusBar();    // 状态栏

    NavBar *m_navBar = nullptr;
    QStackedWidget *m_pages = nullptr;
};