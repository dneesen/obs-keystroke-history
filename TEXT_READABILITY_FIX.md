# Text Readability Fix

**Issue:** Text was outlined, semi-transparent, and nearly impossible to read

## Root Causes Identified

1. **Transparent Background Mode**: `SetBkMode(hdc, TRANSPARENT)` was making text rendering use alpha blending incorrectly
2. **Fade Alpha Not Applied**: We calculated fade alpha but never actually used it in rendering
3. **Alpha Channel Issues**: GDI might not set alpha channel properly in DIB sections
4. **Aggressive Fade**: The fade calculation was making text semi-transparent from the start

## Fixes Applied

### 1. Text Rendering Mode
**Before:**
```cpp
SetBkMode(hdc, TRANSPARENT);
```

**After:**
```cpp
SetBkMode(hdc, OPAQUE);
SetBkColor(hdc, RGB(bg_r, bg_g, bg_b));
```

This ensures text is rendered solidly with proper background fill.

### 2. Alpha Channel Enforcement
**Before:**
```cpp
pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
```

**After:**
```cpp
pixels[i] = (255 << 24) | (r << 16) | (g << 8) | b; // Force full opacity
```

This ensures all pixels are fully opaque regardless of what GDI writes.

### 3. Fade Effect Disabled
**Before:**
```cpp
if (context->fade_duration > 0) {
    float position_ratio = (float)i / (std::max)(1.0f, (float)num_entries);
    entry.alpha = position_ratio * 0.3f + 0.7f;
    // Complex fade calculations...
}
```

**After:**
```cpp
// Disable fade for now - just keep entries at full opacity
entry.alpha = 1.0f;
```

This keeps all entries fully visible until we confirm text rendering works properly.

## Testing Results Expected

- ✅ Text should be solid and fully readable
- ✅ Text color matches settings
- ✅ Background color matches settings
- ✅ No transparency issues
- ✅ Entries stay visible (don't fade)

## Re-enabling Fade (Later)

Once text rendering is confirmed working, you can re-enable fade by:

### Option 1: Simple Disappear (No Fade)
Uncomment in `keystroke-source.cpp`:
```cpp
if (context->fade_duration > 0 && elapsed > context->fade_duration) {
    entry.alpha = 0.0f; // Mark for removal
}
```

### Option 2: Gradual Fade (Requires Per-Pixel Alpha)
This is more complex and requires:
1. Rendering each entry to a separate bitmap
2. Applying alpha blending per entry
3. Compositing entries together
4. More CPU intensive

**Recommendation:** Start with Option 1 (simple disappear) as it's much simpler and still provides a clean visual experience.

## Next Steps

1. Test with `.\install-plugin.ps1` and verify text is readable
2. Confirm color settings work properly
3. If text is solid and readable, can add back simple disappear effect
4. Later: Consider implementing proper per-entry fade if needed

---

**Key Takeaway:** GDI text rendering with transparent background mode doesn't play well with alpha blending in OBS. Using opaque mode with solid colors is much more reliable.
