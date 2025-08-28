@echo off
chcp 65001 >nul
echo ========================================
echo         Godot 构建工具
echo ========================================
echo.

REM 设置 Visual Studio 环境变量
echo [INFO] 正在设置 Visual Studio 环境变量...
call "D:\VS2022\VC\Auxiliary\Build\vcvarsall.bat" x64

REM 运行构建命令
echo [INFO] 开始构建 Godot...
echo [INFO] 构建命令: scons platform=windows target=editor dev_build=yes -j4
echo.
scons platform=windows target=editor dev_build=yes -j4

echo.
echo ========================================
if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] 构建成功！
    echo [SUCCESS] 可执行文件: bin\godot.windows.editor.dev.x86_64.exe
    echo.
    echo [TIPS] 现在可以运行 run_godot.bat 启动编辑器
) else (
    echo [ERROR] 构建失败，错误代码: %ERRORLEVEL%
    echo.
    echo [HELP] 请检查错误信息并修复问题
    echo [HELP] 常见问题请参考 GODOT_BUILD_SETUP.md
)
echo ========================================
echo.
pause
