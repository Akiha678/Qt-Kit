// ============================================================
// MainWindow.h —— 主窗口类声明（界面层）
// 布局：左侧 NavBar（图标导航）+ 右侧 QStackedWidget（页面容器）。
// 关键设计：MainWindow 只提供"导航框架"，不认识任何具体业务模块——
//   addPage(图标, 提示, 页面控件) 由 app 层(main.cpp)调用，
//   把 modules 里的页面装配进来。这就是"组装根(composition root)"模式：
//   ui 层不依赖 modules，依赖方向保持单向。
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