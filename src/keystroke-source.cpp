#include "keystroke-source.h"
#include "input-capture.h"
#include "text-renderer.h"
#include <obs-module.h>
#include <util/platform.h>
#include <algorithm>
#include <cmath>

// Forward declarations
static const char* keystroke_source_get_name(void* unused);
static void* keystroke_source_create(obs_data_t* settings, obs_source_t* source);
static void keystroke_source_destroy(void* data);
static void keystroke_source_update(void* data, obs_data_t* settings);
static void keystroke_source_render(void* data, gs_effect_t* effect);
static uint32_t keystroke_source_get_width(void* data);
static uint32_t keystroke_source_get_height(void* data);
static void keystroke_source_get_defaults(obs_data_t* settings);
static obs_properties_t* keystroke_source_get_properties(void* data);
static void keystroke_source_tick(void* data, float seconds);

void keystroke_source_register()
{
    struct obs_source_info keystroke_source_info = {};
    keystroke_source_info.id = "keystroke_history_source";
    keystroke_source_info.type = OBS_SOURCE_TYPE_INPUT;
    keystroke_source_info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW;
    keystroke_source_info.get_name = keystroke_source_get_name;
    keystroke_source_info.create = keystroke_source_create;
    keystroke_source_info.destroy = keystroke_source_destroy;
    keystroke_source_info.update = keystroke_source_update;
    keystroke_source_info.video_render = keystroke_source_render;
    keystroke_source_info.get_width = keystroke_source_get_width;
    keystroke_source_info.get_height = keystroke_source_get_height;
    keystroke_source_info.get_defaults = keystroke_source_get_defaults;
    keystroke_source_info.get_properties = keystroke_source_get_properties;
    keystroke_source_info.video_tick = keystroke_source_tick;
    
    obs_register_source(&keystroke_source_info);
}

static const char* keystroke_source_get_name(void* unused)
{
    UNUSED_PARAMETER(unused);
    return obs_module_text("KeystrokeHistory");
}

static void* keystroke_source_create(obs_data_t* settings, obs_source_t* source)
{
    keystroke_source* context = new keystroke_source();
    context->source = source;
    context->texture = nullptr;
    context->cx = 400;
    context->cy = 200;
    context->is_capturing = false;
    context->in_password_field = false;
    context->last_update = std::chrono::steady_clock::now();
    context->last_keystroke_time = std::chrono::steady_clock::now();
    context->current_group = "";
    
    // Update settings first before starting capture
    keystroke_source_update(context, settings);
    
    // DON'T start input capture immediately - wait for first tick
    // This prevents crashes during OBS initialization
    // start_input_capture(context);
    
    blog(LOG_INFO, "Keystroke History source created (capture will start on first render)");
    return context;
}

static void keystroke_source_destroy(void* data)
{
    keystroke_source* context = static_cast<keystroke_source*>(data);
    
    stop_input_capture(context);
    
    if (context->texture) {
        obs_enter_graphics();
        gs_texture_destroy(context->texture);
        obs_leave_graphics();
    }
    
    delete context;
    blog(LOG_INFO, "Keystroke History source destroyed");
}

static void keystroke_source_update(void* data, obs_data_t* settings)
{
    keystroke_source* context = static_cast<keystroke_source*>(data);
    
    context->max_entries = (int)obs_data_get_int(settings, "max_entries");
    context->show_mouse_clicks = obs_data_get_bool(settings, "show_mouse_clicks");
    context->ignore_modifier_keys_alone = obs_data_get_bool(settings, "ignore_modifier_keys");
    context->fade_duration = (float)obs_data_get_double(settings, "fade_duration");
    
    context->font_name = obs_data_get_string(settings, "font_name");
    context->font_size = (int)obs_data_get_int(settings, "font_size");
    context->font_color = (uint32_t)obs_data_get_int(settings, "font_color");
    context->text_alignment = obs_data_get_string(settings, "text_alignment");
    context->background_color = (uint32_t)obs_data_get_int(settings, "background_color");
    context->show_background = obs_data_get_bool(settings, "show_background");
    context->background_opacity = (float)obs_data_get_double(settings, "background_opacity");
    context->capture_area_only = obs_data_get_bool(settings, "capture_area_only");
    context->target_window = obs_data_get_string(settings, "target_window");
    context->capture_source_name = obs_data_get_string(settings, "capture_source_name");
    context->use_source_capture = obs_data_get_bool(settings, "use_source_capture");
    
    // Log filter configuration for debugging
    if (context->capture_area_only) {
        if (context->use_source_capture && !context->capture_source_name.empty()) {
            blog(LOG_INFO, "[CONFIG] Source filtering ENABLED - Target source: '%s'", context->capture_source_name.c_str());
        } else if (!context->target_window.empty()) {
            blog(LOG_INFO, "[CONFIG] Window filtering ENABLED - Target: '%s'", context->target_window.c_str());
        } else {
            blog(LOG_INFO, "[CONFIG] Filtering ENABLED - Capturing from ALL windows");
        }
    } else {
        blog(LOG_INFO, "[CONFIG] Filtering DISABLED - Capturing from all windows");
    }
    
    context->group_keystrokes = obs_data_get_bool(settings, "group_keystrokes");
    context->group_duration = (float)obs_data_get_double(settings, "group_duration");
    context->display_newest_on_top = obs_data_get_bool(settings, "display_newest_on_top");
    
    // Limit entries to max
    std::lock_guard<std::mutex> lock(context->entries_mutex);
    if (context->entries.size() > (size_t)context->max_entries) {
        context->entries.erase(context->entries.begin(), 
            context->entries.begin() + (context->entries.size() - context->max_entries));
    }
}

static void keystroke_source_tick(void* data, float seconds)
{
    UNUSED_PARAMETER(seconds);
    keystroke_source* context = static_cast<keystroke_source*>(data);
    
    if (!context)
        return;
    
    // Start input capture on first tick if not already started
    if (!context->is_capturing) {
        try {
            start_input_capture(context);
            blog(LOG_INFO, "Input capture started successfully");
        } catch (...) {
            blog(LOG_ERROR, "Failed to start input capture");
        }
    }
    
    auto now = std::chrono::steady_clock::now();
    context->last_update = now;
    
    // Update alpha values for fade effect - scope the lock
    {
        std::lock_guard<std::mutex> lock(context->entries_mutex);
        
        // Don't render if we have no entries
        if (context->entries.empty()) {
            return;
        }
        
        blog(LOG_DEBUG, "[TICK] Updating alpha for %d entries", (int)context->entries.size());
        
        // Simple disappear after duration (no fade)
        size_t num_entries = context->entries.size();
        for (size_t i = 0; i < num_entries; i++) {
            auto& entry = context->entries[i];
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - entry.timestamp).count() / 1000.0f;
            
            // Keep at full opacity until fade_duration expires, then mark for removal
            if (context->fade_duration > 0 && elapsed > context->fade_duration) {
                entry.alpha = 0.0f; // Mark for removal (disappear, not fade)
            } else {
                entry.alpha = 1.0f; // Fully visible
            }
        }
        
        blog(LOG_DEBUG, "[TICK] Alpha update complete");
        
        // Remove fully faded entries
        auto initial_size = context->entries.size();
        context->entries.erase(
            std::remove_if(context->entries.begin(), context->entries.end(),
                [](const keystroke_entry& e) { return e.alpha <= 0.01f; }),
            context->entries.end()
        );
        
        if (context->entries.size() != initial_size) {
            blog(LOG_DEBUG, "[TICK] Removed %d faded entries", (int)(initial_size - context->entries.size()));
        }
    } // Lock released here!
    
    // Now render WITHOUT holding the lock
    // render_text_to_texture will acquire its own lock
    if (!context->entries.empty()) {
        blog(LOG_DEBUG, "[TICK] About to render %d entries", (int)context->entries.size());
        render_text_to_texture(context);
    } else {
        blog(LOG_DEBUG, "[TICK] No entries to render (after fade removal)");
    }
}

static void keystroke_source_render(void* data, gs_effect_t* effect)
{
    keystroke_source* context = static_cast<keystroke_source*>(data);
    
    if (!context->texture)
        return;
    
    effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
    
    gs_eparam_t* image = gs_effect_get_param_by_name(effect, "image");
    gs_effect_set_texture(image, context->texture);
    
    while (gs_effect_loop(effect, "Draw")) {
        gs_draw_sprite(context->texture, 0, context->cx, context->cy);
    }
}

static uint32_t keystroke_source_get_width(void* data)
{
    keystroke_source* context = static_cast<keystroke_source*>(data);
    return context->cx;
}

static uint32_t keystroke_source_get_height(void* data)
{
    keystroke_source* context = static_cast<keystroke_source*>(data);
    return context->cy;
}

static void keystroke_source_get_defaults(obs_data_t* settings)
{
    obs_data_set_default_int(settings, "max_entries", 5);
    obs_data_set_default_bool(settings, "show_mouse_clicks", true);
    obs_data_set_default_bool(settings, "ignore_modifier_keys", true);
    obs_data_set_default_double(settings, "fade_duration", 3.0);
    
    obs_data_set_default_string(settings, "font_name", "Arial");
    obs_data_set_default_int(settings, "font_size", 24);
    obs_data_set_default_int(settings, "font_color", 0xFFFFFFFF); // White
    obs_data_set_default_string(settings, "text_alignment", "left"); // Default: left aligned
    obs_data_set_default_int(settings, "background_color", 0x000000); // Black
    obs_data_set_default_bool(settings, "show_background", true);
    obs_data_set_default_double(settings, "background_opacity", 0.5); // 50% opacity
    obs_data_set_default_bool(settings, "capture_area_only", false);
    obs_data_set_default_string(settings, "target_window", "");
    obs_data_set_default_string(settings, "capture_source_name", "");
    obs_data_set_default_bool(settings, "use_source_capture", false);
    obs_data_set_default_bool(settings, "group_keystrokes", false);
    obs_data_set_default_double(settings, "group_duration", 0.5);
    obs_data_set_default_bool(settings, "display_newest_on_top", false); // Default: newest at bottom
}

static obs_properties_t* keystroke_source_get_properties(void* data)
{
    UNUSED_PARAMETER(data);
    
    obs_properties_t* props = obs_properties_create();
    
    obs_properties_add_int_slider(props, "max_entries",
        obs_module_text("MaxEntries"), 1, 20, 1);
    
    obs_properties_add_bool(props, "show_mouse_clicks",
        obs_module_text("ShowMouseClicks"));
    
    obs_properties_add_bool(props, "ignore_modifier_keys",
        obs_module_text("IgnoreModifierKeys"));
    
    obs_properties_add_float_slider(props, "fade_duration",
        obs_module_text("FadeDuration"), 0.0, 10.0, 0.5);
    
    // Font dropdown with common cross-platform fonts
    obs_property_t* font_list = obs_properties_add_list(props, "font_name",
        obs_module_text("FontName"), OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
    
    // Add common fonts available across Windows, macOS, and Linux
    obs_property_list_add_string(font_list, "Arial", "Arial");
    obs_property_list_add_string(font_list, "Arial Black", "Arial Black");
    obs_property_list_add_string(font_list, "Calibri", "Calibri");
    obs_property_list_add_string(font_list, "Cambria", "Cambria");
    obs_property_list_add_string(font_list, "Comic Sans MS", "Comic Sans MS");
    obs_property_list_add_string(font_list, "Consolas", "Consolas");
    obs_property_list_add_string(font_list, "Courier New", "Courier New");
    obs_property_list_add_string(font_list, "Georgia", "Georgia");
    obs_property_list_add_string(font_list, "Impact", "Impact");
    obs_property_list_add_string(font_list, "Lucida Console", "Lucida Console");
    obs_property_list_add_string(font_list, "Segoe UI", "Segoe UI");
    obs_property_list_add_string(font_list, "Tahoma", "Tahoma");
    obs_property_list_add_string(font_list, "Times New Roman", "Times New Roman");
    obs_property_list_add_string(font_list, "Trebuchet MS", "Trebuchet MS");
    obs_property_list_add_string(font_list, "Verdana", "Verdana");
    
    obs_properties_add_int_slider(props, "font_size",
        obs_module_text("FontSize"), 8, 72, 1);
    
    obs_properties_add_color(props, "font_color",
        obs_module_text("FontColor"));
    
    // Text alignment dropdown
    obs_property_t* alignment_list = obs_properties_add_list(props, "text_alignment",
        obs_module_text("TextAlignment"), OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
    obs_property_list_add_string(alignment_list, obs_module_text("TextAlignment.Left"), "left");
    obs_property_list_add_string(alignment_list, obs_module_text("TextAlignment.Center"), "center");
    obs_property_list_add_string(alignment_list, obs_module_text("TextAlignment.Right"), "right");
    
    obs_properties_add_bool(props, "show_background",
        obs_module_text("ShowBackground"));
    
    obs_properties_add_color(props, "background_color",
        obs_module_text("BackgroundColor"));
    
    obs_properties_add_float_slider(props, "background_opacity",
        obs_module_text("BackgroundOpacity"), 0.0, 1.0, 0.01);
    
    // Display direction
    obs_properties_add_bool(props, "display_newest_on_top",
        obs_module_text("DisplayNewestOnTop"));
    
    // Keystroke grouping settings
    obs_properties_add_bool(props, "group_keystrokes",
        obs_module_text("GroupKeystrokes"));
    
    obs_properties_add_float_slider(props, "group_duration",
        obs_module_text("GroupDuration"), 0.1, 2.0, 0.1);
    
    // Area-based capture settings
    obs_properties_add_bool(props, "capture_area_only",
        obs_module_text("CaptureAreaOnly"));
    
    // Radio-style selection: Use source capture or window title
    obs_properties_add_bool(props, "use_source_capture",
        obs_module_text("UseSourceCapture"));
    
    // Note: Removed scene filtering checkbox since frontend API isn't available for plugins
    // All capture sources from all scenes are shown
    
    // Dropdown for OBS capture sources (Display/Window/Game Capture)
    // Shows all capture sources from all scenes (no filtering available without frontend API)
    obs_property_t* source_list = obs_properties_add_list(props, "capture_source_name",
        obs_module_text("CaptureSourceName"), OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
    
    obs_property_list_add_string(source_list, "-- Select Source --", "");
    
    // Enumerate all Display Capture, Window Capture, and Game Capture sources
    obs_enum_sources([](void* param, obs_source_t* source) -> bool {
        obs_property_t* list = static_cast<obs_property_t*>(param);
        const char* source_id = obs_source_get_id(source);
        
        // Only add capture sources
        if (strcmp(source_id, "monitor_capture") == 0 ||      // Display Capture
            strcmp(source_id, "window_capture") == 0 ||       // Window Capture
            strcmp(source_id, "game_capture") == 0) {         // Game Capture
            
            const char* name = obs_source_get_name(source);
            if (name && strlen(name) > 0) {
                obs_property_list_add_string(list, name, name);
            }
        }
        
        return true; // Continue enumeration
    }, source_list);
    
    // Text field for target window title (partial match, case-insensitive)
    obs_property_t* target_window = obs_properties_add_text(props, "target_window",
        obs_module_text("TargetWindow"), OBS_TEXT_DEFAULT);
    
    obs_property_set_long_description(target_window,
        "Enter part of the window title to filter (e.g., 'notepad', 'chrome', 'autocad'). "
        "Leave empty to capture from all windows. Case-insensitive partial match. "
        "Ignored if 'Use OBS Source Capture' is enabled.");
    
    return props;
}

void add_keystroke(keystroke_source* context, const std::string& keystroke)
{
    if (!context || keystroke.empty())
        return;
    
    std::lock_guard<std::mutex> lock(context->entries_mutex);
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - context->last_keystroke_time).count() / 1000.0f;
    
    // Check if we should count repetitions or group keystrokes
    if (!context->entries.empty() && elapsed < 1.0f) { // Within 1 second window
        // Always work with the most recent entry (at end if newest_on_bottom, at start if newest_on_top)
        size_t recent_index = context->display_newest_on_top ? 0 : context->entries.size() - 1;
        auto& last_entry = context->entries[recent_index];
        
        // Check for key repetition (same key pressed multiple times)
        // Extract base key without count (e.g., "A x3" -> "A")
        std::string last_key = last_entry.text;
        size_t x_pos = last_key.find(" x");
        if (x_pos != std::string::npos) {
            last_key = last_key.substr(0, x_pos);
        }
        
        // If same key pressed again within window, increment count
        if (last_key == keystroke) {
            // Extract current count
            int count = 1;
            if (x_pos != std::string::npos) {
                std::string count_str = last_entry.text.substr(x_pos + 2);
                count = std::stoi(count_str);
            }
            count++;
            
            // Update entry with count
            last_entry.text = keystroke + " x" + std::to_string(count);
            last_entry.timestamp = now;
            context->last_keystroke_time = now;
            blog(LOG_INFO, "[ENTRIES] Repeated keystroke: '%s'", last_entry.text.c_str());
            return;
        }
        
        // Check if we should group this keystroke (if enabled)
        if (context->group_keystrokes && elapsed < context->group_duration) {
            bool is_letter = keystroke.length() == 1 && isalpha(keystroke[0]);
            bool is_number = keystroke.length() == 1 && isdigit(keystroke[0]);
            bool is_groupable = is_letter || is_number;
            
            // Check if last entry is groupable text (no " x" count, no modifiers)
            bool last_is_groupable = last_key.find(" + ") == std::string::npos && 
                                     last_key.find("Click") == std::string::npos &&
                                     last_key.find("Scroll") == std::string::npos &&
                                     x_pos == std::string::npos;
            
            // If both are groupable, append
            if (is_groupable && last_is_groupable && !context->current_group.empty()) {
                context->current_group += keystroke;
                last_entry.text = context->current_group;
                last_entry.timestamp = now;
                context->last_keystroke_time = now;
                blog(LOG_INFO, "[ENTRIES] Grouped keystroke: '%s'", context->current_group.c_str());
                return;
            } else if (is_groupable && last_is_groupable) {
                // Start grouping with last entry + this one
                context->current_group = last_key + keystroke;
                last_entry.text = context->current_group;
                last_entry.timestamp = now;
                context->last_keystroke_time = now;
                blog(LOG_INFO, "[ENTRIES] Started grouping: '%s'", context->current_group.c_str());
                return;
            } else if (!is_groupable && !context->current_group.empty()) {
                // Non-groupable key ends the group
                last_entry.text = context->current_group + " + " + keystroke;
                last_entry.timestamp = now;
                context->last_keystroke_time = now;
                context->current_group.clear();
                blog(LOG_INFO, "[ENTRIES] Completed group with special key: '%s'", last_entry.text.c_str());
                return;
            }
        }
    }
    
    // Add as new entry
    context->current_group.clear();
    
    keystroke_entry entry;
    entry.text = keystroke;
    entry.timestamp = now;
    entry.alpha = 1.0f;
    
    // Add to beginning (newest on top) or end (newest at bottom)
    if (context->display_newest_on_top) {
        context->entries.insert(context->entries.begin(), entry);
    } else {
        context->entries.push_back(entry);
    }
    context->last_keystroke_time = now;
    
    blog(LOG_INFO, "[ENTRIES] Added keystroke: '%s' (total entries: %d)", 
         keystroke.c_str(), (int)context->entries.size());
    
    // Maintain max entries limit
    if (context->entries.size() > (size_t)context->max_entries) {
        if (context->display_newest_on_top) {
            context->entries.pop_back(); // Remove oldest from bottom
        } else {
            context->entries.erase(context->entries.begin()); // Remove oldest from top
        }
    }
}
