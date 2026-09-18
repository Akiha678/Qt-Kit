# resources —— 资源目录

## 图标（共享图标统一放这里）

- 素材目录：`resources/icons/`，**首选 SVG**
- 清单：`resources/icons/icons.qrc`（**qrc 与素材同目录**，前缀 `/icons`，由 app 目标编译进程序）
- 代码入口：一律 `AppIcons::get("name")`，**禁止手写 `":/icons/xxx"` 字符串**
  （`AppIcons` 位于 `src/ui/icons/`，负责主题适配与缺失兜底）

### 新增图标流程（两步）

1. 放文件：`resources/icons/save.svg`（需要深色变体就再加 `save_dark.svg`）
2. 登记：`resources/icons/icons.qrc` 里加 `<file>save.svg</file>`

> ⚠️ qrc 的 `<file>` 路径相对 qrc 自身 → **qrc 必须和素材放在同一个目录**，
> 否则资源 URL 会多出一层目录（`:icons/icons/xxx.svg`），运行时找不到。

### 命名与主题规范

| 规则 | 说明 |
|---|---|
| 命名 | 小写 kebab-case：`save`、`wifi-off`、`settings` |
| 深色变体 | 名字加 `_dark` 后缀：`save.svg` ↔ `save_dark.svg` |
| 颜色 | 单色图标用中性灰（浅色主题 `#5a5a5a`，深色主题 `#d6d6d6`）；强调色图标用 Theme 强调色 |

### 常见图标参考

`app`（应用 logo）、`info`、`warning`、`settings`、`refresh`

## 图片（Logo / 大图）

- 素材目录：`resources/images/`，**优先 PNG**（QtGui 内置解码；JPG/WebP 需运行时插件）
- 清单：`resources/images/images.qrc`（与素材同目录，前缀 `/images`，由 app 目标编译进程序）
- 代码入口：`AppImages::logo()` / `AppImages::logoIcon()`（位于 `src/ui/icons/AppImages`）
- 新增流程：素材转 PNG → 放 `resources/images/` → `images.qrc` 登记 → 代码引用

> 原始设计稿可以放在 `docs/images/`（文档区），程序用的正式资源一律进 `resources/`。

## 其他资源

- `styles/` 跨模块共享的 qss（模块自己的样式跟随模块存放）
- `translations/` 翻译文件（.ts → .qm）
- `data/` 随程序分发的数据文件