// ============================================================
// ThemeManager.cpp —— 主题管理器实现
// ============================================================

#include "ThemeManager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QResource>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
}

ThemeManager &ThemeManager::instance()
{
    Q_INIT_RESOURCE(theme);

    // 函数内静态局部变量：C++11 起保证只初始化一次、且线程安全
    static ThemeManager mgr;
    return mgr;
}

Theme::Type ThemeManager::currentTheme() const
{
    return m_current;
}

void ThemeManager::applyTheme(Theme::Type type)
{
    // qss 存在 .qrc 里，随程序编译进 exe（AUTORCC 负责），
    // 资源路径固定为 qrc 前缀 + 文件路径：:/theme/styles/xxx.qss
    const QString qssPath = (type == Theme::Dark)
        ? QStringLiteral(":/theme/styles/dark.qss")
        : QStringLiteral(":/theme/styles/light.qss");

    QFile f(qssPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "ThemeManager: 无法加载样式表" << qssPath;
        return;
    }

    // 关键一步：setStyleSheet 作用于整个应用
    // （QWidget 级别调用只影响单个控件；这里要全局换肤）
    qApp->setStyleSheet(QString::fromUtf8(f.readAll()));

    m_current = type;
    emit themeChanged(type); // 通知所有关心主题的地方
}