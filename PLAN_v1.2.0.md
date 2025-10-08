# Development Plan: v1.2.0

## Target Features

### 1. Text Justification (Left/Center/Right)
**Priority:** High  
**Complexity:** Low

**Implementation:**
- Add dropdown property: "Text Alignment" with options: Left, Center, Right
- Store as enum or string in keystroke_source struct
- Modify DrawTextA flags: `DT_LEFT`, `DT_CENTER`, `DT_RIGHT`
- Update locale strings

**Files to modify:**
- `src/keystroke-source.h` - Add text_alignment field
- `src/keystroke-source.cpp` - Add property, defaults, update logic
- `src/text-renderer.cpp` - Apply alignment in DrawTextA call
- `data/locale/en-US.ini` - Add TextAlignment string

---

### 2. Fix Text Alignment (Newest at Bottom)
**Priority:** CRITICAL  
**Complexity:** Medium

**Current Issue:**
When `display_newest_on_top = false`:
- Entries stored: [oldest...newest] (push_back)
- Rendering: Reverses order, renders oldest-to-newest from top
- Result: Newest appears at TOP (incorrect!)
- Expected: Newest appears at BOTTOM

**Root Cause:**
The rendering logic is backwards. When newest should be at bottom:
- Entries array: index 0 = oldest, last index = newest ✓ Correct
- Rendering: Should render forward (0 to N) from bottom up
- Currently: Renders backward (N to 0) which puts newest at top ✗ Wrong

**Solution:**
When `display_newest_on_top = false`:
- Keep storage order (oldest at 0, newest at end)
- Render FORWARD order (0 to N) 
- Start from BOTTOM and work upward
- This puts oldest at top, newest at bottom ✓

**Pseudocode:**
```cpp
if (display_newest_on_top) {
    // Entries: [newest...oldest] (insert at begin)
    // Render forward from TOP
    y = padding;
    for (i = 0 to N) {
        draw entries[i] at y;
        y += line_height;
    }
} else {
    // Entries: [oldest...newest] (push_back)
    // Render forward from BOTTOM
    y = height - padding - line_height; // Start at bottom
    for (i = 0 to N) {
        draw entries[i] at y;
        y -= line_height; // Move UP for next entry
    }
}
```

**Files to modify:**
- `src/text-renderer.cpp` - Fix rendering order logic

---

### 3. Windows EXE Installer
**Priority:** Medium  
**Complexity:** Medium

**Options:**
1. **Inno Setup** (Recommended)
   - Free, widely used
   - Script already exists: `installer.iss`
   - Compile with Inno Setup Compiler
   
2. **NSIS**
   - Free, alternative option
   - Would need to create .nsi script

**Implementation Steps:**
1. Install Inno Setup Compiler
2. Test existing `installer.iss` script
3. Compile to create `keystroke-history-setup.exe`
4. Test installer on clean system
5. Include in release assets

**Files involved:**
- `installer.iss` (already exists)
- Output: `keystroke-history-setup-v1.2.0.exe`

---

## Implementation Order

### Phase 1: Fix Critical Issue (Newest at Bottom)
**Estimated Time:** 30 minutes  
**Priority:** Do this FIRST

1. Modify `src/text-renderer.cpp` rendering logic
2. Test both modes:
   - Checked: Newest at top (already works)
   - Unchecked: Newest at bottom (fix this)
3. Verify anchor points work correctly

### Phase 2: Add Text Justification
**Estimated Time:** 1 hour  
**Priority:** Do this SECOND

1. Add property to keystroke-source
2. Update text renderer to use alignment
3. Test all three alignments
4. Update documentation

### Phase 3: Create Windows Installer
**Estimated Time:** 2 hours  
**Priority:** Do this LAST

1. Install Inno Setup
2. Test/modify `installer.iss`
3. Compile installer
4. Test on clean Windows 10/11
5. Document installer build process

---

## Testing Checklist

### Text Alignment (Newest at Bottom)
- [ ] With 5 entries, newest (entry 5) is at the BOTTOM
- [ ] Oldest (entry 1) is at the TOP
- [ ] When positioned at bottom of canvas, newest stays at bottom edge
- [ ] New entries appear ABOVE the previous newest

### Text Justification
- [ ] Left alignment: Text starts at left edge
- [ ] Center alignment: Text centered in source width
- [ ] Right alignment: Text ends at right edge
- [ ] All three work with both newest-top and newest-bottom

### Windows Installer
- [ ] Detects OBS installation automatically
- [ ] Installs plugin to correct directory
- [ ] Creates uninstaller
- [ ] Works on Windows 10
- [ ] Works on Windows 11
- [ ] Properly removes plugin on uninstall

---

## Documentation Updates

### Files to Update:
- `WHATS_NEW_v1.2.0.md` - User-facing release notes
- `TESTING_v1.2.0.md` - Testing procedures
- `RELEASE_v1.2.0.md` - Release summary
- `README.md` - Update features list
- `PACKAGING.md` - Add EXE build instructions

---

## Release Artifacts

### v1.2.0 Distribution:
1. `obs-keystroke-history-manual-install.zip` (~34 KB)
2. `keystroke-history-setup-v1.2.0.exe` (~200 KB estimated)

---

## Timeline Estimate

- **Phase 1 (Fix alignment):** 30 minutes
- **Phase 2 (Text justification):** 1 hour  
- **Phase 3 (Installer):** 2 hours
- **Testing:** 1 hour
- **Documentation:** 30 minutes

**Total:** ~5 hours

---

## Notes

- Fix alignment issue BEFORE adding justification feature
- Justification should work with both display modes
- Installer is nice-to-have, ZIP is sufficient if time limited
- Consider adding checkbox: "Preview alignment" that shows test text

---

**Status:** Phase 1 & 2 Complete ✅  
**Ready to Start:** Phase 3 (Windows Installer)  
**Next Step:** Install Inno Setup Compiler and build .exe installer
