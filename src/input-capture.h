#pragma once

#include "keystroke-source.h"
#include <string>

// Windows-specific input capture using low-level keyboard and mouse hooks
#ifdef _WIN32
#include <Windows.h>
#endif

// Input capture management
void start_input_capture(keystroke_source* context);
void stop_input_capture(keystroke_source* context);

// Helper functions
std::string get_key_name(int vk_code, bool shift_pressed);
bool is_password_field_active();
bool is_modifier_key(int vk_code);
