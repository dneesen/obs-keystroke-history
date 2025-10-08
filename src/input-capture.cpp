#include "input-capture.h"
#include <obs-module.h>
#include <obs.h>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <Windows.h>

// Global state
static HHOOK g_keyboard_hook = nullptr;
static HHOOK g_mouse_hook = nullptr;
static keystroke_source* g_context = nullptr;
static std::set<int> g_pressed_keys;

// Virtual key code to string mapping
std::string get_key_name(int vk_code, bool shift_pressed)
{
    static const std::map<int, std::string> special_keys = {
        {VK_BACK, "Backspace"},
        {VK_TAB, "Tab"},
        {VK_RETURN, "Enter"},
        {VK_SPACE, "Space"},
        {VK_ESCAPE, "Esc"},
        {VK_PRIOR, "PgUp"},
        {VK_NEXT, "PgDn"},
        {VK_END, "End"},
        {VK_HOME, "Home"},
        {VK_LEFT, "←"},
        {VK_UP, "↑"},
        {VK_RIGHT, "→"},
        {VK_DOWN, "↓"},
        {VK_INSERT, "Ins"},
        {VK_DELETE, "Del"},
        {VK_F1, "F1"}, {VK_F2, "F2"}, {VK_F3, "F3"}, {VK_F4, "F4"},
        {VK_F5, "F5"}, {VK_F6, "F6"}, {VK_F7, "F7"}, {VK_F8, "F8"},
        {VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},
        {VK_NUMLOCK, "NumLock"},
        {VK_SCROLL, "ScrollLock"},
        {VK_CAPITAL, "CapsLock"},
    };
    
    // Check for special keys
    auto it = special_keys.find(vk_code);
    if (it != special_keys.end()) {
        return it->second;
    }
    
    // Get character representation
    BYTE keyboard_state[256] = {0};
    GetKeyboardState(keyboard_state);
    
    WORD chars[2] = {0};
    int result = ToAscii(vk_code, MapVirtualKey(vk_code, MAPVK_VK_TO_VSC), 
                        keyboard_state, chars, 0);
    
    if (result > 0) {
        char c = static_cast<char>(chars[0]);
        if (c >= 32 && c <= 126) { // Printable ASCII
            // Convert lowercase to uppercase for display
            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
            return std::string(1, c);
        }
    }
    
    return "";
}

bool is_modifier_key(int vk_code)
{
    return vk_code == VK_SHIFT || vk_code == VK_CONTROL || 
           vk_code == VK_MENU || vk_code == VK_LSHIFT ||
           vk_code == VK_RSHIFT || vk_code == VK_LCONTROL ||
           vk_code == VK_RCONTROL || vk_code == VK_LMENU ||
           vk_code == VK_RMENU || vk_code == VK_LWIN ||
           vk_code == VK_RWIN;
}

std::string get_current_modifiers()
{
    std::string modifiers;
    
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        modifiers += "Ctrl + ";
    }
    if (GetAsyncKeyState(VK_MENU) & 0x8000) { // Alt key
        modifiers += "Alt + ";
    }
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
        modifiers += "Shift + ";
    }
    if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000) {
        modifiers += "Win + ";
    }
    
    return modifiers;
}

bool matches_obs_source_target(keystroke_source* context, HWND current_hwnd, const char* current_window_title)
{
    if (context->capture_source_name.empty()) {
        blog(LOG_WARNING, "[SOURCE-FILTER] No capture source specified");
        return false;
    }
    
    // Find the OBS source by name
    obs_source_t* source = obs_get_source_by_name(context->capture_source_name.c_str());
    if (!source) {
        blog(LOG_WARNING, "[SOURCE-FILTER] Source '%s' not found", context->capture_source_name.c_str());
        return false;
    }
    
    // Get source ID to determine type
    const char* source_id = obs_source_get_id(source);
    obs_data_t* settings = obs_source_get_settings(source);
    bool matches = false;
    
    blog(LOG_DEBUG, "[SOURCE-FILTER] Checking source '%s' (type: %s)", 
         context->capture_source_name.c_str(), source_id);
    
    if (strcmp(source_id, "monitor_capture") == 0) {
        // Display Capture - check if current window is on the captured monitor
        int monitor_idx = (int)obs_data_get_int(settings, "monitor");
        
        // Get monitor of current window
        HMONITOR current_monitor = MonitorFromWindow(current_hwnd, MONITOR_DEFAULTTOPRIMARY);
        
        // Enumerate monitors to find index
        struct MonitorEnumData {
            HMONITOR target;
            int index;
            int current_index;
        };
        MonitorEnumData enum_data = { current_monitor, -1, 0 };
        
        EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
            MonitorEnumData* data = (MonitorEnumData*)dwData;
            if (hMonitor == data->target) {
                data->index = data->current_index;
                return FALSE; // Stop enumeration
            }
            data->current_index++;
            return TRUE;
        }, (LPARAM)&enum_data);
        
        matches = (enum_data.index == monitor_idx);
        blog(LOG_DEBUG, "[SOURCE-FILTER] Display capture: source monitor=%d, current window monitor=%d, match=%s",
             monitor_idx, enum_data.index, matches ? "YES" : "NO");
        
    } else if (strcmp(source_id, "window_capture") == 0) {
        // Window Capture - check if current window matches the captured window
        const char* target_window = obs_data_get_string(settings, "window");
        
        // OBS stores window format as: "title:class:exe"
        // Extract just the title part for comparison
        std::string target(target_window);
        size_t colon_pos = target.find(':');
        if (colon_pos != std::string::npos) {
            target = target.substr(0, colon_pos);
        }
        
        // Case-insensitive comparison
        std::string current_title(current_window_title);
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        std::transform(current_title.begin(), current_title.end(), current_title.begin(), ::tolower);
        
        matches = (current_title.find(target) != std::string::npos) || (target.find(current_title) != std::string::npos);
        blog(LOG_DEBUG, "[SOURCE-FILTER] Window capture: target='%s', current='%s', match=%s",
             target.c_str(), current_window_title, matches ? "YES" : "NO");
        
    } else if (strcmp(source_id, "game_capture") == 0) {
        // Game Capture - check if current window matches the captured executable
        const char* target_window = obs_data_get_string(settings, "window");
        const char* capture_mode = obs_data_get_string(settings, "capture_mode");
        
        // Extract executable name from OBS format "title:class:exe"
        std::string target(target_window);
        size_t last_colon = target.rfind(':');
        std::string target_exe;
        if (last_colon != std::string::npos) {
            target_exe = target.substr(last_colon + 1);
        }
        
        // Get current process executable name
        DWORD process_id;
        GetWindowThreadProcessId(current_hwnd, &process_id);
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id);
        char exe_path[MAX_PATH] = {0};
        if (hProcess) {
            DWORD size = MAX_PATH;
            QueryFullProcessImageNameA(hProcess, 0, exe_path, &size);
            CloseHandle(hProcess);
        }
        
        // Extract just the executable name from full path
        std::string current_exe(exe_path);
        size_t last_slash = current_exe.rfind('\\');
        if (last_slash != std::string::npos) {
            current_exe = current_exe.substr(last_slash + 1);
        }
        
        // Case-insensitive comparison
        std::transform(target_exe.begin(), target_exe.end(), target_exe.begin(), ::tolower);
        std::transform(current_exe.begin(), current_exe.end(), current_exe.begin(), ::tolower);
        
        matches = (target_exe == current_exe);
        blog(LOG_DEBUG, "[SOURCE-FILTER] Game capture: target='%s', current='%s', match=%s",
             target_exe.c_str(), current_exe.c_str(), matches ? "YES" : "NO");
    }
    
    obs_data_release(settings);
    obs_source_release(source);
    
    return matches;
}

bool should_capture_input(keystroke_source* context)
{
    // If area-based capture is disabled, always capture
    if (!context->capture_area_only) {
        return true;
    }
    
    // Get foreground window
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) {
        blog(LOG_WARNING, "[FILTER] No foreground window");
        return false;
    }
    
    // Get window title (needed for both modes)
    char window_title[256] = {0};
    GetWindowTextA(hwnd, window_title, sizeof(window_title));
    
    // Check which filtering mode to use
    if (context->use_source_capture) {
        // OBS Source-based filtering
        static bool logged_mode_once = false;
        if (!logged_mode_once) {
            blog(LOG_INFO, "[FILTER] Using OBS source capture mode: '%s'", 
                 context->capture_source_name.c_str());
            logged_mode_once = true;
        }
        
        if (context->capture_source_name.empty()) {
            blog(LOG_WARNING, "[FILTER] Source capture enabled but no source specified");
            return false;
        }
        
        bool matches = matches_obs_source_target(context, hwnd, window_title);
        
        // Log window changes (not every keystroke)
        static std::string last_window_title;
        static bool last_match_result = false;
        
        if (std::string(window_title) != last_window_title || matches != last_match_result) {
            blog(LOG_INFO, "[FILTER] Window: '%s' | Source: '%s' | Match: %s", 
                 window_title, context->capture_source_name.c_str(), matches ? "YES" : "NO");
            last_window_title = window_title;
            last_match_result = matches;
        }
        
        return matches;
        
    } else {
        // Traditional window title filtering
        static bool logged_mode_once = false;
        if (!logged_mode_once) {
            blog(LOG_INFO, "[FILTER] Using window title filtering mode");
            logged_mode_once = true;
        }
        
        // If no target window specified, capture from all windows when enabled
        if (context->target_window.empty()) {
            static bool logged_once = false;
            if (!logged_once) {
                blog(LOG_INFO, "[FILTER] Window filtering enabled but no target specified - capturing all windows");
                logged_once = true;
            }
            return true;
        }
        
        // Check if window title contains target string (case-insensitive partial match)
        std::string title(window_title);
        std::string target(context->target_window);
        
        // Convert to lowercase for comparison
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        
        // Return true if target is found in window title
        bool matches = title.find(target) != std::string::npos;
        
        // Log window changes (not every keystroke)
        static std::string last_window_title;
        static bool last_match_result = false;
        
        if (title != last_window_title || matches != last_match_result) {
            blog(LOG_INFO, "[FILTER] Window: '%s' | Target: '%s' | Match: %s", 
                 window_title, context->target_window.c_str(), matches ? "YES" : "NO");
            last_window_title = title;
            last_match_result = matches;
        }
        
        return matches;
    }
}

bool is_password_field_active()
{
    // Get foreground window
    HWND hwnd = GetForegroundWindow();
    if (!hwnd)
        return false;
    
    // Get focused control
    HWND focused = GetFocus();
    if (!focused)
        return false;
    
    // Check window class for password indicators
    char class_name[256] = {0};
    GetClassNameA(focused, class_name, sizeof(class_name));
    
    // Check for edit control
    if (strcmp(class_name, "Edit") == 0) {
        // Check for password style
        LONG style = GetWindowLong(focused, GWL_STYLE);
        if (style & ES_PASSWORD) {
            return true;
        }
    }
    
    // Additional checks for common password fields
    char window_text[256] = {0};
    GetWindowTextA(hwnd, window_text, sizeof(window_text));
    std::string title(window_text);
    
    // Simple heuristic: check for password-related keywords
    std::transform(title.begin(), title.end(), title.begin(), ::tolower);
    if (title.find("password") != std::string::npos ||
        title.find("login") != std::string::npos ||
        title.find("sign in") != std::string::npos) {
        return true;
    }
    
    return false;
}

LRESULT CALLBACK keyboard_hook_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && g_context) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
        
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            int vk_code = kbd->vkCode;
            
            blog(LOG_DEBUG, "[INPUT] Key pressed: VK=%d", vk_code);
            
            // Check if we should capture based on window filter
            if (!should_capture_input(g_context)) {
                blog(LOG_DEBUG, "[INPUT] Not in target window, ignoring");
                return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);
            }
            
            // Check if we're in a password field
            if (is_password_field_active()) {
                blog(LOG_DEBUG, "[INPUT] Password field detected, ignoring keystroke");
                return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);
            }
            
            // Check if this is a modifier key alone
            if (is_modifier_key(vk_code)) {
                if (g_context->ignore_modifier_keys_alone) {
                    g_pressed_keys.insert(vk_code);
                    blog(LOG_DEBUG, "[INPUT] Modifier key alone, ignoring (setting enabled)");
                    return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);
                }
            }
            
            // Avoid duplicate events
            if (g_pressed_keys.count(vk_code)) {
                blog(LOG_DEBUG, "[INPUT] Duplicate key press, ignoring");
                return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);
            }
            g_pressed_keys.insert(vk_code);
            
            // Get modifiers
            std::string modifiers = get_current_modifiers();
            
            // Get key name
            bool shift_pressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
            std::string key_name = get_key_name(vk_code, shift_pressed);
            
            if (!key_name.empty()) {
                std::string full_key = modifiers + key_name;
                blog(LOG_INFO, "[INPUT] Adding keystroke: '%s'", full_key.c_str());
                add_keystroke(g_context, full_key);
            } else {
                blog(LOG_DEBUG, "[INPUT] Key name empty for VK=%d", vk_code);
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            g_pressed_keys.erase(kbd->vkCode);
        }
    }
    
    return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK mouse_hook_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && g_context && g_context->show_mouse_clicks) {
        // Check if we should capture based on window filter
        if (!should_capture_input(g_context)) {
            return CallNextHookEx(g_mouse_hook, nCode, wParam, lParam);
        }
        
        std::string mouse_action;
        
        switch (wParam) {
            case WM_LBUTTONDOWN:
                mouse_action = "Left Click";
                break;
            case WM_RBUTTONDOWN:
                mouse_action = "Right Click";
                break;
            case WM_MBUTTONDOWN:
                mouse_action = "Middle Click";
                break;
            case WM_XBUTTONDOWN:
                mouse_action = "Mouse Button";
                break;
            case WM_MOUSEWHEEL: {
                MSLLHOOKSTRUCT* mouse = (MSLLHOOKSTRUCT*)lParam;
                short delta = GET_WHEEL_DELTA_WPARAM(mouse->mouseData);
                if (delta > 0) {
                    mouse_action = "Scroll Up";
                } else if (delta < 0) {
                    mouse_action = "Scroll Down";
                }
                break;
            }
        }
        
        if (!mouse_action.empty()) {
            // Check for modifiers
            std::string modifiers = get_current_modifiers();
            blog(LOG_INFO, "[INPUT] Adding mouse action: '%s'", (modifiers + mouse_action).c_str());
            add_keystroke(g_context, modifiers + mouse_action);
        }
    }
    
    return CallNextHookEx(g_mouse_hook, nCode, wParam, lParam);
}

void start_input_capture(keystroke_source* context)
{
    if (!context)
        return;
    
    g_context = context;
    
    if (!g_keyboard_hook) {
        g_keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_proc, 
                                          GetModuleHandle(nullptr), 0);
        if (g_keyboard_hook) {
            blog(LOG_INFO, "Keyboard hook installed successfully");
        } else {
            blog(LOG_ERROR, "Failed to install keyboard hook");
        }
    }
    
    if (!g_mouse_hook) {
        g_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, mouse_hook_proc,
                                       GetModuleHandle(nullptr), 0);
        if (g_mouse_hook) {
            blog(LOG_INFO, "Mouse hook installed successfully");
        } else {
            blog(LOG_ERROR, "Failed to install mouse hook");
        }
    }
    
    context->is_capturing = true;
}

void stop_input_capture(keystroke_source* context)
{
    if (!context)
        return;
    
    if (g_keyboard_hook) {
        UnhookWindowsHookEx(g_keyboard_hook);
        g_keyboard_hook = nullptr;
        blog(LOG_INFO, "Keyboard hook removed");
    }
    
    if (g_mouse_hook) {
        UnhookWindowsHookEx(g_mouse_hook);
        g_mouse_hook = nullptr;
        blog(LOG_INFO, "Mouse hook removed");
    }
    
    g_context = nullptr;
    context->is_capturing = false;
    g_pressed_keys.clear();
}

#else
// Non-Windows platforms (stub implementation)
void start_input_capture(keystroke_source* context)
{
    blog(LOG_WARNING, "Input capture not implemented for this platform");
}

void stop_input_capture(keystroke_source* context)
{
}

std::string get_key_name(int vk_code, bool shift_pressed)
{
    return "";
}

bool is_password_field_active()
{
    return false;
}

bool is_modifier_key(int vk_code)
{
    return false;
}
#endif
