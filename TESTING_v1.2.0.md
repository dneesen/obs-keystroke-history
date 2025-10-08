# Testing Guide for v1.2.0

## Pre-Testing Setup

### 1. Build the Plugin
```powershell
cmake --build build --config Release
```

### 2. Install to OBS (as Administrator)
```powershell
.\install-plugin.ps1
```

### 3. Restart OBS Studio
Close and reopen OBS to load the new version.

---

## Test Suite

### Test 1: Text Alignment - Left
**Purpose:** Verify left alignment works correctly

1. Add a new "Keystroke History" source
2. Set "Text Alignment" to **Left**
3. Type some keystrokes: `Hello World`
4. **Expected:** Text starts at the left edge of the source
5. **Verify:** No text is cut off on the left side

**Status:** ☐ Pass ☐ Fail

---

### Test 2: Text Alignment - Center
**Purpose:** Verify center alignment works correctly

1. Select your "Keystroke History" source
2. Set "Text Alignment" to **Center**
3. Type some keystrokes: `Centered Text`
4. **Expected:** Text is horizontally centered in the source
5. **Verify:** Equal space on left and right sides

**Status:** ☐ Pass ☐ Fail

---

### Test 3: Text Alignment - Right
**Purpose:** Verify right alignment works correctly

1. Select your "Keystroke History" source
2. Set "Text Alignment" to **Right**
3. Type some keystrokes: `Right Side`
4. **Expected:** Text aligns to the right edge of the source
5. **Verify:** No text is cut off on the right side

**Status:** ☐ Pass ☐ Fail

---

### Test 4: Newest at Top (Original Behavior)
**Purpose:** Ensure existing mode still works

1. Select your "Keystroke History" source
2. **Check** "Display Newest Entries at Top"
3. Clear entries (restart OBS or wait for fade)
4. Type: `First` (wait) `Second` (wait) `Third`
5. **Expected Order (top to bottom):**
   - Third
   - Second
   - First

**Status:** ☐ Pass ☐ Fail

---

### Test 5: Newest at Bottom - Basic Function
**Purpose:** Verify the fixed rendering anchors to bottom

1. Select your "Keystroke History" source
2. **Uncheck** "Display Newest Entries at Top"
3. Set "Maximum Entries" to **5**
4. Clear entries (restart OBS or wait for fade)
5. Type: `First` (wait) `Second` (wait) `Third`
6. **Expected Order (top to bottom):**
   - First
   - Second
   - Third (← This should be at the BOTTOM of the source)

**Status:** ☐ Pass ☐ Fail

---

### Test 6: Newest at Bottom - Anchoring
**Purpose:** Critical test - verify bottom anchoring with fewer entries

1. Ensure "Display Newest Entries at Top" is **unchecked**
2. Set "Maximum Entries" to **5**
3. Position source at bottom of canvas
4. Clear entries
5. Type exactly **2 entries**: `First` (wait) `Second`
6. **Expected:**
   - "Second" should be at the absolute BOTTOM of the source
   - "First" should be directly above "Second"
   - NO gap between "Second" and bottom edge
7. Type a 3rd entry: `Third`
8. **Expected:**
   - "Third" is now at the bottom
   - "Second" moved up one line
   - "First" moved up one line
   - Still no gap at bottom

**Status:** ☐ Pass ☐ Fail

---

### Test 7: Text Alignment + Newest at Top
**Purpose:** Verify alignment works with both display modes

1. **Check** "Display Newest Entries at Top"
2. Test each alignment:
   - Left: ☐ Text aligns left
   - Center: ☐ Text is centered
   - Right: ☐ Text aligns right
3. Type keystrokes between each change
4. **Expected:** Alignment updates immediately, newest stays at top

**Status:** ☐ Pass ☐ Fail

---

### Test 8: Text Alignment + Newest at Bottom
**Purpose:** Verify alignment works with newest-at-bottom mode

1. **Uncheck** "Display Newest Entries at Top"
2. Test each alignment:
   - Left: ☐ Text aligns left
   - Center: ☐ Text is centered
   - Right: ☐ Text aligns right
3. Type keystrokes between each change
4. **Expected:** Alignment updates immediately, newest stays at bottom

**Status:** ☐ Pass ☐ Fail

---

### Test 9: Maximum Entries + Newest at Bottom
**Purpose:** Verify trimming works correctly with bottom anchoring

1. **Uncheck** "Display Newest Entries at Top"
2. Set "Maximum Entries" to **3**
3. Type 5 entries: `1` (wait) `2` (wait) `3` (wait) `4` (wait) `5`
4. **Expected Visible Entries (top to bottom):**
   - 3
   - 4
   - 5 (← At bottom)
5. **Expected:** Entries 1 and 2 are removed (oldest first)
6. **Expected:** Entry 5 (newest) stays anchored at bottom

**Status:** ☐ Pass ☐ Fail

---

### Test 10: Fade Effect + Newest at Bottom
**Purpose:** Ensure fade doesn't affect anchoring

1. **Uncheck** "Display Newest Entries at Top"
2. Set "Fade Duration" to **2.0 seconds**
3. Type 3 entries rapidly: `A` `B` `C`
4. Watch as entries fade
5. **Expected:**
   - Newest entry (C) stays at bottom while fading
   - Older entries fade from above
   - No jumping or repositioning during fade

**Status:** ☐ Pass ☐ Fail

---

### Test 11: Settings Persistence
**Purpose:** Verify settings are saved/loaded correctly

1. Set these values:
   - Text Alignment: **Center**
   - Display Newest on Top: **Unchecked**
   - Font Size: **32**
2. Close OBS Studio completely
3. Reopen OBS Studio
4. Open source properties
5. **Expected:** All settings retained correctly

**Status:** ☐ Pass ☐ Fail

---

### Test 12: Default Settings
**Purpose:** Verify new sources have correct defaults

1. Add a brand new "Keystroke History" source
2. Check properties without changing anything
3. **Expected Defaults:**
   - Text Alignment: **Left**
   - Display Newest on Top: **Unchecked** (newest at bottom)
   - Font: **Arial**
   - Font Size: **24**

**Status:** ☐ Pass ☐ Fail

---

### Test 13: Windows Installer (If Built)
**Purpose:** Test the .exe installer

1. Completely uninstall the plugin manually
2. Close OBS Studio
3. Run `obs-keystroke-history-installer-v1.2.0.exe`
4. **Expected:** 
   - Installer detects OBS installation ☐
   - Installer completes without errors ☐
   - Restart OBS Studio ☐
   - Plugin appears in sources list ☐
   - Add source and verify it works ☐

**Status:** ☐ Pass ☐ Fail ☐ Not Tested

---

### Test 14: Uninstaller (If Installer Used)
**Purpose:** Test the uninstaller

1. Close OBS Studio
2. Go to Windows Settings → Apps → Installed apps
3. Find "OBS Keystroke History Plugin"
4. Click Uninstall
5. **Expected:**
   - Uninstaller asks for confirmation ☐
   - Uninstaller completes without errors ☐
   - Restart OBS Studio ☐
   - Plugin no longer in sources list ☐

**Status:** ☐ Pass ☐ Fail ☐ Not Tested

---

## Critical Issues to Watch For

### High Priority
- [ ] Text cut off on any alignment
- [ ] Newest entry not at bottom when unchecked
- [ ] Crash when changing alignment
- [ ] Settings not saving/loading

### Medium Priority
- [ ] Fade effect causing visual glitches
- [ ] Alignment not updating immediately
- [ ] Performance issues with multiple sources

### Low Priority
- [ ] Installer cosmetic issues
- [ ] Minor text positioning differences

---

## Performance Testing

1. Add **5** Keystroke History sources with different settings
2. Type continuously for 30 seconds
3. **Expected:** Smooth performance, no lag, no frame drops

**Status:** ☐ Pass ☐ Fail

---

## Test Results Summary

Total Tests: 14  
Passed: ___  
Failed: ___  
Not Tested: ___

**Overall Status:** ☐ Ready for Release ☐ Needs Fixes

---

## Issues Found

| Test # | Issue Description | Severity | Fixed? |
|--------|------------------|----------|--------|
|        |                  |          |        |
|        |                  |          |        |
|        |                  |          |        |

---

## Sign-Off

**Tested By:** ________________  
**Date:** ________________  
**OBS Version:** ________________  
**Windows Version:** ________________  

**Ready for Release:** ☐ Yes ☐ No

**Notes:**
```

