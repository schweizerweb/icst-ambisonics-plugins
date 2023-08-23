# ICST Ambisonics Encoder CMakeLists.txt

juce_add_plugin(AmbiEncoderMulti
    ${COMMON_PLUGIN_SETTINGS}

	PLUGIN_CODE "AEXF"                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
	PLUGIN_NAME "AmbiEncoder"
	BUNDLE_ID "com.icst.ambiencoder_multi"
    PRODUCT_NAME "AmbiEncoder_64ch"
	DESCRIPTION "1st-7th Order Ambisonic, 1-64 Channel Input"
	AU_EXPORT_PREFIX "AmbiEncoderMultiAU"
)

juce_generate_juce_header(AmbiEncoderMulti)

target_sources(AmbiEncoderMulti
    PRIVATE
	${SRC_FILES}
    )

target_compile_definitions(AmbiEncoderMulti
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
		JUCE_MODAL_LOOPS_PERMITTED=1
		MAX_NUM_INPUT=64
		)

target_link_libraries(AmbiEncoderMulti
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
