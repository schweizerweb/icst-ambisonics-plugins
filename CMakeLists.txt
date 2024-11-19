# ICST Ambisonics Plugins CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

if(APPLE)
    set( CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" )
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OSX deployment version")
    if(XCODE_VERSION VERSION_GREATER_EQUAL 15)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-ld_classic" CACHE STRING "Xcode 15 Linker issue")
        message("Xcode ${XCODE_VERSION} detected, using old linker style")
    endif()
endif()

project(ICST_AMBISONICS_PLUGINS 
    VERSION 0.0.0.1 #version_replacement_tag#
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
	COMPANY_COPYRIGHT "© 2024"
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE "ICST"               # A four-character manufacturer id with at least one upper-case character
    FORMATS AU VST3 LV2 Standalone                # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    AU_MAIN_TYPE kAudioUnitType_Effect
    NEEDS_WEB_BROWSER TRUE
)

set(COMMON_LINK_LIBRARIES
    PRIVATE
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

add_subdirectory(Encoder)
add_subdirectory(Decoder)
