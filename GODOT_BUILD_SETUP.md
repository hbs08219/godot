# Godot 编译环境配置指南

## 概述
本文档指导如何在 Windows 系统上配置 Godot 引擎的编译环境，供 CursorAgent 协助用户配置使用。

## 系统要求
- Windows 10/11 操作系统
- 至少 8GB RAM（推荐 16GB+）
- 至少 10GB 可用磁盘空间
- 管理员权限（用于安装软件）

## 必需软件安装

### 1. Visual Studio 2022 Community（免费版）
- 下载地址：https://visualstudio.microsoft.com/zh-hans/vs/community/
- 安装时必须选择：
  - "使用 C++ 的桌面开发"
  - "MSVC v143 - VS 2022 C++ x64/x86 编译器"
  - "Windows 10/11 SDK"
- 安装完成后记录安装路径（通常是 `C:\Program Files\Microsoft Visual Studio\2022\Community\`）

### 2. Python 3.8+
- 下载地址：https://www.python.org/downloads/
- 安装时勾选 "Add Python to PATH"
- 验证安装：打开命令提示符，输入 `python --version`

### 3. SCons 构建工具
- 安装命令：`python -m pip install scons`
- 验证安装：`python -c "import scons; print('SCons installed')"`

## 环境配置步骤

### 步骤 1：获取 Godot 源码
```bash
git clone https://github.com/godotengine/godot.git
cd godot
```

### 步骤 2：创建 VS Code 配置文件
在项目根目录创建 `.vscode` 文件夹，并添加以下配置文件：

#### tasks.json
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "group": "build",
            "type": "shell",
            "command": "scons",
            "args": [
                "platform=windows",
                "target=editor",
                "dev_build=yes",
                "compiledb=yes"
            ],
            "problemMatcher": "$msCompile",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

#### c_cpp_properties.json
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/platform/windows"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "TOOLS_ENABLED",
                "DEBUG_ENABLED",
                "TESTS_ENABLED"
            ],
            "windowsSdkVersion": "10.0.22621.0",
            "compilerPath": "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-msvc-x64"
        }
    ],
    "version": 4
}
```

**注意**：需要根据实际安装的 Visual Studio 路径和 MSVC 版本调整 `compilerPath` 和 `windowsSdkVersion`。

### 步骤 3：创建构建脚本
创建 `build.bat` 文件：
```batch
@echo off
echo 正在构建 Godot for Windows...
echo.

REM 设置 Visual Studio 环境变量
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM 运行构建命令
echo 开始构建...
scons platform=windows target=editor dev_build=yes -j4

echo.
if %ERRORLEVEL% EQU 0 (
    echo 构建成功！
    echo 可执行文件: bin\godot.windows.editor.dev.x86_64.exe
) else (
    echo 构建失败，错误代码: %ERRORLEVEL%
)

pause
```

**注意**：需要根据实际安装路径调整 `vcvarsall.bat` 的路径。

## 构建命令

### 基本构建
```bash
scons platform=windows target=editor dev_build=yes
```

### 并行构建（推荐）
```bash
scons platform=windows target=editor dev_build=yes -j4
```

### 清理构建文件
```bash
scons --clean
```

## 常见问题解决

### 1. "cl" 命令未找到
**原因**：Visual Studio 环境变量未设置
**解决**：使用 `build.bat` 脚本，或手动运行：
```bash
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

### 2. SCons 命令未找到
**原因**：SCons 未安装或不在 PATH 中
**解决**：重新安装 SCons：`python -m pip install scons`

### 3. 编译器版本不匹配
**原因**：配置文件中的编译器路径不正确
**解决**：检查实际安装路径，更新 `c_cpp_properties.json` 中的 `compilerPath`

### 4. Windows SDK 版本错误
**原因**：配置文件中的 SDK 版本不正确
**解决**：检查 `C:\Program Files (x86)\Windows Kits\10\Include\` 目录下的实际版本号

## 验证配置

### 1. 检查编译器
```bash
cl /?
```

### 2. 检查 SCons
```bash
scons --version
```

### 3. 测试构建
```bash
scons platform=windows target=editor dev_build=yes
```

## 成功标志
- 构建过程无错误
- 生成 `bin\godot.windows.editor.dev.x86_64.exe` 文件
- 可以正常运行 Godot 编辑器

## 注意事项
1. 首次构建时间较长（通常 10-30 分钟）
2. 确保网络连接稳定（需要下载依赖）
3. 构建过程中不要关闭命令提示符
4. 如遇到权限问题，请以管理员身份运行

## 技术支持
- Godot 官方文档：https://docs.godotengine.org/
- 构建问题：https://github.com/godotengine/godot/issues
- 社区论坛：https://godotforums.org/
