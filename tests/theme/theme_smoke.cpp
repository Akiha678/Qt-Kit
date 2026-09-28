// ============================================================
// theme_smoke.cpp —— 主题运行时冒烟检查（诊断用）
// 结果写入 probe_result.txt（控制台输出在沙箱里会被吞，改用文件）
// 正常结果应全部为 true：
//   [1][2] 资源在运行时是否注册成功
//   [3][4] applyTheme 后全局样式表是否真的被设置
// ============================================================

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <theme/Theme.h>
#include <theme/ThemeManager.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 使用当前执行目录输出诊断结果，避免硬编码开发者本机绝对路径
    const QString outPath = QDir::current().filePath(QStringLiteral("probe_result.txt"));
    QFile out(outPath);
    out.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&out);

    const bool darkExists = QFile::exists(QStringLiteral(":/theme/styles/dark.qss"));
    const bool lightExists = QFile::exists(QStringLiteral(":/theme/styles/light.qss"));
    s << "[1] dark.qss exists : " << darkExists << "\n";
    s << "[2] light.qss exists: " << lightExists << "\n";

    ThemeManager::instance().applyTheme(Theme::Dark);

    const bool hasSize = app.styleSheet().size() > 0;
    const bool hasDarkBg = app.styleSheet().contains(QStringLiteral("#2b2b2b"));
    s << "[3] styleSheet size : " << app.styleSheet().size() << "\n";
    s << "[4] has dark bg    : " << hasDarkBg << "\n";
    s.flush();
    out.close();

    // 检查核心断言，若失败返回非0退出码
    if (!darkExists || !lightExists || !hasSize || !hasDarkBg) {
        return 1;
    }

    return 0;
}