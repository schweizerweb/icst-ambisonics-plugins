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
	Source/AmbiSettingsComponent.cpp
	Source/AmbiSettingsComponent.h
	Source/CheckBoxFilterCustomComponent.h
	Source/DecoderSectionControlComponent.cpp
	Source/DecoderSectionControlComponent.h
	Source/DecoderSettings.cpp
	Source/DecoderSettings.h
	Source/DecodingPresetHelper.h
	Source/FilterPresetHelper.h
	Source/FilterSettingsComponent.cpp
	Source/FilterSettingsComponent.h
	Source/IIRFilterGraph.cpp
	Source/IIRFilterGraph.h
	Source/MultiDecoderComponent.cpp
	Source/MultiDecoderComponent.h
	Source/OSCHandlerDecoder.cpp
	Source/OSCHandlerDecoder.h
	Source/PluginEditor.cpp
	Source/PluginEditor.h
	Source/PluginProcessor.cpp
	Source/PluginProcessor.h
	Source/ScalingComponent.cpp
	Source/ScalingComponent.h
	Source/SingleFilterSettingsComponent.cpp
	Source/SingleFilterSettingsComponent.h
	Source/SpeakerPresetHelper.h
	Source/SpeakerSettingsComponent.cpp
	Source/SpeakerSettingsComponent.h
	Source/SpeakerSettingsDialog.h
	Source/SpeakerTestCustomComponent.cpp
	Source/SpeakerTestCustomComponent.h	

	../Common/ActivationDialog.h
	../Common/AdditionalWindow.h
	../Common/AmbiDataSet.cpp
	../Common/AmbiDataSet.h
	../Common/AmbiGroup.cpp
	../Common/AmbiGroup.h
	../Common/AmbiLookAndFeel.cpp
	../Common/AmbiLookAndFeel.h
	../Common/AmbiPoint.cpp
	../Common/AmbiPoint.h
	../Common/AmbiSettings.cpp
	../Common/AmbiSettings.h
	../Common/AmbiSettingsCollection.cpp
	../Common/AmbiSettingsCollection.h
	../Common/AmbiSettingsSection.cpp
	../Common/AmbiSettingsSection.h
	../Common/AmbiSource.cpp
	../Common/AmbiSource.h
	../Common/AmbiSourceSet.cpp
	../Common/AmbiSourceSet.h
	../Common/AmbiSpeaker.cpp
	../Common/AmbiSpeaker.h
	../Common/AmbiSpeakerSet.cpp
	../Common/AmbiSpeakerSet.h
	../Common/AudioParameterBoolAmbi.cpp
	../Common/AudioParameterBoolAmbi.h
	../Common/AudioParameterBoolAmbiAbs.h
	../Common/AudioParameterFloatAmbi.cpp
	../Common/AudioParameterFloatAmbi.h
	../Common/AudioParameterFloatAmbiAbs.h
	../Common/AudioParameterSet.h
	../Common/AudioParams.h
	../Common/BuildInfo.h
	../Common/ChannelLayout.cpp
	../Common/ChannelLayout.h
	../Common/CheckBoxCustomComponent.h
	../Common/ColorDefinition.h
	../Common/ColorEditorCustomComponent.h
	../Common/ColorSelectionComponent.h
	../Common/Constants.cpp
	../Common/Constants.h
	../Common/CsvImportExport.h
	../Common/DawParameter.h
	../Common/DelayBuffer.cpp
	../Common/DelayBuffer.h
	../Common/DelayHelper.cpp
	../Common/DelayHelper.h
	../Common/EditableCodeCustomComponent.h
	../Common/EditableTextCustomComponent.h
	../Common/EnablementCallback.h
	../Common/EncoderConstants.cpp
	../Common/EncoderConstants.h
	../Common/ExternalImportExport.h
	../Common/FFTAnalyzer.cpp
	../Common/FFTAnalyzer.h
	../Common/FilterBankInfo.h
	../Common/FilterControlCallback.h
	../Common/FilterInfo.h
	../Common/GroupPointsSelectionComponent.h
	../Common/HelpComponent.cpp
	../Common/HelpComponent.h
	../Common/HelpDialog.h
	../Common/HelpDialogManager.h
	../Common/HelpOscSyntax.cpp
	../Common/HelpOscSyntax.h
	../Common/HelpRadarInteraction.cpp
	../Common/HelpRadarInteraction.h
	../Common/HelpWebBrowserComponent.cpp
	../Common/HelpWebBrowserComponent.h
	../Common/ImportExport.h
	../Common/JsEditorComponent.cpp
	../Common/JsEditorComponent.h
	../Common/LabelCreator.cpp
	../Common/LabelCreator.h
	../Common/MathHelper.cpp
	../Common/MathHelper.h
	../Common/MultiActivationDialog.h
	../Common/MultiDecoderColors.h
	../Common/MultiSliderControl.cpp
	../Common/MultiSliderControl.h
	../Common/NumericColumnCustomComponent.h
	../Common/OSCHandler.cpp
	../Common/OSCHandler.h
	../Common/OSCLogComponent.cpp
	../Common/OSCLogComponent.h
	../Common/OSCLogDialog.h
	../Common/OSCLogDialogManager.h
	../Common/Point3D.h
	../Common/PointInfoControl.cpp
	../Common/PointInfoControl.h
	../Common/PointSelection.cpp
	../Common/PointSelection.h
	../Common/PresetComparators.h
	../Common/PresetHelper.cpp
	../Common/PresetHelper.h
	../Common/PresetManagerComponent.cpp
	../Common/PresetManagerComponent.h
	../Common/PresetManagerDialog.h
	../Common/PresetTableModel.h
	../Common/Radar2D.cpp
	../Common/Radar2D.h
	../Common/Radar3D.cpp
	../Common/Radar3D.h
	../Common/RadarColors.h
	../Common/RadarComponent.cpp
	../Common/RadarComponent.h
	../Common/RadarOptions.cpp
	../Common/RadarOptions.h
	../Common/ScalingInfo.cpp
	../Common/ScalingInfo.h
	../Common/SimpleGraph.cpp
	../Common/SimpleGraph.h
	../Common/SliderColumnCustomComponent.h
	../Common/SliderRange.h
	../Common/SoloMuteCallback.h
	../Common/SoloMuteCustomComponent.h
	../Common/SpeakerSelectionComponent.h
	../Common/StatusMessage.h
	../Common/StatusMessageHandler.cpp
	../Common/StatusMessageHandler.h
	../Common/StatusMessageReceiver.h
	../Common/TableColumnCallback.h
	../Common/TestSoundGenerator.cpp
	../Common/TestSoundGenerator.h
	../Common/TrackColors.h
	../Common/TrackColorSelector.h
	../Common/TreeViewItemHelp.h
	../Common/VarDelayBuffer.cpp
	../Common/VarDelayBuffer.h
	../Common/ZoomSettings.cpp
	../Common/ZoomSettings.h
	)

juce_add_binary_data(AmbiDecoder_BinResources
    SOURCES
	../Resources/png/animator_icon.png
	../Resources/png/settings.png
	../Resources/png/help.png
	../Resources/png/info.png
	../Resources/png/save.png	
	../Resources/png/objects_group.png
	../Resources/png/objects_ungroup.png
	../Resources/png/trash_bin.png
	../Resources/png/speaker.png
	../Resources/png/flat_arrow_up.png
	../Resources/png/flat_arrow_down.png

	../Resources/xml/osc_syntax.xml
	../Resources/xml/radar_documentation.xml
)

target_compile_definitions(AmbiDecoder
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
		JUCE_MODAL_LOOPS_PERMITTED=1
		MAX_NUM_INPUT=64
		JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
		)

target_link_libraries(AmbiDecoder
    ${COMMON_LINK_LIBRARIES} PRIVATE AmbiDecoder_BinResources
    )

set_target_properties(AmbiDecoder
    ${COMMON_OUTPUT_DIRECTORY}
	)
