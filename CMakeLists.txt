cmake_minimum_required(VERSION 3.16...3.26)

project(keystroke-history VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set paths for OBS
if(NOT DEFINED LIBOBS_INCLUDE_DIR)
    message(FATAL_ERROR "LIBOBS_INCLUDE_DIR not set. Please specify with -DLIBOBS_INCLUDE_DIR=<path>")
endif()

if(NOT DEFINED OBS_INSTALL_DIR)
    set(OBS_INSTALL_DIR "C:/Program Files/obs-studio")
endif()

# Include directories
include_directories(
    ${LIBOBS_INCLUDE_DIR}
    ${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api
    ${LIBOBS_INCLUDE_DIR}/../build/config
    C:/obs-studio/build/config
    C:/obs-studio/frontend/api
)

# Library directories
link_directories(
    ${OBS_INSTALL_DIR}/bin/64bit
    C:/obs-studio/build/libobs/Release
)

# Plugin sources
set(keystroke-history_SOURCES
    src/plugin-main.cpp
    src/keystroke-source.cpp
    src/input-capture.cpp
    src/text-renderer.cpp
)

set(keystroke-history_HEADERS
    src/plugin-main.h
    src/keystroke-source.h
    src/input-capture.h
    src/text-renderer.h
)

# Create the plugin module
add_library(keystroke-history MODULE
    ${keystroke-history_SOURCES}
    ${keystroke-history_HEADERS}
)

# Link libraries
target_link_libraries(keystroke-history
    obs.lib
    gdi32.lib
)

# Set output directory
set_target_properties(keystroke-history PROPERTIES
    FOLDER "plugins"
    PREFIX ""
    OUTPUT_NAME "keystroke-history"
)

# Installation
if(WIN32)
    install(TARGETS keystroke-history
        LIBRARY DESTINATION "obs-plugins/64bit"
        RUNTIME DESTINATION "obs-plugins/64bit"
    )
    install(DIRECTORY data/
        DESTINATION "data/obs-plugins/keystroke-history"
    )
endif()
