# Testing Guide for v1.1.0

## Installation
1. Run PowerShell as Administrator
2. Navigate to plugin directory: `cd "d:\OBS Plugins\Keystroke History"`
3. Run: `.\install-plugin.ps1`
4. Restart OBS Studio

## Test 1: Display Direction & Anchor Point

### Test Newest at Bottom (Default - Unchecked)
1. Add "Keystroke History" source to scene
2. **Uncheck** "Display Newest Entries at Top" 
3. Position source at **bottom-left** of canvas
4. Type some keys: A, B, C, D, E

**Expected Behavior:**
- Source height stays constant (based on max_entries setting)
- First keystroke "A" appears at the BOTTOM of the source
- Each new keystroke appears BELOW the previous one, pushing up
- Text "stacks upward" from the bottom
- Oldest entries at top of source, newest at bottom
- **Source effectively anchors to the bottom**

**Visual:**
```
[  E        ] <- Empty space at top
[  D        ]
[  C        ]
[  B        ]
[  A        ] <- Newest at bottom, anchors here
```

### Test Newest at Top (Checked)
1. **Check** "Display Newest Entries at Top"
2. Position source at **top-left** of canvas
3. Type some keys: F, G, H, I, J

**Expected Behavior:**
- Source height stays constant
- First keystroke "F" appears at the TOP
- Each new keystroke appears ABOVE the previous one (pushes down)
- Text "stacks downward" from the top
- Newest entries at top, oldest at bottom
- **Source effectively anchors to the top**

**Visual:**
```
[  J        ] <- Newest at top, anchors here
[  I        ]
[  H        ]
[  G        ]
[  F        ]
[           ] <- Empty space at bottom
```

---

## Test 2: OBS Source-Based Filtering

### Setup
1. In OBS main window, create test sources:
   - Add **Display Capture** (e.g., "Display 2")
   - Add **Window Capture** (e.g., "Notepad")
   - Note the exact names shown in Sources panel

2. In Keystroke History source properties:
   - **Check** "Enable Window/Source Filtering"
   - **Check** "Use OBS Source Capture"
   - Click the "OBS Capture Source" dropdown

### Test A: Display Capture Filtering

**Expected:** Dropdown shows ALL Display Capture sources from your OBS project (all scenes)

**Test Steps:**
1. Select your display capture source from dropdown (e.g., "Display 2")
2. Click OK/Apply
3. Move your active window to that display
4. Type: TEST123

**Expected Behavior:**
- Keystrokes only appear when typing in windows on the selected display
- Check OBS log for: `[SOURCE-FILTER] Display capture: monitor_id='...', current_device='...', match=YES`
- If window is on different display, should see: `match=NO`

**Debug in OBS Log:**
- Look for: `[SOURCE-FILTER] Checking source 'Display 2' (type: monitor_capture)`
- Look for: `[SOURCE-FILTER] Source settings: {...}` (shows monitor_id)

### Test B: Window Capture Filtering

**Test Steps:**
1. In dropdown, select your Window Capture source (e.g., "Notepad")
2. Click OK/Apply
3. Click into the target window (make it active)
4. Type: WINDOW-TEST

**Expected Behavior:**
- Keystrokes only appear when typing in the captured window
- Check OBS log for: `[SOURCE-FILTER] Window capture: target='notepad', current='notepad', match=YES`

### Test C: Source Dropdown Population

**What to check:**
- Dropdown shows ALL capture sources from ALL scenes
- Includes only:
  - Display Capture sources (type: monitor_capture)
  - Window Capture sources (type: window_capture)
  - Game Capture sources (type: game_capture)
- Does NOT show:
  - Browser sources
  - Image sources
  - Video sources
  - Other plugin sources

**Note:** Source filtering by current scene is not available due to OBS API limitations. The dropdown shows all capture sources across all scenes. Simply ignore or delete sources you don't need.

---

## Test 3: Font Dropdown

1. Open Keystroke History properties
2. Click "Font Name" dropdown

**Expected:**
- Should show 15 fonts (Arial, Calibri, Consolas, etc.)
- No text input field
- Select different fonts and verify they render correctly

---

## Test 4: Transparent Background Text Rendering

1. Set text color to any color (e.g., **Black**)
2. Set background color to any different color (e.g., **White**)
3. **Uncheck "Show Background"**
4. Type some keys

**Expected:**
- Clean text with NO background visible
- No color bleeding around text edges
- Proper anti-aliasing
- Text renders correctly even if text and background colors are the same

**Test with same colors:**
1. Set BOTH text and background to **Black**
2. Uncheck "Show Background"
3. Type keys

**Expected:**
- Black text still renders correctly
- No artifacts or invisible text

---

## Test 5: Traditional Window Title Filtering (Backward Compatibility)

1. **Uncheck** "Use OBS Source Capture"
2. In "Target Window Title" text field, type: `notepad`
3. Open Notepad and type

**Expected:**
- Works exactly as before
- Filters based on window title substring match
- Case-insensitive

---

## Known Issues & Limitations

### Limitation 1: Source List Shows All Scenes
**Behavior:** The "OBS Capture Source" dropdown shows sources from ALL scenes, not just the current scene.

**Why:** The OBS Frontend API required for scene detection isn't available to plugins.

**Workaround:** Simply ignore sources from other scenes, or delete unused sources from your project.

### Limitation 2: Display Capture Matching
**Behavior:** Display capture filtering relies on Windows monitor enumeration, which may not always match OBS's internal monitor numbering.

**Workaround:** If filtering doesn't work as expected, check the OBS log to see which monitor indices are being detected.

---

## Debugging: Enable OBS Logging

1. Help → Log Files → Upload Current Log File
2. Or check log location: `%APPDATA%\obs-studio\logs\`
3. Look for lines containing:
   - `[keystroke-history]` - General plugin messages
   - `[FILTER]` - Window filtering
   - `[SOURCE-FILTER]` - OBS source filtering
   - `[RENDER]` - Text rendering
   - `[INPUT]` - Keystroke capture

**Useful search patterns:**
- Search for: `SOURCE-FILTER` to see source matching logic
- Search for: `Display capture:` or `Window capture:` for specific match results
- Search for: `Source settings:` to see what OBS is providing

---

## Success Criteria

✅ Display direction works with proper anchoring:
- [ ] Newest at bottom anchors to bottom edge
- [ ] Newest at top anchors to top edge
- [ ] Source height remains constant

✅ Source filtering works:
- [ ] Dropdown populated with capture sources from all scenes
- [ ] Display Capture filtering works
- [ ] Window Capture filtering works
- [ ] OBS log shows correct source detection

✅ Font dropdown works:
- [ ] 15 fonts listed
- [ ] Font selection changes rendered text

✅ Transparent background works:
- [ ] No background visible when unchecked
- [ ] No color bleeding on text edges
- [ ] Works even with matching text/bg colors

✅ Backward compatibility:
- [ ] Window title filtering still works
- [ ] Existing settings preserved after update

---

## Reporting Issues

If tests fail, please provide:
1. OBS version
2. Windows version  
3. Which test failed
4. Relevant log excerpts containing `[keystroke-history]` or `[SOURCE-FILTER]`
5. Screenshot of source properties
6. List of sources in your OBS scenes
