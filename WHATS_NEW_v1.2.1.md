# What's New in v1.2.1

## 🐛 Bug Fix: Unicode Arrow Keys

### The Problem
In previous versions, arrow keys were supposed to display as Unicode symbols (←, ↑, →, ↓) but appeared as garbage characters, boxes, or question marks instead.

### The Fix
The text renderer now properly supports Unicode characters!

**What Changed:**
- Upgraded from ANSI text rendering to Unicode rendering
- Added UTF-8 to wide string conversion
- Full Unicode support for all keys and characters

### What You'll See Now
When you press arrow keys, you'll see:
- **Left Arrow:** ← (clear leftward arrow)
- **Up Arrow:** ↑ (clear upward arrow)
- **Right Arrow:** → (clear rightward arrow)
- **Down Arrow:** ↓ (clear downward arrow)

Beautiful, intuitive symbols that are easy for viewers to understand!

### Side Benefits
This Unicode fix also enables:
- ✅ Support for international characters (é, ñ, 中, etc.)
- ✅ Potential for emoji support in future versions
- ✅ Better font compatibility across different typefaces
- ✅ More consistent rendering across different systems

---

## 📸 Before & After

### Before v1.2.1
```
Left Arrow:  ? or □ or garbage
Up Arrow:    ? or □ or garbage
Right Arrow: ? or □ or garbage
Down Arrow:  ? or □ or garbage
```

### After v1.2.1
```
Left Arrow:  ←
Up Arrow:    ↑
Right Arrow: →
Down Arrow:  ↓
```

---

## 🎮 Perfect for Gaming Streams

Arrow keys are commonly used in many games:
- **Fighting games:** ← → ↓ ↑ for moves
- **Platformers:** ← → for movement
- **Rhythm games:** ↑ ↓ ← → for notes
- **Menu navigation:** Arrow keys for selection

Now your viewers can clearly see which arrow keys you're pressing!

---

## 💡 How to Use

1. **Update your plugin** to v1.2.1
2. **Restart OBS Studio**
3. **Press arrow keys** - they'll automatically display correctly
4. **No configuration needed** - it just works!

---

## 🌟 All Features Included

### From v1.2.0
- **Text Alignment** - Choose Left, Center, or Right alignment
- **Fixed Bottom Anchoring** - Newest-at-bottom works correctly
- **Windows Installer** - Easy installation

### From v1.0.0
- **Keystroke Capture** - Capture all keyboard input
- **Mouse Clicks** - Show left/right/middle clicks
- **Fade Animations** - Smooth entry/exit effects
- **Window Targeting** - Capture from specific windows
- **Source Filtering** - Match OBS capture sources
- **Customizable Fonts** - Choose font, size, color
- **Background Options** - Solid background or transparent
- **Entry Limits** - Control how many keys show

---

## 🔧 Technical Details

For developers and curious users:

### What Was Changed
**File:** `src/text-renderer.cpp`

**Changes:**
1. Added `#include <string>` for `std::wstring` support
2. Changed `CreateFontA` → `CreateFontW` (Unicode font creation)
3. Changed `DrawTextA` → `DrawTextW` (Unicode text rendering)
4. Added `MultiByteToWideChar` conversion for UTF-8 → UTF-16

### Why This Matters
Windows has two sets of text APIs:
- **ANSI (A suffix):** Only supports basic ASCII characters
- **Unicode (W suffix):** Supports all international characters

The plugin now uses Unicode APIs throughout, ensuring proper display of all special characters.

---

## 🔄 Upgrading

### From v1.2.0
This is a simple bug fix update. Just install over your existing version:
1. Close OBS
2. Run the installer or extract the ZIP
3. Restart OBS

### From Earlier Versions
If you're upgrading from v1.1.x or earlier, you'll get:
- Unicode arrow keys (new!)
- Text alignment options (v1.2.0)
- Fixed bottom anchoring (v1.2.0)
- Windows installer (v1.2.0)

---

## 🎯 Use Cases

### Tutorials
Show exact key presses for:
- Keyboard shortcuts (Ctrl + ← to move left)
- Text editing (↑ ↓ to navigate lines)
- Application navigation

### Gaming
Display inputs for:
- Fighting game combos
- Platformer movement
- Menu selections
- Speedrun inputs

### Demonstrations
Demonstrate:
- Software workflows
- Code editing with arrow keys
- Spreadsheet navigation
- Terminal commands

---

## 🙏 Thank You!

Thank you for reporting the arrow key display issue. Your feedback helps make this plugin better for everyone!

If you find any other issues or have feature requests, please:
- Open an issue on GitHub
- Start a discussion
- Share your experience

---

## 📦 Download

Get v1.2.1:
- **Installer:** `obs-keystroke-history-installer-v1.2.1.exe`
- **Manual ZIP:** `obs-keystroke-history-v1.2.1-manual-install.zip`

---

**Enjoy the update!** 🎉
