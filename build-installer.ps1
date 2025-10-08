# Build OBS Keystroke History Installer
# This script compiles the Inno Setup script to create a Windows installer

Write-Host "Building OBS Keystroke History Installer..." -ForegroundColor Cyan
Write-Host ""

# Check if Inno Setup is installed
$InnoSetupPaths = @(
    "C:\Program Files (x86)\Inno Setup 6\ISCC.exe",
    "C:\Program Files\Inno Setup 6\ISCC.exe",
    "C:\Program Files (x86)\Inno Setup 5\ISCC.exe",
    "C:\Program Files\Inno Setup 5\ISCC.exe"
)

$ISCC = $null
foreach ($path in $InnoSetupPaths) {
    if (Test-Path $path) {
        $ISCC = $path
        Write-Host "Found Inno Setup Compiler at: $ISCC" -ForegroundColor Green
        break
    }
}

if ($null -eq $ISCC) {
    Write-Host "ERROR: Inno Setup Compiler not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install Inno Setup from: https://jrsoftware.org/isdl.php" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "After installation, run this script again." -ForegroundColor Yellow
    Write-Host ""
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if the DLL exists
if (-not (Test-Path "build\Release\keystroke-history.dll")) {
    Write-Host "ERROR: Plugin DLL not found!" -ForegroundColor Red
    Write-Host "Please build the plugin first by running: cmake --build build --config Release" -ForegroundColor Yellow
    Write-Host ""
    Read-Host "Press Enter to exit"
    exit 1
}

# Compile the installer
Write-Host ""
Write-Host "Compiling installer..." -ForegroundColor Cyan

& $ISCC "installer.iss"

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "SUCCESS! Installer created." -ForegroundColor Green
    Write-Host ""
    
    # Find the created installer
    $installer = Get-ChildItem -Path . -Filter "obs-keystroke-history-installer-v*.exe" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    
    if ($installer) {
        $size = [math]::Round($installer.Length / 1KB, 2)
        Write-Host "Installer: $($installer.Name)" -ForegroundColor Cyan
        Write-Host "Size: $size KB" -ForegroundColor Cyan
        Write-Host "Location: $($installer.FullName)" -ForegroundColor Cyan
    }
} else {
    Write-Host ""
    Write-Host "ERROR: Installer compilation failed!" -ForegroundColor Red
    Write-Host "Check the output above for errors." -ForegroundColor Yellow
}

Write-Host ""
Read-Host "Press Enter to exit"
