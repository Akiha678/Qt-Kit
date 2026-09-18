# ============================================================
# tests/CMakeLists.txt —— 单元测试目录
# 企业项目里测试与源码"平行"：tests/core 测 core，tests/modules 测模块。
# 用 Qt Test (QTest) 框架。示例（等 core 有逻辑代码后启用）：
#   add_subdirectory(core)
# 每个测试目录内：
#   add_executable(tst_logger test_logger.cpp)
#   add_test(NAME tst_logger COMMAND tst_logger)
#   target_link_libraries(tst_logger PRIVATE core Qt::Test)
# ============================================================
