# Godot 编辑器启动脚本
Write-Host "正在启动 Godot 编辑器..." -ForegroundColor Green
Write-Host ""

# 检查 Godot 可执行文件是否存在
$godotExe = "bin\godot.windows.editor.dev.x86_64.exe"
$godotConsoleExe = "bin\godot.windows.editor.dev.x86_64.console.exe"

if (Test-Path $godotExe) {
    Write-Host "找到 Godot 编辑器，正在启动..." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "可执行文件: $godotExe" -ForegroundColor Cyan
    Write-Host ""
    
    try {
        Start-Process -FilePath $godotExe -ErrorAction Stop
        Write-Host "Godot 编辑器已启动！" -ForegroundColor Green
    } catch {
        Write-Host "启动失败: $($_.Exception.Message)" -ForegroundColor Red
    }
} elseif (Test-Path $godotConsoleExe) {
    Write-Host "找到 Godot 编辑器（控制台版本），正在启动..." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "可执行文件: $godotConsoleExe" -ForegroundColor Cyan
    Write-Host ""
    
    try {
        Start-Process -FilePath $godotConsoleExe -ErrorAction Stop
        Write-Host "Godot 编辑器已启动！" -ForegroundColor Green
    } catch {
        Write-Host "启动失败: $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "错误：未找到 Godot 可执行文件！" -ForegroundColor Red
    Write-Host ""
    Write-Host "请先运行 build.bat 构建 Godot" -ForegroundColor Yellow
    Write-Host "或者检查以下路径是否存在：" -ForegroundColor Yellow
    Write-Host "  - $godotExe" -ForegroundColor Cyan
    Write-Host "  - $godotConsoleExe" -ForegroundColor Cyan
    Write-Host ""
    Read-Host "按回车键退出"
    exit 1
}

Write-Host ""
Write-Host "提示：" -ForegroundColor Yellow
Write-Host "- 如果编辑器没有启动，请检查是否有杀毒软件阻止" -ForegroundColor Gray
Write-Host "- 首次启动可能需要较长时间" -ForegroundColor Gray
Write-Host "- 可以在命令行中运行以查看详细输出" -ForegroundColor Gray
Write-Host ""
Read-Host "按回车键退出"
