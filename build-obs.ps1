# Build OBS Studio from source to get development headers
# This script configures and builds only what's needed for plugin development

Write-Host "Building OBS Studio from source..." -ForegroundColor Green
Write-Host "This will take about 30-60 minutes depending on your system." -ForegroundColor Yellow
Write-Host ""

# Check if obs-studio source exists
if (-not (Test-Path "C:\obs-studio")) {
    Write-Host "ERROR: OBS Studio source not found at C:\obs-studio" -ForegroundColor Red
    Write-Host "Please run: git clone --recursive https://github.com/obsproject/obs-studio.git C:\obs-studio" -ForegroundColor Yellow
    exit 1
}

# Navigate to OBS source
cd C:\obs-studio

# Create build directory
if (Test-Path build) {
    Write-Host "Removing old build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force build
}
mkdir build | Out-Null
cd build

Write-Host "Configuring OBS with CMake..." -ForegroundColor Green

# Configure OBS build
cmake .. `
    -G "Visual Studio 17 2022" -A x64 `
    -DENABLE_PLUGINS=OFF `
    -DENABLE_UI=OFF `
    -DENABLE_SCRIPTING=OFF `
    -DBUILD_TESTS=OFF

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Building OBS (this will take a while)..." -ForegroundColor Green

# Build only libobs (what we need for plugin development)
cmake --build . --config Release --target obs

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "SUCCESS! OBS built successfully." -ForegroundColor Green
Write-Host "You can now build the keystroke-history plugin." -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. cd 'd:\OBS Plugins\Keystroke History\build'" -ForegroundColor Cyan
Write-Host "2. cmake .. -DLIBOBS_INCLUDE_DIR='C:/obs-studio/libobs' -DOBS_INSTALL_DIR='C:/Program Files/obs-studio' -G 'Visual Studio 17 2022' -A x64" -ForegroundColor Cyan
Write-Host "3. cmake --build . --config Release" -ForegroundColor Cyan
