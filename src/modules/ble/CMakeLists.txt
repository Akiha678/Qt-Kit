# ============================================================
# src/modules/ble/CMakeLists.txt —— 蓝牙连接模块
# 结构（逻辑与界面分离）：
#   BleManager         —— 纯逻辑：扫描/连接/服务发现（可单测）
#   BleConnectWidget   —— 界面：设备列表 + 状态 + 按钮
# ============================================================

add_library(ble STATIC
    BleManager.cpp
    BleManager.h
    BleConnectWidget.cpp
    BleConnectWidget.h
)

target_link_libraries(ble PUBLIC ui Qt${QT_VERSION_MAJOR}::Bluetooth)
# 头文件暴露在 modules/CMakeLists.txt 统一处理（include 根 = modules/ 目录）