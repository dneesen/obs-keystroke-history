#pragma once

#include <obs-module.h>
#include <obs-source.h>
#include <string>

#define PLUGIN_NAME "keystroke-history"
#define PLUGIN_VERSION "1.0.0"

// Logging macros
#define blog(log_level, format, ...) \
    blog(log_level, "[Keystroke History] " format, ##__VA_ARGS__)

// Plugin info
extern const char* plugin_name;
extern const char* plugin_version;
