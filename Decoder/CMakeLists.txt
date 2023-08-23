# ICST Ambisonics Decoder CMakeLists.txt

juce_add_plugin(AmbiDecoder
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
    PLUGIN_CODE "ADXF"                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
	PLUGIN_NAME "AmbiDecoder"
	BUNDLE_ID "com.icst.ambidecoder"
    FORMATS AU VST3 Standalone                 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "ICST Ambisonics Decoder"
	DESCRIPTION "1st-7th Order Ambisonic, 64 Channel Output"
	AU_MAIN_TYPE kAudioUnitType_Effect
	AU_EXPORT_PREFIX "AmbiDecoderAU"
)

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(AmbiDecoder)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(AmbiDecoder
    PRIVATE
	Source/ScalingComponent.cpp
	Source/OSCHandlerDecoder.cpp
	Source/IIRFilterGraph.cpp
	Source/FilterSettingsComponent.cpp
	Source/SingleFilterSettingsComponent.cpp
	Source/DecoderSettings.cpp
	Source/SpeakerSettingsComponent.cpp
	Source/PluginProcessor.cpp
	Source/PluginEditor.cpp

	../Common/HelpComponent.cpp
	../Common/HelpOscSyntax.cpp
	../Common/HelpRadarInteraction.cpp
	../Common/HelpWebBrowserComponent.cpp
	../Common/AmbiDataSet.cpp
	../Common/AmbiGroup.cpp
	../Common/AmbiPoint.cpp
	../Common/AmbiSettings.cpp
	../Common/AmbiSource.cpp
	../Common/AmbiSourceSet.cpp
	../Common/AmbiSpeaker.cpp
	../Common/AmbiSpeakerSet.cpp
	../Common/ChannelLayout.cpp
	../Common/Constants.cpp
	../Common/DelayBuffer.cpp
	../Common/DelayHelper.cpp
	../Common/EncoderConstants.cpp
	../Common/FFTAnalyzer.cpp
	../Common/LabelCreator.cpp
	../Common/MultiSliderControl.cpp
	../Common/OSCHandler.cpp
	../Common/PointInfoControl.cpp
	../Common/PointSelection.cpp
	../Common/PresetHelper.cpp
	../Common/PresetManagerComponent.cpp
	../Common/Radar2D.cpp
	../Common/Radar3D.cpp
	../Common/RadarComponent.cpp
	../Common/RadarOptions.cpp
	../Common/ScalingInfo.cpp
	../Common/SimpleGraph.cpp
	../Common/StatusMessageHandler.cpp
	../Common/TestSoundGenerator.cpp
	../Common/ZoomSettings.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(AmbiDecoder
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
		JUCE_MODAL_LOOPS_PERMITTED=1
		MAX_NUM_INPUT=64
		)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(AudioPluginData SOURCES ...)
juce_add_binary_data(BinaryData SOURCES 
	../Resources/speaker.png
)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(AmbiDecoder
    PRIVATE
        BinaryData

        juce::juce_audio_utils
        juce::juce_osc
        juce::juce_opengl
        juce::juce_gui_extra
        juce::juce_gui_basics
        juce::juce_graphics
        juce::juce_events
        juce::juce_data_structures
        juce::juce_core
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
