# Advanced Features Implementation

## Features Added

### 1. Keystroke Grouping ✅

**Purpose:** Group rapid keystrokes into words, perfect for command-driven applications like AutoCAD

**Example Use Cases:**
```
AutoCAD:
  Type: C O Enter
  Shows: CO + Enter

Text Input:
  Type: H E L L O
  Shows: HELLO
  
Commands:
  Type: C T R L + S
  Shows: CTRL + S (if modifiers used)
```

**Settings:**
- **"Group Rapid Keystrokes Together"** (checkbox)
  - Enables/disables grouping feature
  - Default: Disabled (to maintain backwards compatibility)

- **"Grouping Duration"** (0.1 - 2.0 seconds slider)
  - Maximum time between keystrokes to group them
  - Default: 0.5 seconds
  - Lower values = must type faster
  - Higher values = more forgiving timing

**How It Works:**

1. **Letter/Number Detection:**
   - Only groups single-character alphanumeric keys (a-z, A-Z, 0-9)
   - Special keys (Enter, Space, Tab, etc.) are NOT grouped

2. **Grouping Logic:**
   ```
   User types: C (time = 0s)
   User types: O (time = 0.3s) → Elapsed: 0.3s < 0.5s → Group "CO"
   User types: Enter (time = 0.6s) → Special key → Result: "CO + Enter"
   ```

3. **Group Completion:**
   - Special keys (Enter, Space, Tab, etc.) end the group
   - Display format: `<grouped_letters> + <special_key>`
   - Example: `LINE + Enter`, `CO + Tab`, `MOVE + Space`

4. **Timeout Behavior:**
   - If time between keystrokes > grouping duration, start new group
   - Each group is a separate entry in the history

**Implementation Details:**

```cpp
// Track grouping state
std::chrono::steady_clock::time_point last_keystroke_time;
std::string current_group;

// Grouping logic
if (elapsed < context->group_duration && is_groupable) {
    // Append to current group
    context->current_group += keystroke;
} else if (is_special_key && !current_group.empty()) {
    // End group with special key
    entry.text = current_group + " + " + keystroke;
}
```

**Benefits:**
- Cleaner display for typing-heavy workflows
- Easier to see actual commands vs individual letters
- Reduces clutter in keystroke history
- Perfect for CAD, terminal, and command-line applications

---

### 2. Background Opacity 0-1 Scale ✅

**Problem:** 0-255 scale wasn't intuitive for opacity

**Solution:** Changed to 0.0 - 1.0 floating point scale

**Changes:**
- Property type: `obs_properties_add_float_slider()`
- Range: 0.0 (fully transparent) to 1.0 (fully opaque)
- Step: 0.01 (1% increments)
- Default: 0.5 (50% opacity)

**UI Display:**
```
Background Opacity: [slider] 0.0 ←────●────→ 1.0
                                    (0.5)
Description: "Background Opacity (0 = transparent, 1 = opaque)"
```

**Internal Conversion:**
```cpp
// Settings stores as float (0.0 - 1.0)
float background_opacity;

// Convert to alpha channel (0 - 255)
int bg_alpha = context->show_background ? 
    (int)(context->background_opacity * 255.0f) : 0;
```

**Benefits:**
- More intuitive (matches standard opacity controls)
- Finer control with 1% increments
- Industry-standard scale
- Easier to remember common values (0.25, 0.5, 0.75, 1.0)

---

### 3. Target Window Debug Logging ✅

**Problem:** Window filtering not working, no way to debug why

**Solution:** Added comprehensive debug logging

**Log Messages:**

```
[FILTER] No foreground window
  → Windows API couldn't get active window

[FILTER] No target specified, capturing all
  → Target window field is empty, capturing everything

[FILTER] Current window: 'Google Chrome', Target: 'chrome'
  → Shows active window title and target filter

[FILTER] Match result: YES
  → Window matches filter, capturing input

[FILTER] Match result: NO
  → Window doesn't match, ignoring input
```

**How to Use for Debugging:**

1. Enable "Only Capture in Target Window"
2. Set a target (e.g., "Chrome")
3. Switch to target window
4. Press a key
5. Check OBS log file for [FILTER] messages

**Log File Location:**
```
C:\Users\<username>\AppData\Roaming\obs-studio\logs\<latest>.txt
```

**Example Log Session:**
```
[FILTER] Current window: 'Google Chrome - New Tab', Target: 'chrome'
[FILTER] Match result: YES
[INPUT] Key pressed: VK=71
[INPUT] Adding keystroke: 'G'
[ENTRIES] Added keystroke: 'G'
```

**Debugging Workflow:**

1. **Not capturing anything:**
   - Look for: `[FILTER] Match result: NO`
   - Check: Is window title what you expect?
   - Solution: Adjust target string to match actual title

2. **Capturing in wrong window:**
   - Look for: `[FILTER] Current window: '<unexpected>'`
   - Check: Is foreground window detection working?
   - Solution: Make target filter more specific

3. **Filter not active:**
   - Look for: `[FILTER] No target specified`
   - Check: Is "Only Capture in Target Window" enabled?
   - Check: Did target string save properly?

---

## Configuration Examples

### Example 1: AutoCAD Workflow
```
✓ Group Rapid Keystrokes Together
Grouping Duration: 0.5 seconds
✓ Only Capture in Target Window
Target Window: AutoCAD

Result: 
  Type "CO" + Enter → "CO + Enter"
  Type "LINE" + Enter → "LINE + Enter"
  Only captures when AutoCAD is active
```

### Example 2: Gaming (No Grouping)
```
☐ Group Rapid Keystrokes Together
✓ Only Capture in Target Window
Target Window: game

Result:
  W A S D shown individually
  Only captures in game window
```

### Example 3: Typing Demo (With Grouping)
```
✓ Group Rapid Keystrokes Together
Grouping Duration: 0.8 seconds
☐ Only Capture in Target Window

Result:
  Type "HELLO" → Shows "HELLO"
  Captures in all windows
  0.8s allows slower typing
```

---

## Testing Checklist

### Keystroke Grouping:
- [ ] Enable grouping checkbox
- [ ] Type rapid letters (e.g., "TEST")
- [ ] Verify letters group into single entry
- [ ] Press Enter after letters
- [ ] Verify format: "TEST + Enter"
- [ ] Adjust grouping duration slider
- [ ] Verify faster/slower grouping with duration change

### Background Opacity:
- [ ] Set opacity to 0.0 → Background invisible
- [ ] Set opacity to 0.5 → Background semi-transparent
- [ ] Set opacity to 1.0 → Background fully opaque
- [ ] Uncheck "Show Background" → No background regardless of opacity

### Window Filtering Debug:
- [ ] Enable "Only Capture in Target Window"
- [ ] Set target to "notepad"
- [ ] Open Notepad
- [ ] Type something → Should capture
- [ ] Switch to another app
- [ ] Type something → Should NOT capture
- [ ] Check log for [FILTER] messages
- [ ] Verify current window title shown correctly
- [ ] Verify match result correct (YES in Notepad, NO elsewhere)

---

## Known Limitations

### Keystroke Grouping:
- Only groups alphanumeric characters (a-z, 0-9)
- Modifier combinations (Ctrl+G) don't group with letters
- Special characters (!, @, #) don't group
- Mouse clicks end groups

### Window Filtering:
- Uses window title, not process name
- Partial matching may match unintended windows
- Case-insensitive (can't distinguish "Chrome" from "chrome")

### Opacity:
- Only affects background, not text
- Text always rendered at 100% opacity

---

## Future Enhancements

Potential improvements based on this foundation:

1. **Smart Grouping:**
   - Group based on word boundaries (spaces)
   - Special character handling
   - Auto-detect command patterns

2. **Advanced Filtering:**
   - Process name filtering
   - Regex pattern matching
   - Multiple target windows (OR logic)
   - Blacklist mode (capture EXCEPT in X)

3. **Opacity Per Element:**
   - Separate text opacity
   - Fade per-entry opacity
   - Gradient effects

---

**All requested features implemented!** ✅
