# 架构说明

## 分层与依赖方向（铁律）

```
        ┌──────────┐
        │  app     │  可执行程序：组合根(composition root)，只做装配
        └──┬────┬──┘
           │    │ 依赖
        ┌──▼─┐ ┌▼──────────┐
        │ ui │◀│ modules   │  业务模块：ble/serial/……（逻辑+专属页面）
        └──┬─┘ └───────────┘
           │ 依赖
        ┌──▼───┐
        │ core │  核心库：纯逻辑，只依赖 QtCore
        └──────┘
```

- **允许的依赖**：`app → ui / modules / core`；`modules → core`
- **modules → ui**：允许，但只许使用 ui 的**共享组件**
  （AppButton / AppIcons / AppDialog / NavBar），不许调用 ui 的内部实现细节
- **禁止**：`ui → modules`（ui 不认识任何业务模块——页面由 app 层装配进主窗口）；
  modules 之间互相依赖（跨模块协作用信号槽或 core 中的接口）
- 好处：每个层可独立编译、独立测试、可替换

### 组合根（composition root）是关键

主窗口只提供"导航框架 + addPage() 接口"，不知道也不关心页面是谁。
是 `main.cpp` 把 ble/serial 模块的页面塞进主窗口——**只有 app 层认识所有层**，
这是依赖方向不反向的原因，也是企业常规做法。

## 各层职责

| 层 | 放什么 | 不放什么 |
|---|---|---|
| app | main.cpp、装配、启动参数 | 业务逻辑 |
| ui | 主窗口、对话框、自定义控件、**主题(theme)**、qss 样式 | 串口/网络/文件等实际操作 |
| modules | 具体业务功能（每功能一个静态库） | 界面细节 |
| core | 版本、日志、配置、通用工具（纯逻辑，只依赖 QtCore） | 任何界面代码 |

## 主题模块为什么放在 ui 层（而不是 core / 单独一层）

判断一个模块放哪层，看两点：**它依赖什么**、**谁在使用它**。

| 候选位置 | 结论 | 理由 |
|---|---|---|
| core | ❌ | core 的铁律是"纯逻辑、只依赖 QtCore"；主题要调 `qApp->setStyleSheet` 和 QSS，离不开 QtWidgets |
| 独立顶层库 | ❌ | 当前体量单库就够了；等将来"多产品共享同一套主题"时才值得拆独立库 |
| **ui/theme** | ✅ | 主题是**表现层**的事；被主窗口/对话框/控件使用，正好都在 ui 内 |

**换主题的动作（setStyleSheet）永远属于 ui 层**。
若未来需要把"配色数据"（design tokens）共享给 modules 里的自定义绘图，
可把那部分纯色值下沉到 core —— 记住这个演进方向即可，现在不需要。

纪律：**modules 层不得直接调用 ThemeManager**；模块只提供"内容"，
外观由 ui 层统一负责。这保证换肤永远只影响 ui 层代码。

## 弹窗 / 控件放哪层

按"是否通用"分两种，判断方法同样是：**它被谁复用**。

| 组件类型 | 位置 | 例子 |
|---|---|---|
| **通用/共享弹窗**（项目内多处复用） | `ui/dialogs/` | `AppDialog`（通用弹窗组件） |
| **通用/共享控件**（自定义小控件） | `ui/widgets/` | `AppButton`（变体按钮组件） |
| **业务专属弹窗/控件**（属于某个功能） | 跟随业务模块 `modules/xxx/` | 串口设置对话框 → `modules/serial/dialogs/` |
| **一次性小弹框**（无处复用） | 就地用 `QMessageBox` 等，不封装 | 确认删除 |

规则：**只有会被复用的才封装成组件**。业务专属控件跟着业务模块走，
是为了让模块自包含（换掉/删除模块时，它的界面一起走）。

## 控件×主题：QSS 动态属性模式（AppButton 是标准示范）

按钮变体这种"样式随主题变、语义固定"的控件，正确做法是：

```
C++ 只管语义：setProperty("variant", "Primary")   ← 动态属性
QSS 管外观：   QPushButton[variant="Primary"] { ... } ← 属性选择器
light.qss / dark.qss 各配一套颜色                  ← 换主题零代码
```

- **优点**：业务代码永远只说"这是主按钮"，颜色/圆角/字号全在 qss；
  换主题、换品牌风格都不碰 C++。
- **注意（高频坑）**：运行时改动态属性后，必须
  `style()->unpolish(this); style()->polish(this); update();`
  否则样式系统不会重新计算，界面不变（AppButton::applyVariant 里已封装）。

### 三种"外观随主题变"的机制怎么选

| 场景 | 机制 | 代表 |
|---|---|---|
| 标准控件外观 | 全局 QSS（自动） | 背景、按钮、边框 |
| 同一控件多种形态 | QSS 动态属性选择器 `[prop="x"]` | AppButton 变体 |
| QSS 表达不了的运行时绘制 | Theme 接口（accentColor + themeChanged 信号） | 自绘控件 |

## 图标：资源位置与统一入口（企业标准）

| 项 | 约定 |
|---|---|
| 共享图标素材 | `resources/icons/`（首选 **SVG**，矢量可缩放；位图仅特殊场景） |
| 资源清单 | `resources/icons/icons.qrc`（**qrc 与素材同目录**），前缀 `/icons` |
| 代码入口 | **`AppIcons::get("name")`，禁止手写 `":/icons/xxx"` 字符串** |
| 命名 | 小写 kebab-case（settings、wifi-off） |
| 主题适配 | 深色变体约定 `name_dark.svg`，`AppIcons` 按当前主题自动选 |
| 缺失兜底 | 未登记的名字返回灰色占位图，界面不出现"空白图" |

> ⚠️ qrc 的 `<file>` 路径相对 qrc 自身 → **qrc 必须与素材同目录**（如
> `theme.qrc` 与 `styles/` 同目录、`icons.qrc` 在 `icons/` 内）。放错位置
> 会让资源 URL 多出一层目录（`:/icons/icons/xxx`），表现为"图标不显示"。
> 同时资源 URL 必须**带扩展名**（`AppIcons::path` 内部自动补 `.svg`）。

**新增一个图标的完整步骤**（两步，无 C++ 改动）：
1. 把 SVG 放进 `resources/icons/`（需要深色变体则加 `xxx_dark.svg`）
2. 在 `resources/icons.qrc` 里登记一行 `<file>xxx.svg</file>`

### 为什么 icons.qrc 放在 app 而不是 ui 静态库（资源丢失陷阱）

qrc 编译产物是带静态注册的 `.obj`。放进 `STATIC` 库（如 `ui.lib`）时，
注册代码**可能不会被执行** → 运行时 `:/...` 找不到（症状：图标空白、
切主题毫无反应）。两种可靠做法：

1. **qrc 编进 exe**（本项目 icons.qrc / images.qrc 的做法）：
   对象直接属于 exe，注册必然执行，100% 可用
2. **qrc 留在静态库 + `Q_INIT_RESOURCE(名字)`**（本项目 theme.qrc 的做法）：
   在库的入口处显式调用注册函数（见 `ThemeManager::instance()`），
   强制链接器拉入 rcc 对象并立即注册

若将来 ui 改成动态库（dll），两种做法都天然成立，无需改动。
**判断标准**：新增一个 qrc 时先问"这个资源编进哪个目标"——编进 exe 或
库内显式 Q_INIT_RESOURCE，二选一，别裸放。

### 业务模块要图标怎么办

遵循"模块自包含"：模块自定义控件用的图标放模块目录
（如 `modules/serial/resources/`、模块自己的 .qrc）；
跨模块共享图标才进 `resources/icons/`。
scaffold 阶段共享图标已够用，勿提前抽象。

## 图片资源（Logo/大图）：resources/images + AppImages

与"小图标"分开管理：**SVG 小图标走 AppIcons，大图/品牌图走 AppImages**。

| 项 | 约定 |
|---|---|
| 素材 | `resources/images/`（Logo、插图） |
| 清单 | `resources/images.qrc`，前缀 `/images`，编进 app |
| 入口 | `AppImages::logo()` / `AppImages::logoIcon()`，禁止手写资源字符串 |
| 格式 | **PNG** 优先：PNG/BMP/XPM 是 QtGui 内置解码器；JPG/WebP 需要运行时插件（qjpeg.dll），部署多一层依赖 |
| 生产环境 | Windows 正式发布建议提供多尺寸 `.ico`（16/32/48/256）经 RC 资源嵌入 exe |

**流程**：设计稿（如 `docs/images/` 原始素材）→ 转换/导出 PNG → 放入
`resources/images/` → `images.qrc` 登记 → 代码 `AppImages::xxx()` 引用。

## 约定

- 版本号只维护在顶层 `CMakeLists.txt` 的 `project()`，经 `configure_file` 生成 `Version.h`
- 新功能先想清楚属于哪一层，再落目录；不确定就先问
