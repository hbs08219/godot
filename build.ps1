# Godot 构建脚本
Write-Host "Building Godot for Windows..." -ForegroundColor Green
Write-Host ""

# 设置 Visual Studio 环境变量
$vcvarsall = "D:\VS2022\VC\Auxiliary\Build\vcvarsall.bat"
if (Test-Path $vcvarsall) {
    Write-Host "Setting up Visual Studio environment..." -ForegroundColor Yellow
    & cmd /c "`"$vcvarsall`" x64 && set" | ForEach-Object {
        if ($_ -match "=") {
            $name, $value = $_.split("=", 2)
            Set-Item -Path "env:$name" -Value $value
        }
    }
} else {
    Write-Host "Warning: Visual Studio environment not found at $vcvarsall" -ForegroundColor Yellow
}

# 运行构建命令
Write-Host "Starting build..." -ForegroundColor Yellow
$sconsPath = "C:\Users\53410\AppData\Roaming\Python\Python311\Scripts\scons.exe"

if (Test-Path $sconsPath) {
    & $sconsPath platform=windows target=editor dev_build=yes -j4
    $exitCode = $LASTEXITCODE
    
    Write-Host ""
    if ($exitCode -eq 0) {
        Write-Host "Build completed successfully!" -ForegroundColor Green
        Write-Host "Executable: bin\godot.windows.editor.dev.x86_64.exe" -ForegroundColor Cyan
    } else {
        Write-Host "Build failed with error code $exitCode" -ForegroundColor Red
    }
} else {
    Write-Host "Error: SCons not found at $sconsPath" -ForegroundColor Red
}

Write-Host ""
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
