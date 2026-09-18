# ============================================================
# src/modules/serial/CMakeLists.txt —— 串口连接模块
# 骨架阶段只有页面 Widget；后续加入 SerialManager 等逻辑
# ============================================================

add_library(serial STATIC
    SerialConnectWidget.cpp
    SerialConnectWidget.h
)

target_link_libraries(serial PUBLIC ui)
# 头文件暴露在 modules/CMakeLists.txt 统一处理（include 根 = modules/ 目录）