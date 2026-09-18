# ============================================================
# src/app/CMakeLists.txt —— 可执行程序
# ============================================================

add_executable(QtKit WIN32
    main.cpp
    ${CMAKE_SOURCE_DIR}/resources/icons/icons.qrc
    ${CMAKE_SOURCE_DIR}/resources/images/images.qrc
)

target_link_libraries(QtKit PRIVATE
    ui       # ui模块
    core     # 核心模块
    ble      # 蓝牙连接模块
    serial   # 串口连接模块
)
