// ============================================================
// MainWindow.cpp —— 主窗口实现（界面层）
// ============================================================

#include "MainWindow.h"

#include <QActionGroup>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <Version.h>
#include <dialogs/AppDialog.h>
#include <icons/AppIcons.h>
#include <icons/AppImages.h>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>
#include <widgets/AppButton.h>
#include <widgets/NavBar.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString("%1 %2").arg(QTKIT_APP_NAME, QTKIT_VERSION));
    setWindowIcon(AppImages::logoIcon());

    buildNavigation();

    resize(800, 600);
}

// ---------- 导航框架 ----------
// 左：NavBar
// 右：QStackedWidget
void MainWindow::buildNavigation()
{
    m_navBar = new NavBar(this);
    m_pages = new QStackedWidget(this);

    auto *central = new QWidget(this);
    auto *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_navBar);
    layout->addWidget(m_pages, 1);
    setCentralWidget(central);

    // 点击图标导航跳转
    connect(m_navBar, &NavBar::currentChanged,
            m_pages, &QStackedWidget::setCurrentIndex);
}

int MainWindow::addPage(const QIcon &icon, const QString &title, QWidget *page)
{
    const int index = m_pages->addWidget(page);
    m_navBar->addItem(icon, title);
    return index;
}

