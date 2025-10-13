#include "text-renderer.h"
#include <obs-module.h>
#include <graphics/graphics.h>
#include <util/platform.h>
#include <vector>
#include <string>
#include <windows.h>
#include <wingdi.h>

// Use plain GDI (not GDI+) for text rendering - much more stable

void render_text_to_texture(keystroke_source* context)
{
    if (!context) {
        blog(LOG_ERROR, "[RENDER] context is null");
        return;
    }
    
    // Copy entries while holding the lock briefly
    std::vector<keystroke_entry> entries_copy;
    try {
        std::lock_guard<std::mutex> lock(context->entries_mutex);
        entries_copy = context->entries;
    } catch (const std::exception& e) {
        blog(LOG_ERROR, "[RENDER] Exception during copy: %s", e.what());
        return;
    } catch (...) {
        blog(LOG_ERROR, "[RENDER] Unknown exception during copy");
        return;
    }
    
    if (entries_copy.empty()) {
        // Clear texture if no entries
        if (context->texture) {
            obs_enter_graphics();
            gs_texture_destroy(context->texture);
            context->texture = nullptr;
            obs_leave_graphics();
        }
        return;
    }
    
    // Calculate dimensions based on text
    int line_height = context->font_size + 8;
    int padding = 10;
    int width = 600;  // Fixed width for now
    
    // Use max_entries to determine a fixed height, so the source doesn't jump around
    // This allows users to anchor it properly in their scene
    int max_lines = context->max_entries > 0 ? context->max_entries : 10; // Default to 10 if not set
    int height = line_height * max_lines + padding * 2;
    
    blog(LOG_INFO, "[RENDER] Dimensions: %dx%d (max_entries=%d)", width, height, max_lines);
    
    if (width <= 0 || height <= 0) {
        blog(LOG_WARNING, "[RENDER] Invalid dimensions: %dx%d", width, height);
        return;
    }
    
    // Create GDI bitmap for rendering text
    HDC hdc = CreateCompatibleDC(NULL);
    if (!hdc) {
        blog(LOG_ERROR, "[RENDER] Failed to create DC");
        return;
    }
    
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Negative for top-down bitmap
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    uint32_t* pixels = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
    
    if (!hBitmap || !pixels) {
        blog(LOG_ERROR, "[RENDER] Failed to create bitmap");
        DeleteDC(hdc);
        return;
    }
    
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hBitmap);
    
    // Extract background color components (ARGB format from settings)
    uint32_t bg_color = context->background_color;
    int bg_alpha = context->show_background ? (int)(context->background_opacity * 255.0f) : 0;
    int bg_r = (bg_color >> 16) & 0xFF;
    int bg_g = (bg_color >> 8) & 0xFF;
    int bg_b = bg_color & 0xFF;
    
    // Extract text color for later use
    uint32_t text_color = context->font_color;
    int text_r = (text_color >> 16) & 0xFF;
    int text_g = (text_color >> 8) & 0xFF;
    int text_b = text_color & 0xFF;
    
    // Fill background with a contrasting color when background is transparent
    // This allows us to detect where text was drawn
    uint8_t fill_r, fill_g, fill_b;
    if (context->show_background) {
        // Use actual background color
        fill_r = bg_r;
        fill_g = bg_g;
        fill_b = bg_b;
    } else {
        // Use a color that contrasts with text for detection purposes
        // Use inverse of text color to ensure maximum contrast
        fill_r = 255 - text_r;
        fill_g = 255 - text_g;
        fill_b = 255 - text_b;
    }
    
    // Fill bitmap with fill color
    for (int i = 0; i < width * height; i++) {
        pixels[i] = (fill_b) | (fill_g << 8) | (fill_r << 16) | (0 << 24); // Start with alpha=0
    }
    
    // Create font - Use CreateFontW for Unicode support
    // Convert font name from UTF-8 to wide string
    int wchars_needed = MultiByteToWideChar(CP_UTF8, 0, context->font_name.c_str(), -1, NULL, 0);
    std::wstring font_name_wide(wchars_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, context->font_name.c_str(), -1, &font_name_wide[0], wchars_needed);
    
    HFONT hFont = CreateFontW(
        context->font_size,           // Height
        0,                            // Width (auto)
        0,                            // Escapement
        0,                            // Orientation
        FW_NORMAL,                    // Weight
        FALSE,                        // Italic
        FALSE,                        // Underline
        FALSE,                        // Strikeout
        DEFAULT_CHARSET,              // Charset
        OUT_DEFAULT_PRECIS,           // Output precision
        CLIP_DEFAULT_PRECIS,          // Clipping precision
        ANTIALIASED_QUALITY,          // Quality
        DEFAULT_PITCH | FF_DONTCARE,  // Pitch and family
        font_name_wide.c_str()        // Font name
    );
    
    if (!hFont) {
        blog(LOG_ERROR, "[RENDER] Failed to create font");
        SelectObject(hdc, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        return;
    }
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    
    SetTextColor(hdc, RGB(text_r, text_g, text_b));
    
    // Always use OPAQUE mode so GDI anti-aliasing works properly
    SetBkMode(hdc, OPAQUE);
    if (context->show_background) {
        SetBkColor(hdc, RGB(bg_r, bg_g, bg_b));
    } else {
        // Use the inverse color as background for rendering
        // This ensures text is always detectable regardless of text color
        SetBkColor(hdc, RGB(fill_r, fill_g, fill_b));
    }
    
    // Determine text alignment flags
    UINT alignment_flags = DT_VCENTER | DT_SINGLELINE;
    if (context->text_alignment == "center") {
        alignment_flags |= DT_CENTER;
    } else if (context->text_alignment == "right") {
        alignment_flags |= DT_RIGHT;
    } else {
        alignment_flags |= DT_LEFT; // Default
    }
    
    // Render each entry
    // If display_newest_on_top is true, render from top to bottom starting at padding
    // If display_newest_on_top is false, render from bottom to top, anchoring at the bottom
    
    if (context->display_newest_on_top) {
        // Newest at top: render entries forward (0 to N), starting from top
        int y_pos = padding;
        for (size_t i = 0; i < entries_copy.size(); i++) {
            const auto& entry = entries_copy[i];
            
            // Convert UTF-8 string to wide string for Unicode rendering
            int wchars_needed = MultiByteToWideChar(CP_UTF8, 0, entry.text.c_str(), -1, NULL, 0);
            std::wstring text_wide(wchars_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, entry.text.c_str(), -1, &text_wide[0], wchars_needed);
            
            RECT rect = { padding, y_pos, width - padding, y_pos + line_height };
            DrawTextW(hdc, text_wide.c_str(), -1, &rect, alignment_flags);
            
            y_pos += line_height;
        }
    } else {
        // Newest at bottom: render from bottom upward
        // Start at the bottom edge and work upward
        // Entries are stored [oldest...newest], so we render backwards (newest to oldest)
        // This places newest at bottom, oldest at top
        int y_pos = height - padding - line_height;
        
        // Render entries in reverse order (newest to oldest) from bottom upward
        for (int i = (int)entries_copy.size() - 1; i >= 0; i--) {
            const auto& entry = entries_copy[i];
            
            // Convert UTF-8 string to wide string for Unicode rendering
            int wchars_needed = MultiByteToWideChar(CP_UTF8, 0, entry.text.c_str(), -1, NULL, 0);
            std::wstring text_wide(wchars_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, entry.text.c_str(), -1, &text_wide[0], wchars_needed);
            
            RECT rect = { padding, y_pos, width - padding, y_pos + line_height };
            DrawTextW(hdc, text_wide.c_str(), -1, &rect, alignment_flags);
            
            y_pos -= line_height;  // Move UP for the next (older) entry
        }
    }
    
    
    // Process alpha channel for each pixel
    // GDI writes in BGRA format
    for (int i = 0; i < width * height; i++) {
        uint32_t pixel = pixels[i];
        uint8_t b = pixel & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t r = (pixel >> 16) & 0xFF;
        
        if (context->show_background) {
            // Background is visible
            // Check if this pixel is different from the fill color (background)
            bool is_text = (r != fill_r || g != fill_g || b != fill_b);
            
            if (is_text) {
                // This is text - make it fully opaque
                pixels[i] = (255 << 24) | (r << 16) | (g << 8) | b;
            } else {
                // This is background - use background alpha
                pixels[i] = (bg_alpha << 24) | (r << 16) | (g << 8) | b;
            }
        } else {
            // Background is transparent
            // We filled with text color, so pixels that match fill are background (transparent)
            // Pixels that differ are text (may be anti-aliased)
            
            bool is_background = (r == fill_r && g == fill_g && b == fill_b);
            
            if (is_background) {
                // Pure background - make fully transparent
                pixels[i] = (0 << 24) | (r << 16) | (g << 8) | b;
            } else {
                // This pixel has been modified by text rendering
                // It's either pure text or anti-aliased edge
                // Make it fully opaque (anti-aliasing already in RGB values)
                pixels[i] = (255 << 24) | (r << 16) | (g << 8) | b;
            }
        }
    }
    
    obs_enter_graphics();
    
    if (context->texture) {
        gs_texture_destroy(context->texture);
        context->texture = nullptr;
    }
    
    context->texture = gs_texture_create(width, height, GS_BGRA, 1,
        (const uint8_t**)&pixels, 0);
    
    if (!context->texture) {
        blog(LOG_ERROR, "[RENDER] Failed to create texture");
    } else {
        context->cx = width;
        context->cy = height;
    }
    
    obs_leave_graphics();
    
    // Cleanup GDI resources
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    SelectObject(hdc, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdc);
}