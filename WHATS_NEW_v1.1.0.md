# What's New in Version 1.1.0

## Major New Features

### 1. **Display Direction Control**
Choose how keystrokes are displayed:
- **Newest at Top**: New keystrokes appear at the top (great for gaming/streaming)
- **Newest at Bottom**: New keystrokes appear at the bottom (classic display mode)

Find it in: **Source Properties → Display Newest Entries at Top**

### 2. **Enhanced Font Selection**
Simplified font selection with a dropdown menu featuring 15 commonly available fonts:
- Arial
- Calibri  
- Consolas
- Courier New
- Georgia
- Impact
- Lucida Console
- Segoe UI
- Tahoma
- Times New Roman
- Trebuchet MS
- Verdana
- And more!

No more typing font names - just select from the dropdown!

### 3. **OBS Source-Based Filtering** ⭐ NEW
Advanced filtering that integrates directly with your OBS sources:
- **Select from your OBS sources**: Choose from Display Capture, Window Capture, or Game Capture sources
- **Automatic matching**: Plugin detects which display/window/game is currently active
- **More intuitive**: No need to type window titles - just pick the source you're already capturing

**How it works:**
1. Enable "Window/Source Filtering" checkbox
2. Check "Use OBS Source Capture"
3. Select your capture source from the dropdown (e.g., "Display 2", "Game Capture", "Chrome Window")
4. Keystrokes will only be captured when that source's target is active

**Traditional window title filtering still available** for users who prefer it.

### 4. **Improved Distribution & Installation**
Two installation options to suit your preference:

**Option A: Manual Installation (ZIP)**
- Download `obs-keystroke-history-manual-install.zip`
- Extract and run the included PowerShell installer
- Simple, portable, no admin rights needed

**Option B: Automated Installer (EXE)**
- Double-click installer
- Automatically detects OBS installation
- One-click setup with uninstaller included

## Improvements

- **Better logging**: Enhanced debug information for filtering and source detection
- **Cleaner UI**: Reorganized source properties for better user experience
- **More robust**: Improved error handling for edge cases

## Technical Details

- **Minimum OBS Version**: 32.0.1
- **Platform**: Windows 10/11
- **File Size**: ~30 KB (manual ZIP)
- **Dependencies**: None (native OBS plugin)

## Upgrading from 1.0.x

Simply install the new version over your existing installation. All settings will be preserved.

The new filtering options are **opt-in** - your existing window title filter (if configured) will continue to work exactly as before.

## Feedback & Support

Found a bug? Have a feature request?
- GitHub Issues: https://github.com/dneesen/obs-keystroke-history/issues
- Discussions: https://github.com/dneesen/obs-keystroke-history/discussions

---

**Thank you for using Keystroke History!**
