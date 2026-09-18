# ============================================================
# src/core/CMakeLists.txt —— 核心库
# core 是"地基"：与业务无关的通用能力（版本、日志、配置、工具函数）。
# 铁律：core 不依赖 ui 和任何业务模块，但所有人都可以依赖 core。
#
# 目前 core 是"头文件库"（INTERFACE：只传播头文件和依赖，不产 .lib）。
# 以后加入 logger / config 等带 .cpp 的实现时，改成 add_library(core STATIC ...)
# ============================================================

add_library(core INTERFACE)

# 暴露头文件搜索路径：
#   ${CMAKE_CURRENT_SOURCE_DIR} —— 未来放 logger/ 等源码目录
#   ${CMAKE_CURRENT_BINARY_DIR} —— 构建时生成的 Version.h 所在目录
target_include_directories(core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# core 只依赖 Qt Core（不碰 Widgets，保持纯逻辑）
target_link_libraries(core INTERFACE Qt${QT_VERSION_MAJOR}::Core)

# ---------- 生成 Version.h ----------
# configure_file 的作用（企业项目标配）：
#   构建时读取 Version.h.in 模板，把 @XXX@ 占位符替换成 CMake 变量值，
#   生成真正的 Version.h。
# 效果：版本号/产品名只在顶层 project() 维护"一处"，
#       任何代码 include 这个头文件都能拿到，改版本号不用动 C++ 代码。
set(QTKIT_ORG_NAME "QtKit")
configure_file(Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/Version.h @ONLY)
