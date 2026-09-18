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
    void buildNavigation();

    NavBar *m_navBar = nullptr;
    QStackedWidget *m_pages = nullptr;
};