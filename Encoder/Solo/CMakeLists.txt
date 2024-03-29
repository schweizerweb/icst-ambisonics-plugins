# ICST Ambisonics Encoder CMakeLists.txt

juce_add_plugin(AmbiEncoderSolo
    ${COMMON_PLUGIN_SETTINGS}

	PLUGIN_CODE "AEXS"                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
	PLUGIN_NAME "AmbiEncoder"
	BUNDLE_ID "com.icst.ambiencoder_solo"
    PRODUCT_NAME "AmbiEncoder_1ch"
	DESCRIPTION "1st-7th Order Ambisonic, 1 Channel Input"
	AU_EXPORT_PREFIX "AmbiEncoderSoloAU"
)

juce_generate_juce_header(AmbiEncoderSolo)

target_sources(AmbiEncoderSolo
    PRIVATE
	${SRC_FILES}
    )

target_compile_definitions(AmbiEncoderSolo
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
	JUCE_MODAL_LOOPS_PERMITTED=1
	MAX_NUM_INPUT=1
    JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
	)

target_link_libraries(AmbiEncoderSolo
    ${COMMON_LINK_LIBRARIES}
    )
    
set_target_properties( AmbiEncoderSolo
    ${COMMON_OUTPUT_DIRECTORY}
    )