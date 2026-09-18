cmake_minimum_required(VERSION 3.16)

project(QtKit
    VERSION 0.1.0   # 应用版本
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)   # Q_OBJECT 宏 → moc 生成元对象代码
set(CMAKE_AUTORCC ON)   # .qrc 资源 → rcc 编译进程序
set(CMAKE_AUTOUIC ON)   # .ui 设计文件 → uic 转成 C++ 代码

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(src)
