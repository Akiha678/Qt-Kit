# ============================================================
# src/modules/ble/CMakeLists.txt —— 蓝牙连接模块
# 骨架阶段只有页面 Widget；后续在此加入：
#   BleManager（扫描/连接/收发，纯逻辑，便于单测）
#   BleConnectWidget 里 connect 到 BleManager 的信号
# ============================================================

add_library(ble STATIC
    BleConnectWidget.cpp
    BleConnectWidget.h
)

target_link_libraries(ble PUBLIC ui) # 用 ui 的共享组件（AppButton/AppIcons）
# 头文件暴露在 modules/CMakeLists.txt 统一处理（include 根 = modules/ 目录）