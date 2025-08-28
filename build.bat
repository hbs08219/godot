@echo off
echo Building Godot for Windows...
echo.

REM 设置 Visual Studio 环境变量
call "D:\VS2022\VC\Auxiliary\Build\vcvarsall.bat" x64

REM 运行构建命令
echo Starting build...
scons platform=windows target=editor dev_build=yes -j4

echo.
if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully!
    echo Executable: bin\godot.windows.editor.dev.x86_64.exe
) else (
    echo Build failed with error code %ERRORLEVEL%
)

pause
