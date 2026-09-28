# =====================================
# WiFi管理模块
# =====================================

add_library(wifi STATIC
    WifiManageWidget.cpp
    WifiManageWidget.h
)

target_link_libraries(wifi PUBLIC ui)