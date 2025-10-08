# Quick Setup Script for Keystroke History Plugin
# This uses a simpler approach with minimal OBS build requirements

Write-Host "=== Keystroke History Plugin - Quick Setup ===" -ForegroundColor Cyan
Write-Host ""

# Step 1: Check CMake
Write-Host "[1/4] Checking for CMake..." -ForegroundColor Yellow
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Write-Host "ERROR: CMake not found! Please install CMake from https://cmake.org/download/" -ForegroundColor Red
    exit 1
}
Write-Host "  OK CMake found" -ForegroundColor Green

# Step 2: Check OBS source
Write-Host "[2/4] Checking OBS source code..." -ForegroundColor Yellow
if (-not (Test-Path "C:\obs-studio")) {
    Write-Host "  Downloading OBS source code..." -ForegroundColor Cyan
    Set-Location C:\
    git clone --recursive https://github.com/obsproject/obs-studio.git
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Failed to clone OBS repository!" -ForegroundColor Red
        exit 1
    }
}
Write-Host "  OK OBS source found" -ForegroundColor Green

# Step 3: Build minimal OBS (just what we need)
Write-Host "[3/4] Building OBS libraries..." -ForegroundColor Yellow
Write-Host "  This may take 15-30 minutes on first run..." -ForegroundColor Cyan

Set-Location C:\obs-studio

if (-not (Test-Path "build\config\obsconfig.h")) {
    if (-not (Test-Path build)) {
        mkdir build | Out-Null
    }
    Set-Location build
    
    Write-Host "  Configuring OBS build..." -ForegroundColor Cyan
    cmake .. -G "Visual Studio 17 2022" -A x64 -DENABLE_PLUGINS=OFF -DENABLE_UI=OFF
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "  Building libobs (this takes 10-20 minutes)..." -ForegroundColor Cyan
    cmake --build . --config Release --target libobs
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Build failed!" -ForegroundColor Red
        exit 1
    }
}
Write-Host "  OK OBS libraries ready" -ForegroundColor Green

# Step 4: Build plugin
Write-Host "[4/4] Building Keystroke History plugin..." -ForegroundColor Yellow
Set-Location "D:\OBS Plugins\Keystroke History"

if (Test-Path build) {
    Remove-Item -Recurse -Force build
}
mkdir build | Out-Null
Set-Location build

Write-Host "  Configuring plugin..." -ForegroundColor Cyan
cmake .. -DLIBOBS_INCLUDE_DIR="C:/obs-studio/libobs" -DOBS_INSTALL_DIR="C:/Program Files/obs-studio" -G "Visual Studio 17 2022" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Plugin configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host "  Building plugin..." -ForegroundColor Cyan
cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Plugin build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "=== BUILD SUCCESSFUL! ===" -ForegroundColor Green
Write-Host ""
Write-Host "Next: Install the plugin with administrator rights:" -ForegroundColor Yellow
Write-Host "  cmake --install . --config Release" -ForegroundColor Cyan
Write-Host ""
Write-Host "Then restart OBS Studio and add the Keystroke History source!" -ForegroundColor Green
