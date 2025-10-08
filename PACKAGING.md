# Creating Installer and Release Package

## Quick Summary

This guide covers creating distribution packages for the OBS Keystroke History Plugin.

## Option 1: Manual Installation ZIP (Ready to Use!)

The **manual installation ZIP** is already created and ready to distribute:

**Location**: `obs-keystroke-history-manual-install.zip`

### What's Included:
- `keystroke-history.dll` (plugin binary)
- `locale/en-US.ini` (UI translations)
- `INSTALL.txt` (installation instructions)
- `install-plugin.ps1` (automated installer script)

### To Recreate:
```powershell
.\create-package.ps1
```

### Users Install By:
1. Extract ZIP file
2. Run `install-plugin.ps1` as Administrator
   OR
3. Manually copy files following `INSTALL.txt`

---

## Option 2: Executable Installer (.exe)

### Requirements:
Download and install [Inno Setup](https://jrsoftware.org/isdl.php) (free)

### Creating the Installer:

1. **Install Inno Setup**:
   - Download from: https://jrsoftware.org/isdl.php
   - Install with default options

2. **Compile the Installer**:
   ```powershell
   # Option A: Using Inno Setup GUI
   # - Open installer.iss in Inno Setup
   # - Click Build → Compile
   
   # Option B: Using command line
   "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer.iss
   ```

3. **Output**:
   - File: `obs-keystroke-history-installer-v1.0.0.exe`
   - Location: Project root directory

### Installer Features:
- ✅ Checks if OBS Studio is installed
- ✅ Automatically finds OBS installation directory
- ✅ Installs to correct plugin folders
- ✅ Warns if OBS is running
- ✅ Provides option to launch OBS after install
- ✅ Clean uninstaller included

---

## Distribution Checklist

Before releasing:

### 1. Build the Plugin
```powershell
cd build
cmake --build . --config Release
```

### 2. Test the Plugin
- [ ] Install and test in OBS
- [ ] Verify all features work
- [ ] Check for errors in OBS log
- [ ] Test on clean OBS installation

### 3. Create Packages
```powershell
# Create manual ZIP
.\create-package.ps1

# Create installer EXE (requires Inno Setup)
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer.iss
```

### 4. Test Installation Packages
- [ ] Test manual ZIP installation
- [ ] Test exe installer
- [ ] Test uninstaller (if using .exe)
- [ ] Verify all files copied correctly

### 5. Prepare Release Assets
- [ ] `obs-keystroke-history-manual-install.zip`
- [ ] `obs-keystroke-history-installer-v1.0.0.exe` (optional)
- [ ] `README.md` (for release notes)
- [ ] Screenshots/demo GIF (recommended)

---

## GitHub Release Process

### Creating a Release

1. **Tag the version**:
   ```powershell
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

2. **Create Release on GitHub**:
   - Go to: https://github.com/dneesen/obs-keystroke-history/releases
   - Click "Draft a new release"
   - Select tag: `v1.0.0`
   - Title: `OBS Keystroke History v1.0.0`

3. **Upload Assets**:
   - `obs-keystroke-history-manual-install.zip`
   - `obs-keystroke-history-installer-v1.0.0.exe` (if created)

4. **Release Notes Template**:
   ```markdown
   ## OBS Keystroke History v1.0.0
   
   ### Features
   - ⌨️ Real-time keystroke and mouse action capture
   - 🔤 Uppercase key display (A, B, C)
   - 🔢 Key repetition counting (A x3, W x5)
   - 📝 Keystroke grouping for rapid typing
   - 🎯 Window filtering
   - 🔍 Scroll wheel capture
   - 🎨 Customizable fonts, colors, backgrounds
   - ⬆️⬇️ Choose display direction (newest on top or bottom)
   
   ### Installation
   
   **Option 1: Automated Installer (Recommended)**
   1. Download `obs-keystroke-history-installer-v1.0.0.exe`
   2. Run the installer
   3. Follow the prompts
   
   **Option 2: Manual Installation**
   1. Download `obs-keystroke-history-manual-install.zip`
   2. Extract the ZIP
   3. Run `install-plugin.ps1` as Administrator
      OR follow instructions in `INSTALL.txt`
   
   ### Requirements
   - Windows 10/11 (64-bit)
   - OBS Studio 32.0.1 or later
   
   ### Documentation
   - [README](https://github.com/dneesen/obs-keystroke-history/blob/main/README.md)
   - [Build Guide](https://github.com/dneesen/obs-keystroke-history/blob/main/BUILD_GUIDE.md)
   
   ### Support
   - [Report Issues](https://github.com/dneesen/obs-keystroke-history/issues)
   - [Discussion Forum](https://github.com/dneesen/obs-keystroke-history/discussions)
   ```

---

## File Sizes (Approximate)

- Manual ZIP: ~30 KB
- Installer EXE: ~35 KB
- Source code: ~50 KB

Total release size: ~115 KB (very lightweight!)

---

## Troubleshooting

### "Inno Setup not found"
- Download from: https://jrsoftware.org/isdl.php
- Make sure to install (not just download)

### "OBS Studio not found" during installer compilation
- The installer checks at runtime, not compile time
- This is normal - users need OBS installed, not developers

### ZIP file too large
- Make sure you're only including Release builds
- Don't include debug symbols (.pdb files)

### DLL not found during packaging
- Build the plugin first: `cmake --build . --config Release`
- Check that `build\Release\keystroke-history.dll` exists

---

## Automation (Optional)

### GitHub Actions CI/CD

You can automate building and releasing:

1. Create `.github/workflows/release.yml`
2. Configure to build on tag push
3. Automatically create releases with artifacts

Example workflow:
- Build plugin on Windows runner
- Create packages
- Upload to GitHub Releases

---

## Summary

**For quick distribution**:
1. Run `.\create-package.ps1`
2. Upload `obs-keystroke-history-manual-install.zip` to GitHub Releases

**For professional distribution**:
1. Install Inno Setup
2. Run `.\create-package.ps1`
3. Compile `installer.iss`
4. Upload both ZIP and EXE to GitHub Releases

Both methods work great - the ZIP is simpler, the EXE is more user-friendly!
