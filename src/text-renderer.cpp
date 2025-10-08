#include "text-renderer.h"
#include <obs-module.h>
#include <graphics/graphics.h>
#include <util/platform.h>
#include <vector>
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
    int height = line_height * (int)entries_copy.size() + padding * 2;
    
    blog(LOG_INFO, "[RENDER] Dimensions: %dx%d", width, height);
    
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
    
    // Fill background - use Windows BGRA format (bottom-up DIB)
    // If show_background is off, use transparent background (alpha = 0)
    for (int i = 0; i < width * height; i++) {
        pixels[i] = (bg_b) | (bg_g << 8) | (bg_r << 16) | (bg_alpha << 24);
    }
    
    // Create font
    HFONT hFont = CreateFontA(
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
        context->font_name.c_str()    // Font name
    );
    
    if (!hFont) {
        blog(LOG_ERROR, "[RENDER] Failed to create font");
        SelectObject(hdc, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdc);
        return;
    }
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    
    // Extract text color (ARGB format from settings)
    uint32_t text_color = context->font_color;
    int text_r = (text_color >> 16) & 0xFF;
    int text_g = (text_color >> 8) & 0xFF;
    int text_b = text_color & 0xFF;
    
    SetTextColor(hdc, RGB(text_r, text_g, text_b));
    
    // Only draw opaque background if show_background is enabled
    if (context->show_background) {
        SetBkMode(hdc, OPAQUE);
        SetBkColor(hdc, RGB(bg_r, bg_g, bg_b));
    } else {
        SetBkMode(hdc, TRANSPARENT);
    }
    
    // Render each entry - NO FADE for now, just solid text
    int y_pos = padding;
    for (size_t i = 0; i < entries_copy.size(); i++) {
        const auto& entry = entries_copy[i];
        
        // Draw text - fully opaque for now
        RECT rect = { padding, y_pos, width - padding, y_pos + line_height };
        DrawTextA(hdc, entry.text.c_str(), -1, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
        y_pos += line_height;
    }
    
    
    // GDI writes in BGRA format with text rendering
    // When SetBkMode is TRANSPARENT, GDI doesn't write alpha properly for text
    // We need to detect text pixels and make them fully opaque
    for (int i = 0; i < width * height; i++) {
        uint32_t pixel = pixels[i];
        uint8_t b = pixel & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t r = (pixel >> 16) & 0xFF;
        uint8_t a = (pixel >> 24) & 0xFF;
        
        // Check if this pixel was modified by DrawText
        // If the pixel doesn't match the background we filled, it's text
        bool is_text = (b != bg_b || g != bg_g || r != bg_r);
        
        if (is_text) {
            // This is text - make it fully opaque regardless of color
            pixels[i] = (255 << 24) | (r << 16) | (g << 8) | b; // BGRA with alpha=255
        } else {
            // This is background - keep the alpha we set (respects show_background and opacity)
            pixels[i] = (bg_alpha << 24) | (r << 16) | (g << 8) | b; // BGRA with background alpha
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