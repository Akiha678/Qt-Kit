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
    int addPage(const QString &iconName, const QString &title, QWidget *page);
    int addPage(const QIcon &icon, const QString &title, QWidget *page);

private:
    void buildNavigation();

    NavBar *m_navBar = nullptr;
    QStackedWidget *m_pages = nullptr;
};