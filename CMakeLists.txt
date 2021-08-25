
cmake_minimum_required(VERSION 3.15)


project(Hyperpan VERSION 0.0.1)

# This might be specific to my computer, but Windows was not finding JUCE using the find_package() command
if(WIN32)
    # because of that, if you'd like to compile, you need to change the filepath from "/JUCE" to wherever
    # the root dir of JUCE is on your windows system
    add_subdirectory("/JUCE" JUCE)
endif(WIN32)
if(UNIX)
    find_package(JUCE CONFIG REQUIRED)
endif(UNIX)

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
        PluginEditor.cpp
        PluginProcessor.cpp)


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
