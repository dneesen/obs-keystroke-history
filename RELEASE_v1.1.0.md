# Keystroke History v1.1.0 - Release Summary

## 🎉 Release Status: COMPLETE

**Release Date:** October 8, 2025  
**Package Size:** 33.74 KB  
**GitHub Tag:** v1.1.0  
**Repository:** https://github.com/dneesen/obs-keystroke-history

---

## 📦 Distribution Files

### Manual Installation (ZIP)
- **File:** `obs-keystroke-history-manual-install.zip` (33.74 KB)
- **Contents:**
  - `keystroke-history.dll` (64-bit plugin)
  - Locale files (`en-US.ini`)
  - `INSTALL.txt` (installation instructions)
  - `install-plugin.ps1` (automated installer script)

### Installation Methods
1. **PowerShell Installer** (Recommended):
   - Extract ZIP
   - Run PowerShell as Administrator
   - Execute `install-plugin.ps1`
   - Restart OBS

2. **Manual Installation**:
   - Follow instructions in `INSTALL.txt`
   - Copy files to OBS plugin directories
   - Restart OBS

---

## ✨ New Features in v1.1.0

### 1. Display Direction Control
**What it does:** Choose whether new keystrokes appear at the top or bottom of the display.

**Settings:**
- ☐ **Display Newest Entries at Top** (unchecked = default)
  - Newest appears at BOTTOM
  - Stacks upward
  - Anchor point at bottom edge
  - Perfect for bottom-anchored overlays

- ☑ **Display Newest Entries at Top** (checked)
  - Newest appears at TOP
  - Stacks downward
  - Anchor point at top edge
  - Perfect for top-anchored overlays

**Why it's useful:** Allows proper positioning in your scene without text jumping around.

---

### 2. Font Dropdown Selection
**What it does:** Replaces text input with a curated dropdown of 15 cross-platform fonts.

**Available Fonts:**
- Arial (default)
- Arial Black
- Calibri
- Cambria
- Comic Sans MS
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

**Why it's useful:** No more typos in font names, only tested fonts available.

---

### 3. OBS Source-Based Filtering ⭐ MAJOR FEATURE
**What it does:** Filter keystrokes based on which Display/Window/Game you're capturing in OBS.

**How to use:**
1. Enable "Window/Source Filtering" checkbox
2. Check "Use OBS Source Capture"
3. Select your capture source from dropdown
4. Keystrokes only captured when that source's target is active

**Supported Source Types:**
- **Display Capture:** Filters by which monitor is active
- **Window Capture:** Filters by which window is focused
- **Game Capture:** Filters by which game/application is running

**Why it's useful:** 
- More intuitive than typing window titles
- Matches exactly what you're capturing in OBS
- No guessing at window names

**Note:** Dropdown shows sources from ALL scenes (OBS API limitation). Simply ignore sources from other scenes.

---

### 4. Enhanced Text Rendering
**What it does:** Improved transparent background handling with proper anti-aliasing.

**Improvements:**
- Clean text with no background color bleeding
- Works correctly even if text and background colors are the same
- Proper anti-aliasing on all text edges
- No artifacts or halos around text

**Why it's useful:** Professional-looking text overlays with transparency.

---

## 🔧 Technical Improvements

### Display Capture Filtering
- Fixed monitor detection using `monitor_id` (device string)
- Proper Windows display device enumeration
- Accurate matching between OBS sources and active displays

### Text Rendering Algorithm
- Inverse color fill for transparent backgrounds
- Improved pixel detection for anti-aliasing
- Constant source height based on max_entries setting
- Proper anchor point positioning

### Source Enumeration
- Enumerates all Display/Window/Game Capture sources
- Shows sources across all scenes (standard OBS behavior)
- Filtered by source type (only capture sources)

### Logging & Debugging
- Enhanced `[SOURCE-FILTER]` logging
- Monitor device and settings logging
- Better error messages and troubleshooting info

---

## 📋 Known Limitations

### Scene Filtering Not Available
**Issue:** Source dropdown shows sources from ALL scenes, not just the current scene.

**Reason:** The OBS Frontend API required for scene detection isn't available to plugins.

**Impact:** Minimal - this is standard behavior for OBS plugins. Users can simply ignore sources from inactive scenes.

**Workaround:** Delete unused sources from scenes you don't use, or simply ignore them in the dropdown.

---

## 🧪 Testing Status

All features have been tested and verified:

✅ **Display Direction & Anchoring**
- Newest at bottom anchors properly
- Newest at top anchors properly
- Source height remains constant

✅ **Font Dropdown**
- All 15 fonts render correctly
- No text input field issues

✅ **OBS Source Filtering**
- Display Capture filtering works
- Window Capture filtering works
- Game Capture filtering works
- Source dropdown populates correctly

✅ **Transparent Background Rendering**
- No background color bleeding
- Works with all color combinations
- Proper anti-aliasing

✅ **Backward Compatibility**
- Window title filtering still works
- All existing settings preserved
- No breaking changes

See `TESTING_v1.1.0.md` for detailed testing procedures.

---

## 📖 Documentation

### User Documentation
- `README.md` - Main plugin documentation
- `WHATS_NEW_v1.1.0.md` - User-friendly feature summary
- `TESTING_v1.1.0.md` - Comprehensive testing guide
- `INSTALL.txt` - Installation instructions (in ZIP)

### Developer Documentation
- `BUILD_GUIDE.md` - Build instructions
- `PACKAGING.md` - Distribution packaging guide
- `CMakeLists.txt` - Build configuration

---

## 🚀 GitHub Release Checklist

- [x] All code committed to main branch
- [x] Version tagged as v1.1.0
- [x] Tag pushed to GitHub
- [x] Distribution ZIP created (33.74 KB)
- [x] Testing guide updated
- [x] Release notes written

### To Create GitHub Release:

1. Go to: https://github.com/dneesen/obs-keystroke-history/releases
2. Click "Draft a new release"
3. Choose tag: `v1.1.0`
4. Release title: `Keystroke History v1.1.0 - Major Feature Release`
5. Copy release notes from `WHATS_NEW_v1.1.0.md`
6. Upload: `obs-keystroke-history-manual-install.zip`
7. Mark as "Latest release"
8. Publish release

---

## 📊 Version Comparison

| Feature | v1.0.0 | v1.1.0 |
|---------|--------|--------|
| Display Direction Control | ❌ | ✅ |
| Font Dropdown | ❌ | ✅ |
| OBS Source Filtering | ❌ | ✅ |
| Window Title Filtering | ✅ | ✅ |
| Transparent Backgrounds | ⚠️ (issues) | ✅ (fixed) |
| Package Size | ~30 KB | 33.74 KB |
| Distribution Package | ❌ | ✅ |

---

## 💬 Support & Feedback

**Issues:** https://github.com/dneesen/obs-keystroke-history/issues  
**Discussions:** https://github.com/dneesen/obs-keystroke-history/discussions

---

## 🙏 Acknowledgments

Special thanks to all testers and contributors!

---

**Version:** 1.1.0  
**Build Date:** October 8, 2025  
**Platform:** Windows 10/11  
**OBS Version:** 32.0.1+  
**License:** See LICENSE file
