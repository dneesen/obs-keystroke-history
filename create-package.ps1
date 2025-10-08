# OBS Keystroke History Plugin - Package Creator
# This script creates:
# 1. A manual installation ZIP file
# 2. Directory structure for installer creation

$ErrorActionPreference = "Stop"

$ProjectRoot = "D:\OBS Plugins\Keystroke History"
$BuildDir = "$ProjectRoot\build\Release"
$PackageDir = "$ProjectRoot\package"
$ZipOutput = "$ProjectRoot\obs-keystroke-history-manual-install.zip"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "  OBS Keystroke History - Packager" -ForegroundColor Yellow
Write-Host "========================================`n" -ForegroundColor Cyan

# Check if DLL exists
if (-not (Test-Path "$BuildDir\keystroke-history.dll")) {
    Write-Host "ERROR: Plugin DLL not found!" -ForegroundColor Red
    Write-Host "Please build the plugin first with: cmake --build . --config Release" -ForegroundColor Yellow
    exit 1
}

# Clean and create package directory
Write-Host "[1/4] Creating package directory..." -ForegroundColor Green
if (Test-Path $PackageDir) {
    Remove-Item -Recurse -Force $PackageDir
}
New-Item -ItemType Directory -Path $PackageDir | Out-Null
New-Item -ItemType Directory -Path "$PackageDir\obs-plugins\64bit" | Out-Null
New-Item -ItemType Directory -Path "$PackageDir\data\obs-plugins\keystroke-history\locale" | Out-Null

# Copy DLL
Write-Host "[2/4] Copying plugin files..." -ForegroundColor Green
Copy-Item "$BuildDir\keystroke-history.dll" -Destination "$PackageDir\obs-plugins\64bit\"

# Copy locale files
Copy-Item "$ProjectRoot\data\locale\en-US.ini" -Destination "$PackageDir\data\obs-plugins\keystroke-history\locale\"

# Create README for manual installation
Write-Host "[3/4] Creating installation instructions..." -ForegroundColor Green
$InstallReadme = @"
# OBS Keystroke History Plugin - Manual Installation

## Installation Instructions

1. Close OBS Studio if it's running

2. Copy the files to your OBS installation directory:
   
   **Default OBS Location**: C:\Program Files\obs-studio\

   - Copy `obs-plugins\64bit\keystroke-history.dll` 
     to: `C:\Program Files\obs-studio\obs-plugins\64bit\`
   
   - Copy `data\obs-plugins\keystroke-history\` folder
     to: `C:\Program Files\obs-studio\data\obs-plugins\`

3. Start OBS Studio

4. Add the source: Sources → + → Keystroke History

## Alternative: Automated Installation

Run the included `install-plugin.ps1` script as Administrator:

``````powershell
Right-click install-plugin.ps1 → Run with PowerShell
``````

## Verification

After installation, you should see:
- C:\Program Files\obs-studio\obs-plugins\64bit\keystroke-history.dll
- C:\Program Files\obs-studio\data\obs-plugins\keystroke-history\locale\en-US.ini

## Troubleshooting

If the plugin doesn't appear in OBS:
1. Make sure OBS is completely closed before installing
2. Verify files are in the correct directories
3. Run OBS as Administrator
4. Check OBS log file: %appdata%\obs-studio\logs\

## Features

- Real-time keystroke and mouse action capture
- Uppercase key display (A, B, C)
- Key repetition counting (A x3, W x5)
- Keystroke grouping for rapid typing
- Window filtering
- Scroll wheel capture
- Customizable fonts, colors, and backgrounds
- Display direction (newest on top or bottom)

For more information, visit:
https://github.com/dneesen/obs-keystroke-history
"@

Set-Content -Path "$PackageDir\INSTALL.txt" -Value $InstallReadme

# Copy installer script
Copy-Item "$ProjectRoot\install-plugin.ps1" -Destination "$PackageDir\"

# Create ZIP file
Write-Host "[4/4] Creating ZIP archive..." -ForegroundColor Green
if (Test-Path $ZipOutput) {
    Remove-Item $ZipOutput
}

Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::CreateFromDirectory($PackageDir, $ZipOutput)

# Clean up package directory (optional, comment out to keep it)
# Remove-Item -Recurse -Force $PackageDir

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "  PACKAGING COMPLETE!" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "Manual Installation ZIP:" -ForegroundColor Yellow
Write-Host "  $ZipOutput`n" -ForegroundColor White

Write-Host "Package Contents:" -ForegroundColor Yellow
Write-Host "   keystroke-history.dll (64-bit plugin)" -ForegroundColor Gray
Write-Host "   locale files (en-US.ini)" -ForegroundColor Gray
Write-Host "   INSTALL.txt (installation instructions)" -ForegroundColor Gray
Write-Host "   install-plugin.ps1 (automated installer)`n" -ForegroundColor Gray

$ZipSize = (Get-Item $ZipOutput).Length / 1KB
Write-Host "ZIP File Size: $([math]::Round($ZipSize, 2)) KB`n" -ForegroundColor Cyan

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "NEXT STEPS:" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "1. Test the ZIP file by extracting and following INSTALL.txt" -ForegroundColor White
Write-Host "2. Upload to GitHub Releases" -ForegroundColor White
Write-Host "3. Users can download and manually install`n" -ForegroundColor White

Write-Host "For automated installer (.exe):" -ForegroundColor Yellow
Write-Host "  Use NSIS, Inno Setup, or WiX Toolset to create installer" -ForegroundColor White
Write-Host "  Package directory is ready at: $PackageDir`n" -ForegroundColor Gray
