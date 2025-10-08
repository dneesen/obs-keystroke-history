# GitHub Release Creation Guide - v1.1.0

## Option 1: Manual GitHub Web Interface (Recommended)

### Step 1: Navigate to Releases
1. Go to: https://github.com/dneesen/obs-keystroke-history/releases
2. Click **"Draft a new release"** button

### Step 2: Configure Release
Fill in the form with these details:

**Choose a tag:**
- Select existing tag: `v1.1.0`
- Or type: `v1.1.0` (if not in dropdown)

**Release title:**
```
Keystroke History v1.1.0 - Major Feature Release
```

**Release description:**
```markdown
# What's New in Version 1.1.0

## 🎯 Major New Features

### 1. **Display Direction Control**
Choose how keystrokes are displayed:
- **Newest at Top**: New keystrokes appear at the top (great for gaming/streaming)
- **Newest at Bottom**: New keystrokes appear at the bottom (classic display mode)

Find it in: **Source Properties → Display Newest Entries at Top**

### 2. **Enhanced Font Selection**
Simplified font selection with a dropdown menu featuring 15 commonly available fonts:
- Arial, Calibri, Consolas, Courier New, Georgia, Impact, Lucida Console
- Segoe UI, Tahoma, Times New Roman, Trebuchet MS, Verdana, and more!

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

**Option B: Automated Installer (EXE)** _(optional, not included)_
- Can be compiled from included Inno Setup script
- One-click setup with uninstaller included

---

## 🔧 Improvements

- **Better logging**: Enhanced debug information for filtering and source detection
- **Cleaner UI**: Reorganized source properties for better user experience
- **More robust**: Improved error handling for edge cases
- **Fixed transparent backgrounds**: No more color bleeding around text edges
- **Proper anchor points**: Text now anchors correctly at top or bottom

---

## 📋 Technical Details

- **Minimum OBS Version**: 32.0.1
- **Platform**: Windows 10/11
- **File Size**: ~34 KB (manual ZIP)
- **Dependencies**: None (native OBS plugin)

---

## 🔄 Upgrading from 1.0.x

Simply install the new version over your existing installation. All settings will be preserved.

The new filtering options are **opt-in** - your existing window title filter (if configured) will continue to work exactly as before.

---

## ⚠️ Known Limitations

**Source List Shows All Scenes**: The OBS Capture Source dropdown shows sources from ALL scenes, not just the current scene. This is due to OBS API limitations - the Frontend API required for scene detection isn't available to plugins. Simply ignore sources from other scenes or delete unused sources from your project.

---

## 📖 Documentation

- **Installation Guide**: See `INSTALL.txt` in the ZIP file
- **Testing Guide**: Available in repository as `TESTING_v1.1.0.md`
- **Build Guide**: Available in repository as `BUILD_GUIDE.md`

---

## 🐛 Feedback & Support

Found a bug? Have a feature request?
- **Issues**: https://github.com/dneesen/obs-keystroke-history/issues
- **Discussions**: https://github.com/dneesen/obs-keystroke-history/discussions

---

## 🙏 Thank You

Thank you for using Keystroke History! If you find this plugin useful, please consider:
- ⭐ Starring the repository
- 🐛 Reporting bugs
- 💡 Suggesting features
- 📢 Sharing with others

---

**Version**: 1.1.0  
**Release Date**: October 8, 2025  
**Build**: Release configuration  
**Tested with**: OBS Studio 32.0.1 on Windows 10/11
```

### Step 3: Upload File
1. Drag and drop or click to upload: `obs-keystroke-history-manual-install.zip`
   - Location: `d:\OBS Plugins\Keystroke History\obs-keystroke-history-manual-install.zip`
   - Size: ~34 KB

### Step 4: Additional Settings
- ✅ Check **"Set as the latest release"**
- ✅ Check **"Create a discussion for this release"** (optional)

### Step 5: Publish
Click **"Publish release"** button

---

## Option 2: Using GitHub CLI (After Installation)

If you want to install GitHub CLI for future releases:

### Install GitHub CLI:
```powershell
winget install --id GitHub.cli
```

### Then create release with CLI:
```powershell
cd "d:\OBS Plugins\Keystroke History"

gh release create v1.1.0 `
  --title "Keystroke History v1.1.0 - Major Feature Release" `
  --notes-file WHATS_NEW_v1.1.0.md `
  obs-keystroke-history-manual-install.zip
```

---

## Post-Release Checklist

After creating the release:

1. **Verify Release Page**:
   - Check: https://github.com/dneesen/obs-keystroke-history/releases/latest
   - Ensure ZIP file is downloadable
   - Verify release notes display correctly

2. **Test Installation**:
   - Download ZIP from release page
   - Extract and test installation
   - Verify plugin loads in OBS

3. **Update README** (if needed):
   - Add installation link pointing to latest release
   - Update version badges

4. **Announce** (optional):
   - OBS Forums
   - Reddit (r/obs)
   - Discord servers

---

## Quick Links

- **Repository**: https://github.com/dneesen/obs-keystroke-history
- **Releases**: https://github.com/dneesen/obs-keystroke-history/releases
- **Issues**: https://github.com/dneesen/obs-keystroke-history/issues
- **ZIP File Location**: `d:\OBS Plugins\Keystroke History\obs-keystroke-history-manual-install.zip`

---

## Troubleshooting

**Can't find v1.1.0 tag in dropdown?**
- The tag was already pushed: `git push origin v1.1.0`
- Refresh the page or type `v1.1.0` manually

**ZIP file too large?**
- Current size: ~34 KB (perfectly fine)
- GitHub limit: 2 GB (no issues)

**Release notes not formatting correctly?**
- Use the markdown text provided above
- Preview before publishing

---

**Need help?** Open an issue or discussion on GitHub.
