// ============================================================
// main.cpp —— 程序入口（app 层，唯一的"组装根"）
// 职责：
//   1. 创建 QApplication、设置元信息
//   2. 把各层"装配"起来 —— 主窗口 + 各业务模块页面
//      （这就是 composition root：只有它认识所有层，其他层互不感知）
// ============================================================

#include <QApplication>
#include <Version.h>                 // core：应用名称/版本（由 CMake 生成）
#include <mainwindow/MainWindow.h>   // ui：主窗口（导航框架）
#include <icons/AppIcons.h>          // ui：图标统一入口
#include <ble/BleConnectWidget.h>    // modules：蓝牙连接页面
#include <serial/SerialConnectWidget.h> // modules：串口连接页面

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 元信息统一来自 Version.h（唯一事实来源）
    QApplication::setOrganizationName(QTKIT_ORG_NAME);
    QApplication::setApplicationName(QTKIT_APP_NAME);
    QApplication::setApplicationVersion(QTKIT_VERSION);

    MainWindow window;

    // 装配导航页：图标(自动适配主题) + 悬停提示 + 页面控件
    window.addPage(AppIcons::get(QStringLiteral("bluetooth")),
                   QStringLiteral("蓝牙连接"),
                   new BleConnectWidget(&window));
    window.addPage(AppIcons::get(QStringLiteral("serial")),
                   QStringLiteral("串口连接"),
                   new SerialConnectWidget(&window));

    window.show();

    return app.exec();
}