# Install Keystroke History Plugin to OBS
# Run this script with Administrator privileges after building

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "ERROR: This script must be run as Administrator!" -ForegroundColor Red
    Write-Host "Right-click PowerShell and select 'Run as Administrator'" -ForegroundColor Yellow
    exit 1
}

Write-Host "=== Installing Keystroke History Plugin ===" -ForegroundColor Cyan
Write-Host ""

$pluginDll = "D:\OBS Plugins\Keystroke History\build\Release\keystroke-history.dll"
$dataFolder = "D:\OBS Plugins\Keystroke History\data"
$obsPluginDir = "C:\Program Files\obs-studio\obs-plugins\64bit"
$obsDataDir = "C:\Program Files\obs-studio\data\obs-plugins\keystroke-history"

# Check if plugin was built
if (-not (Test-Path $pluginDll)) {
    Write-Host "ERROR: Plugin DLL not found!" -ForegroundColor Red
    Write-Host "Please build the plugin first by running quick-build.ps1" -ForegroundColor Yellow
    exit 1
}

# Copy plugin DLL
Write-Host "Installing plugin DLL..." -ForegroundColor Yellow
if (-not (Test-Path $obsPluginDir)) {
    mkdir $obsPluginDir -Force | Out-Null
}
Copy-Item $pluginDll $obsPluginDir -Force
Write-Host "  OK Copied to $obsPluginDir" -ForegroundColor Green

# Copy data files
Write-Host "Installing data files..." -ForegroundColor Yellow
if (Test-Path $dataFolder) {
    if (-not (Test-Path $obsDataDir)) {
        mkdir $obsDataDir -Force | Out-Null
    }
    Copy-Item "$dataFolder\*" $obsDataDir -Recurse -Force
    Write-Host "  OK Copied to $obsDataDir" -ForegroundColor Green
} else {
    Write-Host "  WARNING: Data folder not found, skipping..." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "=== INSTALLATION COMPLETE! ===" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Close OBS Studio if it's running" -ForegroundColor Cyan
Write-Host "  2. Start OBS Studio" -ForegroundColor Cyan
Write-Host "  3. Add a new source: Sources -> + -> Keystroke History" -ForegroundColor Cyan
Write-Host "  4. Configure settings and position in your scene" -ForegroundColor Cyan
Write-Host "  5. Start typing to see it work!" -ForegroundColor Cyan
Write-Host ""
Write-Host "Note: You may need to run OBS as Administrator for keyboard capture to work." -ForegroundColor Yellow
