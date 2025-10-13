# Release Finalization Checklist

**Purpose:** Complete all steps after testing the Unicode arrow keys fix  
**Target Version:** v1.2.1 (Unicode Arrow Keys Fix)  
**Date:** October 13, 2025

---

## ✅ Pre-Finalization Checklist

### Testing Verification
- [ ] Arrow keys (←, ↑, →, ↓) display correctly in OBS
- [ ] All other special keys still work (Backspace, Enter, Tab, etc.)
- [ ] Text alignment (Left/Center/Right) works correctly
- [ ] Unicode characters render with no corruption
- [ ] Plugin loads without errors in OBS logs
- [ ] No performance issues or memory leaks
- [ ] Test with different fonts (Arial, Consolas, etc.)

---

## 📝 Step-by-Step Finalization Process

### Step 1: Update Version Number
```powershell
# Edit CMakeLists.txt - Update version
```

**File:** `CMakeLists.txt`

Change:
```cmake
project(keystroke-history VERSION 1.2.0)
```

To:
```cmake
project(keystroke-history VERSION 1.2.1)
```

### Step 2: Create Release Notes

**File:** Create `RELEASE_v1.2.1.md`

```markdown
# Release Notes: v1.2.1

**Release Date:** October 13, 2025  
**Type:** Bug Fix Release  
**Status:** Stable

## 🐛 Bug Fixes

### Unicode Arrow Keys Display
**Issue:** Arrow keys were not displaying correctly as Unicode symbols (←, ↑, →, ↓)

**Root Cause:** Text renderer was using ANSI Windows API functions (`CreateFontA`, `DrawTextA`) which don't support Unicode characters.

**Solution:** 
- Updated text renderer to use Unicode Windows API functions (`CreateFontW`, `DrawTextW`)
- Added proper UTF-8 to wide string conversion
- Arrow keys now display as proper Unicode arrow symbols

**Impact:** All arrow keys now render correctly with clear, intuitive symbols instead of garbage characters or boxes.

## 📥 Installation

### Method 1: Windows Installer (Recommended)
1. Download `obs-keystroke-history-installer-v1.2.1.exe`
2. Close OBS Studio
3. Run installer
4. Restart OBS Studio

### Method 2: Manual Installation
1. Download `obs-keystroke-history-manual-install.zip`
2. Close OBS Studio
3. Extract to OBS installation directory
4. Restart OBS Studio

## 🔄 Upgrading from v1.2.0
This is a drop-in replacement. Simply install over your existing version.

## ✨ What's Included
- All features from v1.2.0 (text alignment, fixed bottom anchoring)
- Fixed Unicode arrow keys display
- Full Unicode support for future enhancements
```

### Step 3: Update What's New Document

**File:** Create `WHATS_NEW_v1.2.1.md`

```markdown
# What's New in v1.2.1

## Bug Fix: Unicode Arrow Keys

### The Problem
In previous versions, arrow keys were supposed to display as Unicode symbols (←, ↑, →, ↓) but appeared as garbage characters or boxes instead.

### The Fix
The text renderer now properly supports Unicode characters!

**Technical Details:**
- Upgraded from ANSI text rendering to Unicode rendering
- Added UTF-8 to wide string conversion
- Full Unicode support for all keys and characters

### What You'll See
When you press arrow keys, you'll now see:
- **Left Arrow:** ←
- **Up Arrow:** ↑
- **Right Arrow:** →
- **Down Arrow:** ↓

Clear, intuitive symbols that are easy for viewers to understand!

### Side Benefits
This fix also enables:
- Support for international characters
- Potential for emoji support in future versions
- Better font compatibility

---

## Included from v1.2.0
- Text alignment (Left/Center/Right)
- Fixed bottom anchoring for newest-at-bottom mode
- Windows installer (.exe)
```

### Step 4: Update Main README

**File:** `README.md`

Update the version badge and features section:

```markdown
# OBS Keystroke History

**Version:** 1.2.1  
**Latest Update:** Unicode arrow keys fix
```

Add to features list:
```markdown
- ✨ **Unicode Support** - Proper display of arrow keys and special characters
```

### Step 5: Clean Up Temporary Files

Remove these files:
```powershell
Remove-Item "UNICODE_ARROW_KEYS_FIX.md" -Force
Remove-Item "TESTING_v1.2.0.md" -Force
Remove-Item "RELEASE_v1.2.0.md" -Force
Remove-Item "WHATS_NEW_v1.2.0.md" -Force
```

**Keep these:**
- `README.md` - Main documentation
- `BUILD_GUIDE.md` - Build instructions
- `PACKAGING.md` - Packaging instructions
- `LICENSE` - License file
- `RELEASE_v1.2.1.md` - Current release notes
- `WHATS_NEW_v1.2.1.md` - Current what's new

### Step 6: Update Installer Script

**File:** `installer.iss`

Update version references:
```ini
#define MyAppVersion "1.2.1"
```

Update installer filename:
```ini
OutputBaseFilename=obs-keystroke-history-installer-v1.2.1
```

### Step 7: Final Build

#### Clean Previous Build
```powershell
cd "d:\OBS Plugins\Keystroke History"
Remove-Item build -Recurse -Force
Remove-Item package -Recurse -Force
```

#### Fresh CMake Configuration
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

#### Build Release
```powershell
cmake --build build --config Release
```

#### Verify DLL Created
```powershell
Test-Path "build\Release\keystroke-history.dll"
# Should return: True
```

### Step 8: Create Distribution Packages

#### Create Manual Install ZIP
```powershell
.\create-package.ps1
```

**Verify contents of `obs-keystroke-history-manual-install.zip`:**
- `obs-plugins/64bit/keystroke-history.dll`
- `data/obs-plugins/keystroke-history/locale/en-US.ini`
- `install-plugin.ps1`
- `INSTALL.txt`

#### Rename ZIP for Version
```powershell
Move-Item "obs-keystroke-history-manual-install.zip" "obs-keystroke-history-v1.2.1-manual-install.zip" -Force
```

#### Create Windows Installer
```powershell
.\build-installer.ps1
```

**Verify installer created:**
```powershell
Test-Path "obs-keystroke-history-installer-v1.2.1.exe"
# Should return: True
```

### Step 9: Test Distribution Packages

#### Test Manual Install
```powershell
# Extract ZIP to temporary location
Expand-Archive "obs-keystroke-history-v1.2.1-manual-install.zip" -DestinationPath "test-manual" -Force

# Verify structure
Test-Path "test-manual\obs-plugins\64bit\keystroke-history.dll"
Test-Path "test-manual\data\obs-plugins\keystroke-history\locale\en-US.ini"

# Cleanup
Remove-Item "test-manual" -Recurse -Force
```

#### Test Installer (Optional)
1. Close OBS
2. Run `obs-keystroke-history-installer-v1.2.1.exe`
3. Complete installation
4. Launch OBS
5. Verify plugin appears in sources list
6. Test arrow keys display

### Step 10: Git Commit and Tag

#### Stage Changes
```powershell
git add -A
```

#### Review Changes
```powershell
git status
git diff --staged
```

**Expected changes:**
- `src/text-renderer.cpp` (Unicode rendering)
- `CMakeLists.txt` (version bump)
- `README.md` (version update)
- `installer.iss` (version update)
- New: `RELEASE_v1.2.1.md`
- New: `WHATS_NEW_v1.2.1.md`
- Deleted: Old documentation files

#### Commit Changes
```powershell
git commit -m "Release v1.2.1 - Unicode arrow keys fix

- Fixed arrow keys display using Unicode symbols (←, ↑, →, ↓)
- Updated text renderer to use Unicode Windows API functions
- Changed CreateFontA to CreateFontW
- Changed DrawTextA to DrawTextW
- Added UTF-8 to wide string conversion
- Full Unicode support for all special characters
- Removed temporary documentation files
- Updated version to 1.2.1"
```

#### Create Git Tag
```powershell
git tag -a v1.2.1 -m "Release v1.2.1 - Unicode Arrow Keys Fix"
```

#### Push to Repository
```powershell
git push origin main
git push origin v1.2.1
```

### Step 11: Create GitHub Release

1. Go to: https://github.com/dneesen/obs-keystroke-history/releases/new
2. Select tag: `v1.2.1`
3. Release title: `v1.2.1 - Unicode Arrow Keys Fix`
4. Description: Copy from `RELEASE_v1.2.1.md`
5. Attach files:
   - `obs-keystroke-history-installer-v1.2.1.exe`
   - `obs-keystroke-history-v1.2.1-manual-install.zip`
6. Click "Publish release"

---

## 📋 Final Verification Checklist

After publishing:

- [ ] GitHub release is visible and public
- [ ] Both installer and ZIP are downloadable
- [ ] Version number matches across all files
- [ ] Documentation is accurate
- [ ] Old documentation files removed
- [ ] Git tags pushed successfully
- [ ] Repository main branch is up to date

---

## 🎉 Release Complete!

Congratulations! v1.2.1 is now released with proper Unicode arrow key support.

### Next Steps
- Monitor GitHub issues for bug reports
- Announce release on relevant forums/communities
- Update any external documentation or websites
- Plan next feature release

### Support
If issues arise:
1. Check GitHub issues
2. Review OBS log files
3. Verify installation directory
4. Test with clean OBS profile

---

## 📞 Quick Reference Commands

```powershell
# Full release build from scratch
cd "d:\OBS Plugins\Keystroke History"
Remove-Item build -Recurse -Force
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\create-package.ps1
.\build-installer.ps1

# Git commit and push
git add -A
git commit -m "Release v1.2.1 - Unicode arrow keys fix"
git tag -a v1.2.1 -m "Release v1.2.1"
git push origin main
git push origin v1.2.1

# Cleanup old files
Remove-Item "UNICODE_ARROW_KEYS_FIX.md", "TESTING_v1.2.0.md", "RELEASE_v1.2.0.md", "WHATS_NEW_v1.2.0.md" -Force
```

---

**Document Version:** 1.0  
**Created:** October 13, 2025  
**Last Updated:** October 13, 2025
