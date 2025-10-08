# What's New in v1.2.0

## New Features

### 1. Text Justification (Left/Center/Right)
You can now choose how your keystrokes are aligned within the source!

**Location:** Source Properties → Text Alignment

**Options:**
- **Left** (Default) - Text aligns to the left edge
- **Center** - Text is centered horizontally
- **Right** - Text aligns to the right edge

**Use Cases:**
- Left: Traditional, easy to read for lists
- Center: Great for titles or overlay displays
- Right: Useful for right-side screen overlays

### 2. Fixed Text Alignment (Newest at Bottom)
The "Display Newest Entries at Top" checkbox now works correctly when unchecked!

**What Changed:**
When unchecked (newest at bottom mode):
- ✅ The latest keystroke now **always** appears at the actual bottom of the display
- ✅ New keystrokes appear **above** the previous newest entry
- ✅ Entries properly anchor to the bottom edge regardless of count

**Previously:** Entries would float in the middle of the display area when you had fewer entries than the maximum.

**Now:** The newest entry is always anchored to the bottom, making it much easier to position your source!

### 3. Windows Installer (.exe)
You can now install the plugin using a convenient Windows installer!

**Benefits:**
- Automatically detects OBS installation
- Installs to the correct directory
- Creates proper uninstaller
- No manual file copying needed

**Still Available:** Manual installation ZIP for advanced users or non-standard OBS locations.

---

## How to Use the New Features

### Text Alignment
1. Add or select a "Keystroke History" source
2. In the source properties, find "Text Alignment"
3. Choose from Left, Center, or Right
4. The text updates immediately!

### Newest at Bottom (Fixed)
1. Add or select a "Keystroke History" source
2. **Uncheck** "Display Newest Entries at Top"
3. Position your source where you want the bottom edge
4. As you type, new entries appear above the newest (at the bottom)
5. Perfect for bottom-of-screen overlays!

---

## Installation Options

### Option 1: Windows Installer (Recommended)
1. Download `obs-keystroke-history-installer-v1.2.0.exe`
2. Close OBS Studio
3. Run the installer
4. Restart OBS Studio
5. Add "Keystroke History" source from your sources list

### Option 2: Manual Installation
1. Download `obs-keystroke-history-manual-install.zip`
2. Close OBS Studio
3. Extract to your OBS installation directory
4. Restart OBS Studio
5. Add "Keystroke History" source from your sources list

---

## Technical Details

### Text Alignment Implementation
- Uses Windows GDI `DrawText` flags: `DT_LEFT`, `DT_CENTER`, `DT_RIGHT`
- Works with both "newest at top" and "newest at bottom" modes
- No performance impact

### Rendering Fix
- Fixed rendering order for "newest at bottom" mode
- Now renders from actual bottom edge upward
- Entries always anchor correctly regardless of count

### Installer Details
- Built with Inno Setup
- Automatically detects OBS installation path
- Verifies OBS Studio is installed before proceeding
- Creates proper Windows uninstaller
- ~200-300 KB installer size

---

## Compatibility

- **OBS Studio:** 32.0.1+ (Windows)
- **Windows:** 10/11 (64-bit)
- **Backward Compatible:** All v1.1.0 settings preserved

---

## Upgrading from v1.1.0

Your existing settings will be preserved! The new "Text Alignment" property will default to "Left" (the previous behavior).

**Using the Installer:**
1. Close OBS Studio
2. Run the v1.2.0 installer
3. Restart OBS Studio
4. All your sources and settings remain intact!

**Manual Installation:**
1. Close OBS Studio
2. Replace the plugin files as before
3. Restart OBS Studio

---

## Known Issues

None at this time! 🎉

---

## Coming Soon (Maybe!)

Have ideas for future features? Open an issue on GitHub!

Possible future additions:
- Text shadow/outline options
- Animation effects
- Custom key display names
- Multi-line text support
- ... your idea here!

---

**Enjoy v1.2.0!** 🚀
