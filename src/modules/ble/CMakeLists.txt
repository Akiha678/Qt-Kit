# ============================================================
# src/modules/ble/CMakeLists.txt —— 蓝牙连接模块
# ============================================================

add_library(ble STATIC
    BleConnectWidget.cpp
    BleConnectWidget.h
)

target_link_libraries(ble PUBLIC ui) # 用 ui 的共享组件（AppButton/AppIcons）
# 头文件暴露在 modules/CMakeLists.txt 统一处理（include 根 = modules/ 目录）