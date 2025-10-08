# Build Guide for Keystroke History Plugin

This guide will walk you through building the Keystroke History plugin for OBS Studio on Windows using Visual Studio.

## Step 1: Install Prerequisites

### 1.1 Visual Studio
You mentioned you have Visual Studio installed. Make sure you have:
- **Desktop development with C++** workload installed
- Windows 10 SDK

To verify/add components:
1. Open Visual Studio Installer
2. Click "Modify" on your Visual Studio installation
3. Ensure "Desktop development with C++" is checked
4. Under "Individual Components", verify you have a Windows SDK

### 1.2 Install CMake
Download and install CMake from: https://cmake.org/download/

Choose "Add CMake to system PATH" during installation.

### 1.3 Install Git (if not already installed)
Download from: https://git-scm.com/download/win

## Step 2: Get OBS Studio Source Code

The plugin needs OBS header files to compile. Download the OBS source:

```powershell
cd C:\
git clone --recursive https://github.com/obsproject/obs-studio.git
```

This will create `C:\obs-studio\` with all the necessary header files.

## Step 3: Configure the Build

Open PowerShell in the plugin directory:

```powershell
cd "d:\OBS Plugins\Keystroke History"
mkdir build
cd build
```

Now configure CMake. You'll need to tell it where to find OBS:

```powershell


```

**Note**: 
- Adjust paths if your OBS source or installation is in a different location
- Adjust the Visual Studio version if you have a different version:
- Visual Studio 2022 = "Visual Studio 17 2022"
- Visual Studio 2019 = "Visual Studio 16 2019"

## Step 4: Build the Plugin

### Option A: Build with CMake (Command Line)
```powershell
cmake --build . --config Release
```

### Option B: Build with Visual Studio (GUI)
1. Open `keystroke-history.sln` in the `build` folder
2. Set build configuration to "Release"
3. Right-click the solution → "Build Solution"

## Step 5: Install the Plugin

### Automatic Installation
```powershell
cmake --install . --config Release
```

### Manual Installation
If automatic installation doesn't work:

1. Copy `build/Release/keystroke-history.dll` to:
   ```
   C:\Program Files\obs-studio\obs-plugins\64bit\
   ```

2. Copy the `data` folder to:
   ```
   C:\Program Files\obs-studio\data\obs-plugins\keystroke-history\
   ```

You may need administrator privileges to copy to Program Files.

## Step 6: Test the Plugin

1. **Close OBS Studio** if it's running
2. **Restart OBS Studio**
3. Check the log file for plugin loading:
   - Go to Help → Log Files → View Current Log
   - Look for "[Keystroke History]" entries
4. Add a new source:
   - Sources → + → **Keystroke History**
5. Test by typing some keys - they should appear in the overlay!

## Troubleshooting

### Error: "Cannot find libobs"
**Solution**: Make sure you provided the correct path to OBS Studio installation:
```powershell
cmake .. -DCMAKE_PREFIX_PATH="C:/Program Files/obs-studio"
```

### Error: "Cannot find obs-module.h"
**Solution**: Download the OBS Studio source code and point to it:
```powershell
cmake .. -DLIBOBS_INCLUDE_DIR="C:/obs-studio/libobs"
```

### Plugin doesn't show in OBS
**Checklist**:
1. Is the DLL in the correct folder? `C:\Program Files\obs-studio\obs-plugins\64bit\`
2. Did you restart OBS after copying the DLL?
3. Check OBS log file (Help → Log Files) for errors
4. Try running OBS as Administrator

### Keystrokes not being captured
**Solution**: Run OBS Studio as Administrator (low-level keyboard hooks on Windows may require elevated permissions)

### Build fails with missing Windows.h
**Solution**: Install Windows SDK through Visual Studio Installer

## Quick Reference

**Full Build Command Chain**:
```powershell
cd "d:\OBS Plugins\Keystroke History"
mkdir build -Force
cd build
cmake .. -DLIBOBS_INCLUDE_DIR="C:/obs-studio/libobs" -DOBS_INSTALL_DIR="C:/Program Files/obs-studio" -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cmake --install . --config Release
```

## Next Steps

After successful build and installation:
1. Configure the plugin settings in OBS
2. Customize colors, font, and display options
3. Position the overlay where you want it in your scene

Need help? Check the main README.md for usage instructions and settings explanations.
