add_subdirectory(core)      # 核心库：被所有层依赖，最先构建
add_subdirectory(ui)        # 界面库：依赖 core
add_subdirectory(modules)   # 业务模块库：依赖 core
add_subdirectory(app)       # 可执行程序：最后构建，链接上面所有库
