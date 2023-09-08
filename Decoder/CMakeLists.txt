# ICST Ambisonics Decoder CMakeLists.txt

juce_add_plugin(AmbiDecoder
    ${COMMON_PLUGIN_SETTINGS}

	PLUGIN_CODE "ADXF"                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
	PLUGIN_NAME "AmbiDecoder"
	BUNDLE_ID "com.icst.ambidecoder"
    PRODUCT_NAME "AmbiDecoder_64ch"
	DESCRIPTION "1st-7th Order Ambisonic, 64 Channel Output"
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
	Source/SpeakerTestCustomComponent.cpp
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
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
		JUCE_MODAL_LOOPS_PERMITTED=1
		MAX_NUM_INPUT=64
		)

target_link_libraries(AmbiDecoder
    ${COMMON_LINK_LIBRARIES}
    )

set_target_properties(AmbiDecoder
    ${COMMON_OUTPUT_DIRECTORY}
	)