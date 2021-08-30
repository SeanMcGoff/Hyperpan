cmake_minimum_required(VERSION 3.15)


project(Hyperpan VERSION 0.1.0)

# You can set the juce install path with the JUCE_INSTALL_PATH environment variable
if(DEFINED ENV{JUCE_INSTALL_PATH})
    add_subdirectory($ENV{JUCE_INSTALL_PATH} JUCE)
endif()
# Default install paths for Win/Mac/Linux
if(NOT DEFINED ENV{JUCE_INSTALL_PATH})
    if(WIN32)
        add_subdirectory("/JUCE" JUCE)
    endif(WIN32)
    if(APPLE)
        add_subdirectory("/Users/$ENV{USER}/Applications/JUCE" JUCE)
    endif(APPLE)
    if(UNIX AND NOT APPLE)
        add_subdirectory("/home/$ENV{USER}/JUCE" JUCE)
    endif(UNIX AND NOT APPLE)
endif()



# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

juce_add_plugin(Hyperpan
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "Sean McGoff"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Hpan
    FORMATS VST3                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Hyperpan")


target_sources(Hyperpan
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp)


target_compile_definitions(Hyperpan
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Hyperpan
    PRIVATE
        juce::juce_dsp
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
