# ============================================================
# src/ui/CMakeLists.txt —— 界面库
# ui 存放所有"与界面有关"的代码：主窗口、对话框、自定义控件。
# 主题模块（theme/）也在这里 —— 理由见 docs/ARCHITECTURE.md。
# 依赖方向：ui → core（界面里要用 core 提供的版本、日志等能力）
# 铁律：ui 里禁止写业务逻辑；业务逻辑属于 modules 层。
# ============================================================

add_library(ui STATIC
    mainwindow/MainWindow.cpp
    mainwindow/MainWindow.h
    theme/Theme.h
    theme/ThemeManager.cpp
    theme/ThemeManager.h
    theme/theme.qrc        # qss 资源：AUTORCC 自动编译进库
    dialogs/AppDialog.cpp
    dialogs/AppDialog.h
    widgets/AppButton.cpp
    widgets/AppButton.h
    widgets/NavBar.cpp
    widgets/NavBar.h
    icons/AppIcons.cpp
    icons/AppIcons.h
    icons/AppImages.cpp
    icons/AppImages.h
)

# 把自己源码目录暴露给上层（PUBLIC）：
# app 里才能 #include <mainwindow/MainWindow.h>、<theme/ThemeManager.h>
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# PUBLIC：core 的能力（如 Version.h）也随 ui 传递给 app，
# 且 Qt Widgets 的头文件/库对 app 可见
target_link_libraries(ui PUBLIC
    core
    Qt${QT_VERSION_MAJOR}::Widgets
)