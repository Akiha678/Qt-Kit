// ============================================================
// theme_smoke.cpp —— 主题运行时冒烟检查（诊断用）
// 结果写入 probe_result.txt（控制台输出在沙箱里会被吞，改用文件）
// 正常结果应全部为 true：
//   [1][2] 资源在运行时是否注册成功
//   [3][4] applyTheme 后全局样式表是否真的被设置
// ============================================================

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile out(QStringLiteral("C:/Users/ainstec/Desktop/Project/Qt-Kit/probe_result.txt"));
    out.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&out);

    s << "[1] dark.qss exists : " << QFile::exists(QStringLiteral(":/theme/styles/dark.qss")) << "\n";
    s << "[2] light.qss exists: " << QFile::exists(QStringLiteral(":/theme/styles/light.qss")) << "\n";

    ThemeManager::instance().applyTheme(Theme::Dark);

    s << "[3] styleSheet size : " << app.styleSheet().size() << "\n";
    s << "[4] has dark bg    : " << app.styleSheet().contains(QStringLiteral("#2b2b2b")) << "\n";
    s.flush();
    out.close();

    return 0;
}