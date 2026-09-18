# ============================================================
# tests/ble/CMakeLists.txt —— 蓝牙模块冒烟测试
# 控制台程序：验证本机 BLE 栈能否初始化、扫描是否正常启动。
# 结果写入 ble_probe_result.txt（沙箱吞控制台输出，改用文件）。
# ============================================================

add_executable(tst_ble
    ble_smoke.cpp
)

target_link_libraries(tst_ble PRIVATE ble Qt${QT_VERSION_MAJOR}::Bluetooth)