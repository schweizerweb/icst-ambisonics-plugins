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
	PLUGIN_NAME "ICST Ambisonics Decoder"
	BUNDLE_ID "com.icst.ambidecoder"
    FORMATS AU VST3 Standalone LV2                 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "ICST_AmbiDecoder_64ch"
	DESCRIPTION "1st-7th Order Ambisonic, 64 Channel Output"
	AU_MAIN_TYPE kAudioUnitType_Effect
	AU_EXPORT_PREFIX "AmbiDecoderAU"
)

juce_generate_juce_header(AmbiDecoder)

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

target_compile_definitions(AmbiDecoder
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
		JUCE_MODAL_LOOPS_PERMITTED=1
		MAX_NUM_INPUT=64
		)

juce_add_binary_data(BinaryData SOURCES 
	../Resources/speaker.png
)

target_link_libraries(AmbiDecoder
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
