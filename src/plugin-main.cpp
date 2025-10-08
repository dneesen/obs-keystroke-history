#include "plugin-main.h"
#include "keystroke-source.h"
#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

const char* plugin_name = "Keystroke History";
const char* plugin_version = PLUGIN_VERSION;

bool obs_module_load(void)
{
    blog(LOG_INFO, "Plugin loaded successfully (version %s)", plugin_version);
    
    // Register the keystroke history source
    keystroke_source_register();
    
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "Plugin unloaded");
}

MODULE_EXPORT const char* obs_module_description(void)
{
    return "Displays keystroke and mouse click history as an overlay";
}

MODULE_EXPORT const char* obs_module_name(void)
{
    return plugin_name;
}
