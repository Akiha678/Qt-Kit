# QtKit —— Qt 桌面工具脚手架（分层架构）

基于 **Qt 5.14 (Widgets) + CMake + C++17** 的桌面工具项目骨架。
采用企业级**分层 + 模块化**结构，用于搭建"串口调试 / 图像处理 / 设备初始化"一类工具。

## 目录结构

```
Qt-Kit/
├── CMakeLists.txt        # 顶层：全局选项 + 汇总子目录（版本号唯一来源）
├── cmake/                # 公共 CMake 脚本（预留）
├── docs/
│   └── ARCHITECTURE.md   # 架构说明：分层职责与依赖铁律
├── resources/            # 资源
│   ├── icons/            #   共享 SVG 图标 + icons.qrc（前缀 /icons，编进 app）
│   └── images/           #   大图/品牌图（iris_logo.png）+ images.qrc（前缀 /images）
├── tools/                # 打包等辅助脚本（预留）
├── src/
│   ├── app/              # 【可执行程序】main.cpp：装配各层
│   ├── core/             # 【核心库】Version.h（configure_file 生成）
│   │                     #          后续：logger 日志 / config 配置 / utils
│   ├── ui/               # 【界面库】
│   │   ├── mainwindow/   #   主窗口（NavBar + QStackedWidget 导航框架）
│   │   ├── dialogs/      #   通用弹窗组件（AppDialog）
│   │   ├── widgets/      #   通用控件（AppButton / NavBar）
│   │   ├── icons/        #   图标/图片统一入口（AppIcons 图标 + AppImages 大图）
│   │   └── theme/        #   主题模块：ThemeManager + light/dark.qss
│   ├── modules/          # 【业务模块库】
│   │   ├── ble/          #   蓝牙连接模块：BleManager(逻辑) + BleConnectWidget(界面)
│   │   └── serial/       #   串口连接模块（SerialConnectWidget）
│   └── tests/            # 【单元测试】预留（Qt Test）
└── .gitignore
```

**依赖方向（铁律，详见 `docs/ARCHITECTURE.md`）**：`app → ui/modules/core`；`modules → core`（并可使用 ui 的共享组件）；禁止 `ui → modules`、禁止模块间互依赖。只有 app 层（组合根）认识所有层。

## 构建

### Qt Creator（推荐）
打开项目 → 选 `CMakeLists.txt` → 选 Kit（Qt 5.14.2 MSVC2017 64bit）→ 运行 ▶

### 命令行
```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=D:/Qt/5.14.2/msvc2017_64
cmake --build build --config Debug --parallel
build\src\app\Debug\QtKit.exe
```

## 版本号维护

只改顶层 `CMakeLists.txt` 的 `project(... VERSION ...)`，
构建时 `configure_file` 自动生成 `Version.h`（`QTKIT_APP_NAME` / `QTKIT_VERSION`）。

## 下一步规划（逐个添加）
- [x] modules/ble 蓝牙连接（扫描/连接/服务发现）
- [ ] BLE 特征读写与通知订阅
- [ ] core/logger 日志系统（文件 + 界面面板）
- [ ] core/config 配置持久化（QSettings）
- [ ] modules/serial 串口连接（QSerialPort）
- [ ] tests 正式 QTest 单元测试
