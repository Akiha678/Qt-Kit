# ============================================================
# src/app/CMakeLists.txt —— 可执行程序
# app 是整个项目里唯一的"可执行文件"。
# 职责：main.cpp 负责"装配"——创建 QApplication、把各层串起来。
# app 不写业务代码，业务在 modules，界面在 ui。
# ============================================================

add_executable(QtKit WIN32
    main.cpp
    # 图标资源清单为什么放 exe 而不放 ui 静态库（重要）：
    # qrc 编译产物是带静态注册的 .obj；放进 STATIC 库里，链接器可能
    # 因"无人引用"把它丢弃 → 运行时 :/icons/xxx 找不到。
    # 放进 exe 则必然被链接，资源 100% 可用。
    # 若以后 ui 改作动态库(dll)，可以移回 ui 并使用 Q_INIT_RESOURCE。
    ${CMAKE_SOURCE_DIR}/resources/icons/icons.qrc
    ${CMAKE_SOURCE_DIR}/resources/images/images.qrc
)

target_link_libraries(QtKit PRIVATE
    ui       # 界面库（传递带来 Qt Widgets 的链接）
    core     # 直接用到 core 的 Version.h，显式声明依赖
    ble      # 蓝牙连接模块
    serial   # 串口连接模块
)
