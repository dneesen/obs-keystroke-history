# Release Notes: v1.2.1

**Release Date:** October 13, 2025  
**Type:** Bug Fix Release  
**Status:** Stable

---

## 🐛 Bug Fixes

### Unicode Arrow Keys Display
**Issue:** Arrow keys were not displaying correctly as Unicode symbols (←, ↑, →, ↓)

**Root Cause:** Text renderer was using ANSI Windows API functions (`CreateFontA`, `DrawTextA`) which don't support Unicode characters.

**Solution:** 
- Updated text renderer to use Unicode Windows API functions (`CreateFontW`, `DrawTextW`)
- Added proper UTF-8 to wide string conversion for all text rendering
- Arrow keys now display as proper Unicode arrow symbols

**Impact:** All arrow keys now render correctly with clear, intuitive symbols instead of garbage characters or boxes.

**Technical Details:**
- Modified `src/text-renderer.cpp` to use wide-character APIs
- Implemented `MultiByteToWideChar` conversion from UTF-8 to UTF-16
- Added `#include <string>` for `std::wstring` support
- No breaking changes to existing functionality

---

## 📥 Installation

### Method 1: Windows Installer (Recommended)
1. Download `obs-keystroke-history-installer-v1.2.1.exe`
2. Close OBS Studio
3. Run installer (requires administrator privileges)
4. Restart OBS Studio

### Method 2: Manual Installation
1. Download `obs-keystroke-history-v1.2.1-manual-install.zip`
2. Close OBS Studio
3. Extract contents to OBS installation directory (default: `C:\Program Files\obs-studio`)
4. Restart OBS Studio

---

## 🔄 Upgrading from v1.2.0
This is a drop-in replacement. Simply install over your existing version. No configuration changes needed.

---

## ✨ What's Included

### All Features from v1.2.0
- **Text Alignment** - Choose between Left, Center, or Right alignment
- **Fixed Bottom Anchoring** - Newest-at-bottom mode properly anchors to bottom edge
- **Windows Installer** - Convenient .exe installer

### New in v1.2.1
- **Unicode Arrow Keys** - Proper display of ←, ↑, →, ↓ symbols
- **Full Unicode Support** - Foundation for international characters and future emoji support
- **Improved Font Compatibility** - Better rendering across different font types

---

## 🎯 Keyboard Keys Reference

The following keys are displayed with special symbols:

| Key | Display | Key | Display |
|-----|---------|-----|---------|
| Left Arrow | ← | Right Arrow | → |
| Up Arrow | ↑ | Down Arrow | ↓ |
| Backspace | Backspace | Delete | Del |
| Enter | Enter | Tab | Tab |
| Escape | Esc | Space | Space |
| Page Up | PgUp | Page Down | PgDn |
| Home | Home | End | End |
| Insert | Ins | | |

---

## 🧪 Testing

This release has been tested with:
- **OBS Studio:** 30.x
- **Windows:** 10 and 11
- **Fonts:** Arial, Consolas, Segoe UI, Courier New
- **Display modes:** Newest-on-top and Newest-on-bottom
- **Text alignments:** Left, Center, Right

---

## 🐞 Known Issues

None currently reported.

---

## 📝 Changelog

### v1.2.1 (2025-10-13)
- Fixed Unicode arrow keys display
- Added UTF-8 to wide string conversion
- Updated to Unicode Windows API functions
- Full Unicode character support

### v1.2.0 (2025-10-08)
- Added text alignment options (Left/Center/Right)
- Fixed bottom anchoring for newest-at-bottom mode
- Added Windows installer

### v1.0.0 (Initial Release)
- Basic keystroke capture and display
- Mouse click tracking
- Fade animations
- Window/source targeting

---

## 💬 Feedback & Support

- **GitHub Issues:** https://github.com/dneesen/obs-keystroke-history/issues
- **Discussions:** https://github.com/dneesen/obs-keystroke-history/discussions

---

## 📄 License

MIT License - See LICENSE file for details

---

**Thank you for using OBS Keystroke History!**
