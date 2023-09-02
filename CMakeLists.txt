# ICST Ambisonics Plugins CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()

project(ICST_AMBISONICS_PLUGINS 
    VERSION 0.0.1
)

set( COMMON_OUTPUT_DIRECTORY
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set(COMMON_PLUGIN_SETTINGS
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "ICST"                          
	COMPANY_WEBSITE "https://www.zhdk.ch/forschung/icst"
	COMPANY_COPYRIGHT "© 2023"
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE "ICST"               # A four-character manufacturer id with at least one upper-case character
    FORMATS AU VST3 Standalone LV2                 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    AU_MAIN_TYPE kAudioUnitType_Effect
)

set(COMMON_LINK_LIBRARIES
    PRIVATE
    BinaryData
    juce::juce_audio_utils
    juce::juce_osc
    juce::juce_opengl
    juce::juce_gui_extra
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_data_structures
    juce::juce_audio_processors
    juce::juce_audio_plugin_client
    juce::juce_audio_basics
    juce::juce_dsp
    juce::juce_audio_formats

    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
add_subdirectory(JUCE)                    

juce_add_binary_data(BinaryData SOURCES 
	Resources/speaker.png
)

add_subdirectory(Encoder)
add_subdirectory(Decoder)
