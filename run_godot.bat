@echo off
echo 正在启动 Godot 编辑器...
echo.

REM 检查 Godot 可执行文件是否存在
if exist "bin\godot.windows.editor.dev.x86_64.exe" (
    echo 找到 Godot 编辑器，正在启动...
    echo.
    echo 可执行文件: bin\godot.windows.editor.dev.x86_64.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.exe"
    echo Godot 编辑器已启动！
) else if exist "bin\godot.windows.editor.dev.x86_64.console.exe" (
    echo 找到 Godot 编辑器（控制台版本），正在启动...
    echo.
    echo 可执行文件: bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.console.exe"
    echo Godot 编辑器已启动！
) else (
    echo 错误：未找到 Godot 可执行文件！
    echo.
    echo 请先运行 build.bat 构建 Godot
    echo 或者检查以下路径是否存在：
    echo   - bin\godot.windows.editor.dev.x86_64.exe
    echo   - bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    pause
    exit /b 1
)

echo.
echo 提示：
echo - 如果编辑器没有启动，请检查是否有杀毒软件阻止
echo - 首次启动可能需要较长时间
echo - 可以在命令行中运行以查看详细输出
echo.
pause
