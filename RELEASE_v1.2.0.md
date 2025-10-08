# Release Notes: v1.2.0

**Release Date:** October 8, 2025  
**Type:** Feature Release  
**Status:** Ready for Testing

---

## 📦 What's Included

This release adds text justification controls and fixes critical alignment issues.

### New Features
1. **Text Justification** - Choose between Left, Center, or Right alignment
2. **Fixed Bottom Anchoring** - Newest-at-bottom mode now properly anchors to the bottom edge
3. **Windows Installer** - Convenient .exe installer for easy installation

### Bug Fixes
- Fixed: Entries not anchoring to bottom when "Display Newest on Top" is unchecked
- Fixed: Entries floating in middle of display area with fewer than max entries

---

## 📥 Installation

### Method 1: Windows Installer (Recommended)
1. Download `obs-keystroke-history-installer-v1.2.0.exe`
2. Close OBS Studio
3. Run installer (requires administrator privileges)
4. Restart OBS Studio

### Method 2: Manual Installation
1. Download `obs-keystroke-history-manual-install.zip`
2. Close OBS Studio
3. Extract contents to OBS installation directory (default: `C:\Program Files\obs-studio`)
4. Restart OBS Studio

---

## 🎯 Key Changes

### Text Alignment
**New Property:** "Text Alignment" dropdown with 3 options
- **Left** (Default) - Traditional left-aligned text
- **Center** - Horizontally centered text
- **Right** - Right-aligned text

**Implementation:**
- Added `text_alignment` field to `keystroke_source` struct
- Modified text renderer to use dynamic alignment flags
- Updated locale strings for UI labels
- Defaults to "left" for backward compatibility

### Bottom Anchoring Fix
**Problem:** When "Display Newest on Top" was unchecked, entries would calculate position from bottom but not truly anchor to the bottom edge.

**Solution:**
- Changed rendering to start at `height - padding - line_height` (actual bottom)
- Entries now render upward from bottom edge
- Newest entry always stays at bottom regardless of entry count

**Code Changes:**
```cpp
// Before (v1.1.0):
int start_y = height - padding - (line_height * count);
// Problem: With 2 entries and max=5, this places entries in middle

// After (v1.2.0):
int y_pos = height - padding - line_height;
// Solution: Always start at actual bottom edge
```

---

## 📝 Files Modified

### Source Code
- `src/keystroke-source.h` - Added `text_alignment` field
- `src/keystroke-source.cpp` - Added property, defaults, and setting loading
- `src/text-renderer.cpp` - Fixed rendering order and added alignment logic
- `data/locale/en-US.ini` - Added text alignment strings

### Build/Package
- `installer.iss` - Updated version to 1.2.0
- `build-installer.ps1` - Created installer build script

### Documentation
- `PLAN_v1.2.0.md` - Development plan
- `WHATS_NEW_v1.2.0.md` - User-facing feature summary
- `TESTING_v1.2.0.md` - Comprehensive testing guide
- `RELEASE_v1.2.0.md` - This file

---

## 🧪 Testing Status

### Completed Tests
- [x] Build succeeds on Windows
- [ ] Text alignment (Left/Center/Right)
- [ ] Newest at top (original behavior)
- [ ] Newest at bottom (fixed behavior)
- [ ] Bottom anchoring with various entry counts
- [ ] Settings persistence
- [ ] Default settings for new sources
- [ ] Windows installer
- [ ] Uninstaller

### Test Results
**See:** `TESTING_v1.2.0.md` for detailed test procedures and results.

---

## 🔧 Technical Details

### Alignment Implementation
- Uses Windows GDI `DT_LEFT`, `DT_CENTER`, `DT_RIGHT` flags
- Combined with `DT_VCENTER` and `DT_SINGLELINE`
- No performance impact (flag selection only)

### Rendering Algorithm (Newest at Bottom)
```cpp
// Start at actual bottom edge
int y_pos = height - padding - line_height;

// Render in reverse order (newest to oldest)
for (int i = (int)entries.size() - 1; i >= 0; i--) {
    DrawText(entry[i], y_pos);
    y_pos -= line_height;  // Move UP
}
```

### Backward Compatibility
- All v1.1.0 settings preserved
- New `text_alignment` defaults to "left" (previous behavior)
- Existing sources work without modification

---

## 📊 Statistics

### Code Changes
- Files Modified: 6
- Lines Added: ~80
- Lines Removed: ~15
- Net Change: +65 lines

### Build
- Compiler: MSVC 2022
- Build Configuration: Release
- Target: 64-bit Windows
- DLL Size: ~50 KB (unchanged)

### Installer
- Installer Size: ~200-300 KB (estimated)
- Compression: LZMA
- Requires: Administrator privileges
- Auto-detects: OBS installation path

---

## 🚀 Deployment

### Pre-Release Checklist
- [ ] All tests pass (see TESTING_v1.2.0.md)
- [ ] Documentation updated
- [ ] Build succeeds
- [ ] Manual install tested
- [ ] Installer built and tested
- [ ] No regressions from v1.1.0

### GitHub Release Steps
1. **Commit all changes**
   ```bash
   git add .
   git commit -m "Release v1.2.0: Text justification and bottom anchoring fix"
   ```

2. **Tag the release**
   ```bash
   git tag -a v1.2.0 -m "v1.2.0: Text justification and bottom anchoring"
   git push origin main
   git push origin v1.2.0
   ```

3. **Build release artifacts**
   ```powershell
   # Build plugin
   cmake --build build --config Release
   
   # Create manual install package
   .\create-package.ps1
   
   # Build installer (after installing Inno Setup)
   .\build-installer.ps1
   ```

4. **Create GitHub Release**
   - Go to: https://github.com/dneesen/obs-keystroke-history/releases/new
   - Tag: v1.2.0
   - Title: "v1.2.0 - Text Justification and Bottom Anchoring"
   - Description: Copy from WHATS_NEW_v1.2.0.md
   - Attach:
     - `obs-keystroke-history-installer-v1.2.0.exe`
     - `obs-keystroke-history-manual-install.zip`

---

## 📖 Documentation

### Updated Files
- README.md - Add v1.2.0 features to feature list
- WHATS_NEW_v1.2.0.md - User-facing release notes
- TESTING_v1.2.0.md - Testing procedures
- PLAN_v1.2.0.md - Development plan

### New User Guide Sections Needed
- How to use text alignment
- Explanation of display direction modes
- Troubleshooting alignment issues

---

## 🐛 Known Issues

None at this time.

---

## 🔮 Future Considerations

### Potential v1.3.0 Features
- Text shadow/outline for better visibility
- Custom key name mappings
- Animation effects (slide, fade)
- Multi-line text support
- Per-key color customization

### Technical Debt
- None identified

---

## 👥 Credits

**Developer:** David Neesen  
**Repository:** https://github.com/dneesen/obs-keystroke-history  
**License:** MIT

---

## 📞 Support

### Issues
Report bugs or request features: https://github.com/dneesen/obs-keystroke-history/issues

### Documentation
Full documentation: See README.md and documentation files in repository

---

**Release Prepared By:** David Neesen  
**Date:** October 8, 2025  
**Version:** 1.2.0  
**Status:** 🟡 Ready for Testing
