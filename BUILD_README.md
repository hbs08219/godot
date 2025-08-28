# Godot 构建环境说明

## 概述
这个项目已经配置好了 Godot 引擎的 Windows 构建环境，包括：
- Visual Studio 2022 编译器配置
- VS Code 开发环境配置
- 构建脚本和任务配置

## 系统要求
- Windows 10/11
- Visual Studio 2022 (安装在 D:\VS2022)
- Python 3.8+ (已安装 Python 3.11.9)
- SCons 4.0+ (已安装 4.9.1)

## 快速开始

### 方法 1: 使用批处理文件 (推荐)
```cmd
build.bat
```

### 方法 2: 使用 PowerShell 脚本
```powershell
.\build.ps1
```

### 方法 3: 手动构建
```cmd
REM 设置 Visual Studio 环境
call "D:\VS2022\VC\Auxiliary\Build\vcvarsall.bat" x64

REM 构建 Godot
scons platform=windows target=editor dev_build=yes -j4
```

## VS Code 配置

### 构建任务
- `Ctrl+Shift+P` → "Tasks: Run Task" → "build"
- 或者使用快捷键 `Ctrl+Shift+B`

### 调试配置
- `F5` 启动调试
- 选择 "Launch Editor" 启动 Godot 编辑器
- 选择 "Launch Project" 启动特定项目

### 可用的构建任务
- `build` - 构建开发版本 (默认)
- `build-release` - 构建发布版本
- `build-templates` - 构建导出模板
- `clean` - 清理构建文件

## 构建输出
构建成功后，可执行文件位于：
- `bin\godot.windows.editor.dev.x86_64.exe` - 主要编辑器
- `bin\godot.windows.editor.dev.x86_64.console.exe` - 带控制台版本

## 常见问题

### 1. "cl" 命令未找到
确保使用 `build.bat` 或 `build.ps1` 脚本，它们会自动设置 Visual Studio 环境。

### 2. SCons 命令未找到
SCons 已安装在 `C:\Users\53410\AppData\Roaming\Python\Python311\Scripts\` 目录中。

### 3. 构建失败
- 检查 Visual Studio 2022 是否正确安装
- 确保 Windows SDK 已安装
- 尝试清理构建文件：`scons --clean`

## 开发提示

### 增量构建
Godot 使用 SCons 构建系统，支持增量构建。修改源码后直接运行构建命令即可。

### 并行构建
使用 `-j4` 参数可以并行构建，加快编译速度：
```cmd
scons platform=windows target=editor dev_build=yes -j4
```

### 调试构建
开发版本包含调试信息，可以使用 VS Code 进行断点调试。

## 更多信息
- [Godot 官方文档](https://docs.godotengine.org/)
- [Godot 贡献指南](https://docs.godotengine.org/en/latest/contributing/)
- [SCons 构建系统文档](https://scons.org/)

## 许可证
Godot 引擎使用 MIT 许可证，详见 [LICENSE.txt](LICENSE.txt) 文件。
