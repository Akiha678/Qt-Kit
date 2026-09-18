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

    static ThemeManager mgr;
    return mgr;
}

Theme::Type ThemeManager::currentTheme() const
{
    return m_current;
}

void ThemeManager::applyTheme(Theme::Type type)
{
    const QString qssPath = (type == Theme::Dark)
        ? QStringLiteral(":/theme/styles/dark.qss")
        : QStringLiteral(":/theme/styles/light.qss");

    QFile f(qssPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "ThemeManager: 无法加载样式表" << qssPath;
        return;
    }

    qApp->setStyleSheet(QString::fromUtf8(f.readAll()));

    m_current = type;
    emit themeChanged(type);
}