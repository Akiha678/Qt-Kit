// ============================================================
// ble_smoke.cpp —— 蓝牙模块运行时冒烟检查（诊断用）
// 流程：启动扫描 → 等 4 秒 → 停止 → 输出结果到文件。
// 关注点：
//   [1] 扫描能否正常启动（本机是否有 BLE 适配器/权限）
//   [2] 是否扫到设备（有几个）
//   [3] 是否报错（无适配器时 WinRT 会报错）
// ============================================================

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include <ble/BleManager.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QFile out(QStringLiteral("C:/Users/ainstec/Desktop/Project/Qt-Kit/ble_probe_result.txt"));
    out.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&out);

    BleManager manager;
    int discovered = 0;

    QObject::connect(&manager, &BleManager::deviceDiscovered, &app,
                     [&s, &discovered](const QString &name, const QString &address, qint16 rssi) {
                         ++discovered;
                         s << "  设备: " << name << "  " << address << "  (" << rssi << " dBm)\n";
                     });
    QObject::connect(&manager, &BleManager::scanFinished, &app, [&s]() {
        s << "[2] 扫描自然结束\n";
    });
    QObject::connect(&manager, &BleManager::scanError, &app,
                     [&s](const QString &message) {
                         s << "[3] 扫描错误: " << message << "\n";
                     });

    manager.startScan();
    s << "[1] 扫描已启动: " << manager.isScanning() << "\n";

    // 4 秒后停止扫描并退出事件循环
    QTimer::singleShot(4000, &app, [&app, &manager, &s, &discovered]() {
        manager.stopScan();
        s << "[4] 共发现设备数: " << discovered << "\n";
        s.flush();
        app.quit();
    });

    app.exec();
    out.close();
    return 0;
}