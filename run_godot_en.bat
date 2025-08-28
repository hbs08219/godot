@echo off
echo ========================================
echo         Godot Editor Launcher
echo ========================================
echo.

REM Check if Godot executable exists
if exist "bin\godot.windows.editor.dev.x86_64.exe" (
    echo [INFO] Found Godot Editor, starting...
    echo [INFO] Executable: bin\godot.windows.editor.dev.x86_64.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.exe"
    echo [SUCCESS] Godot Editor started!
) else if exist "bin\godot.windows.editor.dev.x86_64.console.exe" (
    echo [INFO] Found Godot Editor (Console), starting...
    echo [INFO] Executable: bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    start "" "bin\godot.windows.editor.dev.x86_64.console.exe"
    echo [SUCCESS] Godot Editor started!
) else (
    echo [ERROR] Godot executable not found!
    echo.
    echo [HELP] Please run build.bat first to build Godot
    echo [HELP] Or check if these paths exist:
    echo        - bin\godot.windows.editor.dev.x86_64.exe
    echo        - bin\godot.windows.editor.dev.x86_64.console.exe
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo [TIPS] Usage Tips:
echo [TIPS] - If editor doesn't start, check antivirus settings
echo [TIPS] - First launch may take longer time
echo [TIPS] - Run in command line to see detailed output
echo ========================================
echo.
pause
