# Finalize Release v1.2.1
# Quick automation script for common finalization tasks

param(
    [switch]$SkipTests,
    [switch]$AutoCommit,
    [switch]$CleanOnly
)

$ErrorActionPreference = "Stop"
$VERSION = "1.2.1"
$PROJECT_ROOT = "d:\OBS Plugins\Keystroke History"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Release Finalization Script v$VERSION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Change to project directory
Set-Location $PROJECT_ROOT

# Step 1: Clean up old documentation files
if ($CleanOnly -or -not $SkipTests) {
    Write-Host "[1/7] Cleaning up old documentation files..." -ForegroundColor Yellow
    
    $filesToRemove = @(
        "UNICODE_ARROW_KEYS_FIX.md",
        "TESTING_v1.2.0.md",
        "RELEASE_v1.2.0.md", 
        "WHATS_NEW_v1.2.0.md"
    )
    
    foreach ($file in $filesToRemove) {
        if (Test-Path $file) {
            Remove-Item $file -Force
            Write-Host "  ✓ Removed: $file" -ForegroundColor Green
        } else {
            Write-Host "  - Not found: $file" -ForegroundColor Gray
        }
    }
    
    if ($CleanOnly) {
        Write-Host ""
        Write-Host "Cleanup complete!" -ForegroundColor Green
        exit 0
    }
}

# Step 2: Clean build directory
Write-Host ""
Write-Host "[2/7] Cleaning build directory..." -ForegroundColor Yellow

if (Test-Path "build") {
    Remove-Item "build" -Recurse -Force
    Write-Host "  ✓ Removed build directory" -ForegroundColor Green
}

if (Test-Path "package") {
    Remove-Item "package" -Recurse -Force
    Write-Host "  ✓ Removed package directory" -ForegroundColor Green
}

# Step 3: Configure CMake
Write-Host ""
Write-Host "[3/7] Configuring CMake..." -ForegroundColor Yellow

$cmakeArgs = @(
    "-S", ".",
    "-B", "build",
    "-G", "Visual Studio 17 2022",
    "-A", "x64"
)

$cmakeResult = & cmake @cmakeArgs 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ CMake configuration failed!" -ForegroundColor Red
    Write-Host $cmakeResult
    exit 1
}

Write-Host "  ✓ CMake configured successfully" -ForegroundColor Green

# Step 4: Build Release
Write-Host ""
Write-Host "[4/7] Building Release..." -ForegroundColor Yellow

$buildResult = & cmake --build build --config Release 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Build failed!" -ForegroundColor Red
    Write-Host $buildResult
    exit 1
}

Write-Host "  ✓ Build completed successfully" -ForegroundColor Green

# Verify DLL
if (-not (Test-Path "build\Release\keystroke-history.dll")) {
    Write-Host "  ✗ DLL not found!" -ForegroundColor Red
    exit 1
}

Write-Host "  ✓ DLL verified: build\Release\keystroke-history.dll" -ForegroundColor Green

# Step 5: Create packages
Write-Host ""
Write-Host "[5/7] Creating distribution packages..." -ForegroundColor Yellow

# Create manual install ZIP
Write-Host "  - Creating manual install ZIP..." -ForegroundColor Cyan
& .\create-package.ps1

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Package creation failed!" -ForegroundColor Red
    exit 1
}

# Rename ZIP with version
if (Test-Path "obs-keystroke-history-manual-install.zip") {
    $newZipName = "obs-keystroke-history-v$VERSION-manual-install.zip"
    Move-Item "obs-keystroke-history-manual-install.zip" $newZipName -Force
    Write-Host "  ✓ Created: $newZipName" -ForegroundColor Green
} else {
    Write-Host "  ✗ ZIP file not created!" -ForegroundColor Red
    exit 1
}

# Create Windows installer
Write-Host "  - Creating Windows installer..." -ForegroundColor Cyan
& .\build-installer.ps1

if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Installer creation failed!" -ForegroundColor Red
    exit 1
}

$installerName = "obs-keystroke-history-installer-v$VERSION.exe"
if (Test-Path $installerName) {
    Write-Host "  ✓ Created: $installerName" -ForegroundColor Green
} else {
    Write-Host "  ✗ Installer not created!" -ForegroundColor Red
    exit 1
}

# Step 6: Verify packages
Write-Host ""
Write-Host "[6/7] Verifying packages..." -ForegroundColor Yellow

$zipFile = "obs-keystroke-history-v$VERSION-manual-install.zip"
$exeFile = "obs-keystroke-history-installer-v$VERSION.exe"

$zipSize = (Get-Item $zipFile).Length
$exeSize = (Get-Item $exeFile).Length

Write-Host "  ✓ ZIP: $zipFile ($([math]::Round($zipSize/1KB, 2)) KB)" -ForegroundColor Green
Write-Host "  ✓ EXE: $exeFile ($([math]::Round($exeSize/1KB, 2)) KB)" -ForegroundColor Green

# Step 7: Git operations (if AutoCommit is enabled)
if ($AutoCommit) {
    Write-Host ""
    Write-Host "[7/7] Committing to Git..." -ForegroundColor Yellow
    
    # Check for uncommitted changes
    $gitStatus = & git status --porcelain
    
    if ($gitStatus) {
        Write-Host "  - Staging all changes..." -ForegroundColor Cyan
        & git add -A
        
        $commitMessage = @"
Release v$VERSION - Unicode arrow keys fix

- Fixed arrow keys display using Unicode symbols (←, ↑, →, ↓)
- Updated text renderer to use Unicode Windows API functions
- Changed CreateFontA to CreateFontW
- Changed DrawTextA to DrawTextW
- Added UTF-8 to wide string conversion
- Full Unicode support for all special characters
- Removed temporary documentation files
- Updated version to $VERSION
"@
        
        Write-Host "  - Committing changes..." -ForegroundColor Cyan
        & git commit -m $commitMessage
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "  ✓ Changes committed" -ForegroundColor Green
            
            Write-Host "  - Creating tag v$VERSION..." -ForegroundColor Cyan
            & git tag -a "v$VERSION" -m "Release v$VERSION - Unicode Arrow Keys Fix"
            
            if ($LASTEXITCODE -eq 0) {
                Write-Host "  ✓ Tag created: v$VERSION" -ForegroundColor Green
            } else {
                Write-Host "  ✗ Tag creation failed!" -ForegroundColor Red
            }
        } else {
            Write-Host "  ✗ Commit failed!" -ForegroundColor Red
        }
    } else {
        Write-Host "  - No changes to commit" -ForegroundColor Gray
    }
} else {
    Write-Host ""
    Write-Host "[7/7] Skipping Git commit (use -AutoCommit to enable)" -ForegroundColor Gray
}

# Summary
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  ✓ Finalization Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Distribution files created:" -ForegroundColor White
Write-Host "  • obs-keystroke-history-v$VERSION-manual-install.zip" -ForegroundColor Cyan
Write-Host "  • obs-keystroke-history-installer-v$VERSION.exe" -ForegroundColor Cyan
Write-Host ""

if (-not $AutoCommit) {
    Write-Host "Next steps:" -ForegroundColor Yellow
    Write-Host "  1. Review changes: git status" -ForegroundColor White
    Write-Host "  2. Commit: git commit -m 'Release v$VERSION'" -ForegroundColor White
    Write-Host "  3. Tag: git tag -a v$VERSION -m 'Release v$VERSION'" -ForegroundColor White
    Write-Host "  4. Push: git push origin main && git push origin v$VERSION" -ForegroundColor White
    Write-Host "  5. Create GitHub release with the distribution files" -ForegroundColor White
    Write-Host ""
}

Write-Host "Complete checklist: RELEASE_FINALIZATION_CHECKLIST.md" -ForegroundColor Cyan
Write-Host ""
