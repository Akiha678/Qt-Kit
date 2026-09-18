# cmake —— 公共 CMake 脚本目录

放可复用的 CMake 模块（企业项目标配），例如：

- `QtUtils.cmake` 公共函数/宏
- 平台相关配置（Windows 打包、安装器）
- 第三方依赖查找（find_package 包装）

用法：顶层 CMakeLists 里 `list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)`
