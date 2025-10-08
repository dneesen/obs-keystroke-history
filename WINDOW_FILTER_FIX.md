# Window Filtering UI Fix - Text Input vs Dropdown

## Problem Identified

The window filtering feature had a **UI/UX mismatch** that made it unusable:

### What Was Wrong:
1. **UI showed dropdown** with options like:
   - "All Windows"
   - "Window Capture" (OBS source name)
   - "Display Capture" (OBS source name)
   - "Game Capture" (OBS source name)

2. **Backend expected window titles** like:
   - "Notepad"
   - "Google Chrome"
   - "AutoCAD 2024"
   - "Visual Studio Code"

3. **Result**: Selecting anything from dropdown didn't work because:
   - Dropdown provided OBS source names: "Window Capture"
   - Filter expected actual window titles: "Notepad"
   - Mismatch = No filtering happened

### Why This Happened:
The original implementation tried to be "helpful" by listing OBS capture sources in a dropdown, but the filtering code operates at the **Windows OS level** (using `GetForegroundWindow()` and `GetWindowTextA()`), which returns actual window titles, not OBS source names.

---

## Solution: Text Input Field

Changed from a **dropdown** to a simple **text input field** where users type the window title they want to filter.

### Before:
```cpp
// Dropdown with OBS source names
obs_property_t* source_list = obs_properties_add_list(props, "target_window",
    obs_module_text("TargetWindow"), OBS_COMBO_TYPE_EDITABLE, OBS_COMBO_FORMAT_STRING);

obs_property_list_add_string(source_list, "All Windows", "");

obs_enum_sources([](void* param, obs_source_t* source) {
    // Added "Window Capture", "Display Capture", etc.
    obs_property_list_add_string(list, name, name);
    return true;
}, source_list);
```

### After:
```cpp
// Simple text input field
obs_property_t* target_window = obs_properties_add_text(props, "target_window",
    obs_module_text("TargetWindow"), OBS_TEXT_DEFAULT);

obs_property_set_long_description(target_window,
    "Enter part of the window title to filter (e.g., 'notepad', 'chrome', 'autocad'). "
    "Leave empty to capture from all windows. Case-insensitive partial match.");
```

---

## How to Use (After Update)

### Capture from ALL windows:
1. ✅ **Check** "Only Capture in Target Window"
2. ✅ **Leave text field EMPTY**
3. Result: Captures from any window

### Capture from specific window:
1. ✅ **Check** "Only Capture in Target Window"
2. ✅ **Type window title** (e.g., "notepad", "chrome", "autocad")
3. Result: Only captures when that window is active

### Disable filtering:
1. ✅ **Uncheck** "Only Capture in Target Window"
2. Result: Captures from all windows (checkbox state ignored)

---

## Finding Window Titles

To find the exact window title to use:

### Method 1: Check the logs
1. Enable "Only Capture in Target Window"
2. Leave target field empty
3. Switch between windows and type
4. Check OBS log for entries like:
   ```
   [FILTER] Window: 'Untitled - Notepad' | Target: '' | Match: YES
   [FILTER] Window: 'Google Chrome' | Target: '' | Match: YES
   ```
5. Use any part of the window title shown

### Method 2: Windows Task Manager
1. Open Task Manager (Ctrl+Shift+Esc)
2. Look at application names in "Apps" section
3. That's usually the window title

### Method 3: Title Bar
- Look at the window's title bar
- The text shown is usually the window title
- Examples:
  - `Notepad` → Window title: "Notepad" or "Untitled - Notepad"
  - `Chrome with Google open` → Window title: "Google - Google Chrome"
  - `AutoCAD` → Window title: "AutoCAD 2024"

---

## Partial Matching Examples

The filter uses **case-insensitive partial matching**, so you can be flexible:

| You Type | Matches | Doesn't Match |
|----------|---------|---------------|
| `notepad` | "Notepad", "Untitled - Notepad", "NOTEPAD" | "Note", "Pad" alone |
| `chrome` | "Google Chrome", "Chrome", "chrome.exe" | "Firefox", "Edge" |
| `cad` | "AutoCAD 2024", "AutoCAD", "BricsCAD" | "Notepad" |
| `visual` | "Visual Studio", "Visual Studio Code" | "Studio" alone |
| (empty) | **All windows** | None |

**Pro tip**: Use short, unique parts of the title:
- ✅ "autocad" is better than "AutoCAD 2024 - Drawing1.dwg"
- ✅ "chrome" is better than "Google - Google Chrome"
- ✅ "code" is better than "Visual Studio Code"

---

## Configuration Logging

Added new logging to show configuration when source is loaded:

### When filtering is DISABLED:
```
[CONFIG] Window filtering DISABLED - Capturing from all windows
```

### When filtering is ENABLED with NO target:
```
[CONFIG] Window filtering ENABLED - Capturing from ALL windows (no target specified)
```

### When filtering is ENABLED with target:
```
[CONFIG] Window filtering ENABLED - Target: 'notepad'
```

This appears **once** when you:
- Add the source
- Change settings
- Reload OBS

---

## Runtime Logging

When filtering is active, logs appear **only when you switch windows**:

```
[FILTER] Window: 'Untitled - Notepad' | Target: 'notepad' | Match: YES
... (type 100 keys - no logs)
[FILTER] Window: 'Google Chrome' | Target: 'notepad' | Match: NO
... (type 100 keys - no logs)
[FILTER] Window: 'Notepad' | Target: 'notepad' | Match: YES
```

---

## Testing the Fix

### Test 1: All Windows (Empty Target)
1. **Check** "Only Capture in Target Window"
2. **Leave target field EMPTY**
3. Type in any window
4. **Expected**: All keystrokes captured
5. **Log shows**: `[CONFIG] Window filtering ENABLED - Capturing from ALL windows`

### Test 2: Specific Window
1. **Check** "Only Capture in Target Window"
2. **Type**: `notepad`
3. Open Notepad and type
4. **Expected**: Keystrokes captured in Notepad
5. Switch to Chrome and type
6. **Expected**: No keystrokes captured in Chrome
7. **Log shows**: 
   ```
   [CONFIG] Window filtering ENABLED - Target: 'notepad'
   [FILTER] Window: 'Notepad' | Target: 'notepad' | Match: YES
   [FILTER] Window: 'Google Chrome' | Target: 'notepad' | Match: NO
   ```

### Test 3: Filtering Disabled
1. **Uncheck** "Only Capture in Target Window"
2. Target field content doesn't matter
3. Type in any window
4. **Expected**: All keystrokes captured
5. **Log shows**: `[CONFIG] Window filtering DISABLED - Capturing from all windows`

---

## Why This Fix Is Better

### Old Implementation (Dropdown):
❌ Listed OBS sources (wrong data)  
❌ Confusing UI (sources vs windows)  
❌ Didn't actually work  
❌ "All Windows" option was misleading  
❌ Users couldn't enter custom window titles  

### New Implementation (Text Input):
✅ Users type actual window titles  
✅ Clear instructions in tooltip  
✅ Partial matching (flexible)  
✅ Case-insensitive (user-friendly)  
✅ Empty = all windows (intuitive)  
✅ Works as expected  
✅ Logs show exactly what's happening  

---

## Migration Notes

### If you had the old version:
- Any dropdown selections were **ignored** (didn't work anyway)
- After update, just type the window title you want
- Check logs to see actual window titles

### Settings preserved:
- "Only Capture in Target Window" checkbox state
- Target field content (was editable combo, now text field)
- All other settings unchanged

---

## Implementation Details

### Files Modified:
- `src/keystroke-source.cpp`:
  - Changed `obs_properties_add_list()` to `obs_properties_add_text()`
  - Removed `obs_enum_sources()` call
  - Added configuration logging
  - Added tooltip with instructions

### Behavior Changes:
- **UI**: Dropdown → Text input field
- **Functionality**: Now actually works
- **Logging**: Shows configuration on load
- **No breaking changes**: Same property name, same data type

### Backward Compatibility:
- ✅ Existing configurations still load
- ✅ Empty target still works
- ✅ Manual text entry works better
- ⚠️ Old dropdown selections (if any) are now just text

---

## Summary

**Problem**: Dropdown listed OBS sources, but filtering expects window titles → **mismatch → broken**

**Solution**: Text input field for actual window titles → **direct input → works**

**Result**: Window filtering now works correctly with clear instructions and helpful logging!

---

**Install the update and test it out!** 🚀

```powershell
.\install-plugin.ps1
```

After restarting OBS, you'll see:
1. **Text input field** instead of dropdown
2. **Clear instructions** in the UI
3. **Helpful logs** showing configuration
4. **Working window filtering** based on actual window titles
