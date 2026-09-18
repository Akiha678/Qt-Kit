# ============================================================
# tests/theme/CMakeLists.txt —— 主题模块冒烟测试
# 控制台程序：直接打印诊断信息，不弹 GUI。
# 作用：验证主题资源在运行时是否真的存在、applyTheme 是否真的生效。
# ============================================================

add_executable(tst_theme
    theme_smoke.cpp
)

target_link_libraries(tst_theme PRIVATE ui)