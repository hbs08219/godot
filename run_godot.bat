@echo off
chcp 65001 >nul
echo ========================================
echo         Godot 编辑器启动器
echo ========================================
echo.

REM 检查 Godot 可执行文件是否存在
if exist "bin\godot.windows.editor.dev.x86_64.exe" (
    echo [INFO] 找到 Godot 编辑器，正在启动...
    echo [INFO] 可执行文件: bin\godot.windows.editor.dev.x86_64.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.exe"
    echo [SUCCESS] Godot 编辑器已启动！
) else if exist "bin\godot.windows.editor.dev.x86_64.console.exe" (
    echo [INFO] 找到 Godot 编辑器（控制台版本），正在启动...
    echo [INFO] 可执行文件: bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.console.exe"
    echo [SUCCESS] Godot 编辑器已启动！
) else (
    echo [ERROR] 未找到 Godot 可执行文件！
    echo.
    echo [HELP] 请先运行 build.bat 构建 Godot
    echo [HELP] 或者检查以下路径是否存在：
    echo        - bin\godot.windows.editor.dev.x86_64.exe
    echo        - bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo [TIPS] 使用提示：
echo [TIPS] - 如果编辑器没有启动，请检查杀毒软件设置
echo [TIPS] - 首次启动可能需要较长时间
echo [TIPS] - 可以在命令行中运行以查看详细输出
echo ========================================
echo.
pause
