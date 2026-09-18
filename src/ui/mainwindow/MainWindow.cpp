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
    setWindowIcon(AppImages::logoIcon()); // 窗口/任务栏图标（resources/images 的 Logo）

    buildNavigation();
    createMenus();
    createViewMenu();
    createToolsMenu();
    createStatusBar();

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

void MainWindow::createMenus()
{
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    QAction *quitAction = fileMenu->addAction(tr("退出(&Q)"));
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    // 帮助菜单
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(tr("关于(&A)"), this, [this]() {
        // 用 AppDialog 做"关于"框：能展示真实 Logo（AppImages）
        auto *aboutDlg = new AppDialog(this);
        aboutDlg->setAttribute(Qt::WA_DeleteOnClose);
        aboutDlg->setTitleText(tr("关于 %1").arg(QTKIT_APP_NAME));

        auto *body = new QWidget(aboutDlg);
        auto *bodyLayout = new QVBoxLayout(body);

        auto *logoLabel = new QLabel(body);
        logoLabel->setPixmap(AppImages::logo().scaled(96, 96,
                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
        bodyLayout->addWidget(logoLabel);

        auto *infoLabel = new QLabel(tr("%1 —— Qt 桌面工具脚手架\n版本 %2\n\n"
                                        "Logo 来自 resources/images，经 AppImages 统一入口加载。")
                                         .arg(QTKIT_APP_NAME, QTKIT_VERSION), body);
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setWordWrap(true);
        bodyLayout->addWidget(infoLabel);

        aboutDlg->setContentWidget(body);
        aboutDlg->addButton(tr("确定"), QDialogButtonBox::AcceptRole);
        aboutDlg->resize(380, 340);
        aboutDlg->show(); // 非模态
    });
}

// ---------- "视图"菜单：主题切换演示 ----------
// 演示主题模块的正确用法：
//   界面层负责"提供切换入口"，实际换肤动作全交给 ThemeManager
void MainWindow::createViewMenu()
{
    QMenu *viewMenu = menuBar()->addMenu(tr("视图(&V)"));

    // 两个互斥的勾选项：浅色 / 深色
    QAction *lightAction = viewMenu->addAction(Theme::typeName(Theme::Light));
    QAction *darkAction = viewMenu->addAction(Theme::typeName(Theme::Dark));
    lightAction->setCheckable(true);
    darkAction->setCheckable(true);
    lightAction->setChecked(true); // 默认浅色

    auto *group = new QActionGroup(this); // QActionGroup：保证同时只勾选一个
    group->addAction(lightAction);
    group->addAction(darkAction);
    group->setExclusive(true);

    // 用 lambda 捕获主题类型，点击对应菜单项就切换
    connect(lightAction, &QAction::triggered, this,
            []() { ThemeManager::instance().applyTheme(Theme::Light); });
    connect(darkAction, &QAction::triggered, this,
            []() { ThemeManager::instance().applyTheme(Theme::Dark); });
}

// ---------- "工具"菜单：弹窗组件演示 ----------
// 演示点：
//   1. AppDialog 的"三步用法"：setTitleText / setContentWidget / addButton
//   2. 非模态打开（show 而非 exec）：弹窗开着时主窗口仍可操作，
//      可以在 视图→深色 或弹窗内按钮切换主题，观察实时刷新
//   3. 菜单项图标走 AppIcons 统一入口（info，深色下自动用 info_dark）
void MainWindow::createToolsMenu()
{
    QMenu *toolsMenu = menuBar()->addMenu(tr("工具(&T)"));
    QAction *demoDialogAction =
        toolsMenu->addAction(AppIcons::get(QStringLiteral("info")), tr("示例弹窗(&O)"));
    connect(demoDialogAction, &QAction::triggered, this, [this]() {
        auto *dlg = new AppDialog(this);
        dlg->setAttribute(Qt::WA_DeleteOnClose); // 关闭即销毁（非模态必需）

        dlg->setTitleText(tr("示例弹窗 —— 通用弹窗组件"));

        // 内容区：说明 + AppButton 演示（主题切换 / 危险操作 / 运行时切变体）
        auto *body = new QWidget(dlg);
        auto *bodyLayout = new QVBoxLayout(body);
        auto *hint = new QLabel(tr(
            "这是 ui/dialogs 里的通用弹窗组件。\n"
            "底部状态栏实时显示当前主题：\n"
            "  · 图标颜色来自 Theme::accentColor（运行时绘制）\n"
            "  · 背景/按钮颜色来自全局 QSS（自动换肤）\n"
            "下面三个按钮是 ui/widgets 的 AppButton 组件（主/危险/次变体），\n"
            "配色由各自主题的 QSS 决定 —— 切主题时它们自动变色。"), body);
        hint->setWordWrap(true);

        // 主按钮：切换主题（演示 AppButton 作为"主操作"按钮）
        auto *toggleButton = new AppButton(tr("切换主题（浅色 ↔ 深色）"),
                                           AppButton::Variant::Primary, body);
        connect(toggleButton, &QPushButton::clicked, this, []() {
            const Theme::Type next =
                (ThemeManager::instance().currentTheme() == Theme::Dark)
                ? Theme::Light : Theme::Dark;
            ThemeManager::instance().applyTheme(next);
        });

        // 危险按钮：演示危险操作变体
        auto *dangerButton = new AppButton(tr("危险操作"),
                                           AppButton::Variant::Danger, body);
        connect(dangerButton, &QPushButton::clicked, this, [this]() {
            QMessageBox::warning(this, tr("危险操作"),
                                 tr("这是 AppButton 的 Danger 变体。\n"
                                    "危险操作的红色在切到深色主题后会自动变暗。"));
        });

        // 次按钮：运行时切换自身变体（演示 setVariant + repolish 重算样式）
        auto *cycleButton = new AppButton(tr("点击切换变体"),
                                          AppButton::Variant::Secondary, body);
        connect(cycleButton, &QPushButton::clicked, cycleButton, [cycleButton]() {
            using V = AppButton::Variant;
            const V next = (cycleButton->variant() == V::Primary) ? V::Danger
                         : (cycleButton->variant() == V::Danger)  ? V::Secondary
                                                                  : V::Primary;
            cycleButton->setVariant(next);
        });

        bodyLayout->addWidget(hint);
        bodyLayout->addWidget(toggleButton);
        bodyLayout->addWidget(dangerButton);
        bodyLayout->addWidget(cycleButton);

        dlg->setContentWidget(body);
        dlg->addButton(tr("确定"), QDialogButtonBox::AcceptRole);
        dlg->addButton(tr("取消"), QDialogButtonBox::RejectRole);

        dlg->resize(460, 380);
        dlg->show(); // 非模态
    });
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));
}
