# ============================================================
# src/modules/CMakeLists.txt —— 业务模块库
# 每个业务功能 = 一个独立子目录 + 一个静态库（自包含）：
#   ble/     蓝牙连接
#   serial/  串口连接
# 模块内既放业务逻辑（后续 BleManager/SerialManager），
# 也放该功能的专属界面（BleConnectWidget / SerialConnectWidget）。
# 依赖：模块可以使用 ui 的【共享组件】（AppButton/AppIcons/AppDialog），
#       但 ui 绝不能反过来依赖任何模块 —— 方向见 docs/ARCHITECTURE.md。
# ============================================================

add_subdirectory(ble)
add_subdirectory(serial)

# 模块头文件统一暴露（include 根 = modules/ 目录）：
# 上层以 #include <ble/BleConnectWidget.h> 的形式引用，
# 与 ui 库的 <mainwindow/...>、<widgets/...> 风格保持一致。
target_include_directories(ble    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(serial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})