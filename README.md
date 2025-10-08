# OBS Keystroke History Plugin

![OBS Studio](https://img.shields.io/badge/OBS%20Studio-32.0.1-blue)
![Windows](https://img.shields.io/badge/Windows-10%2F11-0078D6)
![License](https://img.shields.io/badge/License-GPL%20v2-green)

A sophisticated OBS Studio plugin that displays keystroke and mouse action history as a real-time overlay, perfect for tutorials, gaming streams, and demonstrations.

## ✨ Features

### Core Functionality
- ⌨️ **Real-time keystroke capture** - Shows keys as they're pressed
- 🖱️ **Mouse action tracking** - Left, right, middle clicks, and scroll wheel
- 🎨 **Fully customizable appearance** - Font, size, colors, and background
- 📊 **Configurable history** - Display 5-20 recent entries
- ⏱️ **Auto-disappear** - Entries fade away after configurable duration

### Advanced Features
- 🔤 **Uppercase display** - All letter keys shown as capitals (A, B, C)
- 🔢 **Key repetition counting** - Shows "A x3" for repeated presses
- 📝 **Keystroke grouping** - Groups rapid typing into words (e.g., "HELLO")
- 🎯 **Window filtering** - Capture input only from specific windows
- 🕹️ **Modifier key formatting** - Clean display with spaces (Ctrl + G)
- 🔍 **Scroll wheel capture** - Shows "Scroll Up" / "Scroll Down"
- 🎨 **Flexible backgrounds** - Show/hide with adjustable opacity (0.0-1.0)

### Display Examples
```
Normal typing:     A, B, C, Enter
Repeated keys:     W x5, Space x2
Grouped typing:    HELLO + Enter
Modifier combos:   Ctrl + C, Alt + Tab
Mouse actions:     Left Click x3, Scroll Up
Gaming example:    W x4, Space, Left Click x2
```

## 🖼️ Use Cases

- **Tutorial Videos** - Show keyboard shortcuts and commands
- **Gaming Streams** - Display WASD movement and ability keys
- **Software Demos** - Demonstrate keyboard workflows
- **CAD/Design Work** - Show command sequences (e.g., "CO + Enter" in AutoCAD)
- **Accessibility** - Make keyboard input visible to viewers

## 📋 Requirements

- **OBS Studio** 32.0.1 or later
- **Windows** 10/11 (64-bit)
- **Visual Studio** 2022 (for building from source)
- **CMake** 3.16 or later

## 🚀 Quick Start

### Installation (Pre-built)
1. Download the latest release from [Releases](../../releases)
2. Run `install-plugin.ps1` as Administrator
3. Restart OBS Studio
4. Add "Keystroke History" source to your scene

### Building from Source
See [BUILD_GUIDE.md](BUILD_GUIDE.md) for detailed build instructions.

**Quick build:**
```powershell
cd build
cmake --build . --config Release
.\install-plugin.ps1  # Run as Administrator
```

## 🎮 Usage

### Basic Setup
1. **Add Source**: Sources → + → Keystroke History
2. **Configure Settings**:
   - Number of entries (5-20)
   - Font name, size, and color
   - Display duration (1-10 seconds)
   - Enable/disable mouse clicks
   - Background color and opacity

### Advanced Configuration

#### Keystroke Grouping
Perfect for command-driven software like AutoCAD:
- **Enable**: "Group Rapid Keystrokes"
- **Duration**: 0.1-2.0 seconds (time window to group keys)
- **Example**: Type "LINE" quickly → shows "LINE" instead of L, I, N, E

#### Window Filtering
Capture input only from specific applications:
- **Enable**: "Only Capture in Target Window"
- **Target Window**: Enter window title (e.g., "Chrome", "AutoCAD")
- **Matching**: Case-insensitive partial match

#### Display Options
- **Show Mouse Clicks**: Track all mouse buttons and scroll wheel
- **Show Background**: Toggle background rectangle
- **Background Opacity**: 0.0 (transparent) to 1.0 (opaque)

## 🎨 Customization Examples

### Gaming Overlay (Minimal)
```
Entries: 10
Duration: 2 seconds
Font: Arial, 24pt, White
Show Background: Off
Grouping: Off
Repetition: On (built-in)
```

### Tutorial Overlay (Professional)
```
Entries: 15
Duration: 5 seconds
Font: Segoe UI, 20pt, Black
Background: White, Opacity 0.8
Grouping: On (0.5s duration)
Window Filtering: Optional
```

### CAD Command Display
```
Entries: 8
Duration: 4 seconds
Font: Consolas, 18pt, Yellow
Background: Dark Gray, Opacity 0.7
Grouping: On (0.8s duration)
Uppercase: On (built-in)
```

## 🔧 Settings Reference

| Setting | Type | Range | Default | Description |
|---------|------|-------|---------|-------------|
| Visible Entries | Integer | 5-20 | 10 | Number of recent entries shown |
| Font Name | String | - | "Arial" | System font name |
| Font Size | Integer | 12-72 | 24 | Text size in points |
| Font Color | Color | - | White | Text color (RGB) |
| Background Color | Color | - | Black | Background rectangle color |
| Background Opacity | Float | 0.0-1.0 | 0.5 | Background transparency |
| Show Background | Boolean | - | true | Enable/disable background |
| Display Duration | Float | 1.0-10.0 | 3.0 | Seconds before entry disappears |
| Show Mouse Clicks | Boolean | - | true | Track mouse buttons and scroll |
| Group Keystrokes | Boolean | - | false | Group rapid typing into words |
| Grouping Duration | Float | 0.1-2.0 | 0.5 | Time window for grouping (seconds) |
| Capture Area Only | Boolean | - | false | Filter by window title |
| Target Window | String | - | "" | Window title to match (partial) |

## 📝 Feature Documentation

- [BUILD_GUIDE.md](BUILD_GUIDE.md) - Detailed build and setup instructions
- [DISPLAY_IMPROVEMENTS.md](DISPLAY_IMPROVEMENTS.md) - Uppercase, scroll wheel, repetition counting
- [TEXT_RENDERING_FIXES.md](TEXT_RENDERING_FIXES.md) - Black text fix, opacity slider, background rendering
- [ADVANCED_FEATURES.md](ADVANCED_FEATURES.md) - Keystroke grouping, window filtering, debug logging
- [CRASH_FIX_NOTES.md](CRASH_FIX_NOTES.md) - Mutex deadlock resolution

## 🐛 Troubleshooting

### Plugin doesn't load
- Ensure OBS is running as Administrator
- Check OBS log: `%appdata%\obs-studio\logs\<latest>.txt`
- Verify DLL location: `C:\Program Files\obs-studio\obs-plugins\64bit\keystroke-history.dll`

### No keystrokes appear
- Check if source is visible in scene
- Verify display duration isn't too short
- Look for [FILTER] logs if using window filtering
- Ensure OBS has input capture permissions

### Black text not visible
- Fixed in latest version! Update if using older build
- Check text color isn't same as background color
- Try enabling background with contrasting color

### Window filtering not working
- Enable "Only Capture in Target Window"
- Check OBS log for `[FILTER]` entries showing window matching
- Verify target window name matches actual window title
- Use partial matches (e.g., "chrome" matches "Google Chrome")

### Performance issues
- Reduce number of visible entries
- Disable mouse click tracking if not needed
- Increase display duration to reduce update frequency

## 🏗️ Development

### Project Structure
```
src/
├── plugin-main.cpp/h       # Plugin initialization and OBS integration
├── keystroke-source.cpp/h  # Main source logic, settings, tick function
├── input-capture.cpp/h     # Windows hooks for keyboard/mouse capture
└── text-renderer.cpp/h     # GDI-based text rendering to texture

data/
└── locale/
    └── en-US.ini          # UI strings and translations

CMakeLists.txt             # Build configuration
build-obs.ps1             # OBS source build script
install-plugin.ps1        # Plugin installation script
```

### Key Technologies
- **Input Capture**: Windows `SetWindowsHookEx` (WH_KEYBOARD_LL, WH_MOUSE_LL)
- **Text Rendering**: Windows GDI (CreateFont, DrawText, CreateDIBSection)
- **Threading**: std::mutex for thread-safe entry management
- **OBS API**: libobs for texture creation and source integration

### Building OBS from Source
If you need to build OBS itself (for development):
```powershell
.\build-obs.ps1
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

### Areas for Contribution
- 🔒 Password field filtering (skeleton exists, needs implementation)
- 🌍 Translations (add locale files in `data/locale/`)
- 🎨 UI improvements
- 📊 Additional display modes
- 🐧 Linux/macOS support (requires platform-specific input capture)

## 📜 License

This project is licensed under the **GNU General Public License v2.0** - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- OBS Studio team for the excellent plugin API
- Windows GDI for stable text rendering
- Community feedback and testing

## 📮 Support

- **Issues**: [GitHub Issues](../../issues)
- **Documentation**: See markdown files in repository
- **OBS Forums**: [OBS Studio Forums](https://obsproject.com/forum/)

---

**Built with ❤️ for the streaming and content creation community**
