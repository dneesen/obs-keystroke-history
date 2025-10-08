#pragma once

#include <obs-module.h>
#include <graphics/graphics.h>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>

struct keystroke_entry {
    std::string text;
    std::chrono::steady_clock::time_point timestamp;
    float alpha; // For fade effect
};

struct keystroke_source {
    obs_source_t* source;
    
    // Text rendering
    gs_texture_t* texture;
    uint32_t cx;
    uint32_t cy;
    
    // Settings
    int max_entries;
    bool show_mouse_clicks;
    bool ignore_modifier_keys_alone;
    float fade_duration; // seconds
    bool capture_area_only; // Only capture when mouse is in a specific area
    std::string target_window; // Window title to monitor (empty = any window)
    std::string capture_source_name; // OBS source name to monitor (display/window capture)
    bool use_source_capture; // true = use OBS source, false = use window title
    bool group_keystrokes; // Group rapid keystrokes together
    float group_duration; // Maximum time between keystrokes to group (seconds)
    bool display_newest_on_top; // true = newest at top, false = newest at bottom
    
    // Font settings
    std::string font_name;
    int font_size;
    uint32_t font_color;
    uint32_t background_color;
    bool show_background;
    float background_opacity; // 0.0 to 1.0
    std::string text_alignment; // "left", "center", or "right"
    
    // Keystroke history
    std::vector<keystroke_entry> entries;
    std::mutex entries_mutex;
    
    // Input capture state
    bool is_capturing;
    std::string current_modifiers;
    bool in_password_field;
    
    // Last update time for fade effect
    std::chrono::steady_clock::time_point last_update;
    
    // Keystroke grouping
    std::chrono::steady_clock::time_point last_keystroke_time;
    std::string current_group; // Accumulator for grouped keystrokes
};

// Registration function
void keystroke_source_register();

// Input capture functions
void start_input_capture(keystroke_source* context);
void stop_input_capture(keystroke_source* context);
void add_keystroke(keystroke_source* context, const std::string& keystroke);
