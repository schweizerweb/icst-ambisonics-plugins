/*
  ==============================================================================

    DecoderPresetHelper.h
    Created: 26 Jan 2020 9:49:43pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "../../Common/PresetHelper.h"
#include "../../Common/TrackColors.h"

#define DEFAULT_PRESET_NAME "Stereo (2)"

class DecoderPresetHelper : public PresetHelper
{
public:
    DecoderPresetHelper(File presetDirectory, ActionListener* pActionListener) : PresetHelper(presetDirectory, pActionListener)
    {
    }

    struct SSpeaker
    {
        double x;
        double y;
        double z;
        String name;
        bool isSubwoofer = false;
    };

    bool checkValid(juce::File presetFile) override
    {
        AmbiSpeakerSet testSet;
        AmbiSettings testSettings;
        if(loadFromXmlFile(presetFile, &testSet, &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        const double cos30 = 0.8660253882408143;
        const double sin45 = 0.7071067811865475;
        createPreset(DEFAULT_PRESET_NAME, 2);
        
		createPreset("11.1", {
	        { -0.5, 0.866025, 0, "L"},
	        { 0.5, 0.866025, 0, "R"},
	        { 0, 1, 0, "C"},
	        { 0.25, 0.933013, -0.258819, "LFE"},
	        { -0.939693, -0.34202, 0, "SL"},
	        { 0.939693, -0.34202, 0, "SR"},
	        { 0, 0, 1, "T"},
	        { -0.433013, 0.75, 0.5, "HFL"},
	        { 0, 0.866025, 0.5, "HFC"},
	        { 0.433013, 0.75, 0.5, "HFR"},
	        { -0.813798, -0.296198, 0.5, "HBL"},
	        { 0.813798, -0.296198, 0.5, "HBR"},
		    });

		createPreset("13.1", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE"},
			{ -0.5, -0.866025, 0, "SBL"},
			{ 0.5, -0.866025, 0, "SBR"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, 0, 1, "T"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0, 0.866025, 0.5, "HFC"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("3D-Hexagon", {
			{ -0.5, 0.866025, 0, "1"},
			{ 0.5, 0.866025, 0, "2"},
			{ 1, 0, 0, "3"},
			{ 0.5, -0.866025, 0, "4"},
			{ -0.5, -0.866025, 0, "5"},
			{ -1, 0, 0, "6"},
			{ 0, 0.707107, 0.707107, "7"},
			{ 0.612372, 0.353553, 0.707107, "8"},
			{ 0.612372, -0.353553, 0.707107, "9"},
			{ 0, -0.707107, 0.707107, "10"},
			{ -0.612372, -0.353553, 0.707107, "11"},
			{ -0.612372, 0.353553, 0.707107, "12"},
			});

		createPreset("4.0", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ -0.829399, -0.301877, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("5.0 Film", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0, 1, 0, "C"},
			{ 0.5, 0.866025, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("5.0 ITU", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("5.0.2", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, 0.707107, "HFL"},
			{ 0.696364, 0.122788, 0.707107, "HFR"},
			});

		createPreset("5.0.4", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, 0.707107, "HFL"},
			{ 0.696364, 0.122788, 0.707107, "HFR"},
			{ -0.664463, -0.241845, 0.707107, "HBL"},
			{ 0.664463, -0.241845, 0.707107, "HBR"},
			});

		createPreset("5.1 Film", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0, 1, 0, "C"},
			{ 0.5, 0.866025, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0.258819, 0.965926, 0, "LFE", true},
			});

		createPreset("5.1 ITU", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("5.1.2", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, 0.707107, "LC"},
			{ 0.696364, 0.122788, 0.707107, "RC"},
			});

		createPreset("5.1.4", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.5, 0.5, 0.707107, "HFL"},
			{ 0.5, 0.5, 0.707107, "HFR"},
			{ -0.5, -0.5, 0.707107, "HBL"},
			{ 0.5, -0.5, 0.707107, "HBR"},
			});

		createPreset("6.0", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, -1, 0, "BC"},
			});

		createPreset("6.1", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, -1, 0, "BC"},
			});

		createPreset("7.0.2", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -0.707107, -0.707107, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			{ -0.696364, 0.122788, 0.707107, "TL"},
			{ 0.696364, 0.122788, 0.707107, "TR"},
			});

		createPreset("7.0.4", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -0.707107, -0.707107, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			{ -0.353553, 0.612372, 0.707107, "HFL"},
			{ 0.353553, 0.612372, 0.707107, "HFR"},
			{ -0.5, -0.5, 0.707107, "HBL"},
			{ 0.5, -0.5, 0.707107, "HBR"},
			});

		createPreset("7.0", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.642788, -0.766044, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			});

		createPreset("7.1.2", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, 0.707107, "TL"},
			{ 0.696364, 0.122788, 0.707107, "TR"},
			});

		createPreset("7.1.4", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.5, -0.866025, 0, "SBL"},
			{ 0.5, -0.866025, 0, "SBR"},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.5, 0.5, 0.707107, "HFL"},
			{ 0.5, 0.5, 0.707107, "HFC"},
			{ -0.5, -0.5, 0.707107, "HRL"},
			{ 0.5, -0.5, 0.707107, "HBR"},
			});

		createPreset("7.1", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.642788, -0.766044, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			});

		createPreset("8.0", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("9.1.2", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -0.707107, -0.707107, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			{ -0.866025, 0.5, 0, "SSL"},
			{ 0.866025, 0.5, 0, "SSR"},
			{ -0.696364, 0.122788, 0.707107, "TL"},
			{ 0.696364, 0.122788, 0.707107, "TR"},
			});

		createPreset("9.1.6", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.707107, -0.707107, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -0.866025, 0.5, 0, "SSL"},
			{ 0.866025, 0.5, 0, "SSR"},
			{ -0.5, 0.5, 0.707107, "HFL"},
			{ 0.5, 0.5, 0.707107, "HFR"},
			{ -0.5, -0.5, 0.707107, "HRL"},
			{ 0.5, -0.5, 0.707107, "HRR"},
			{ -0.707107, 0, 0.707107, "HSL"},
			{ 0.707107, 0, 0.707107, "HSR"},
			});

		createPreset("9.1", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("Cube A", {
			{ -0.707107, 0.707107, 0, "1"},
			{ 0.707107, 0.707107, 0, "2"},
			{ 0.707107, -0.707107, 0, "3"},
			{ -0.707107, -0.707107, 0, "4"},
			{ -0.5, 0.5, 0.707107, "5"},
			{ 0.5, 0.5, 0.707107, "6"},
			{ 0.5, -0.5, 0.707107, "7"},
			{ -0.5, -0.5, 0.707107, "8"},
			});

		createPreset("Cube-B", {
			{ -0.707107, 0.707107, 0, "1"},
			{ 0.707107, 0.707107, 0, "2"},
			{ 0.707107, -0.707107, 0, "3"},
			{ -0.707107, -0.707107, 0, "4"},
			{ 0, 0.707107, 0.707107, "5"},
			{ 0.707107, 0, 0.707107, "6"},
			{ 0, -0.707107, 0.707107, "7"},
			{ -0.707107, 0, 0.707107, "8"},
			});

		createPreset("Dodecagon", {
			{ -0.5, 0.866025, 0, "1"},
			{ 0, 1, 0, "2"},
			{ 0.5, 0.866025, 0, "3"},
			{ 0.866025, 0.5, 0, "4"},
			{ 1, 0, 0, "5"},
			{ 0.866025, -0.5, 0, "6"},
			{ 0.5, -0.866025, 0, "7"},
			{ 0, -1, 0, "8"},
			{ -0.5, -0.866025, 0, "9"},
			{ -0.866025, -0.5, 0, "10"},
			{ -1, 0, 0, "11"},
			{ -0.866025, 0.5, 0, "12"},
			});

		createPreset("Dome", {
			{ -0.382683, 0.92388, 0, "1"},
			{ 0.382683, 0.92388, 0, "2"},
			{ 0.92388, 0.382683, 0, "3"},
			{ 0.92388, -0.382683, 0, "4"},
			{ 0.382683, -0.92388, 0, "5"},
			{ -0.382683, -0.92388, 0, "6"},
			{ -0.92388, -0.382683, 0, "7"},
			{ -0.92388, 0.382683, 0, "8"},
			{ 0, 0.667579, 0.707107, "9"},
			{ 0.5, 0.5, 0.707107, "10"},
			{ 0.707107, 0, 0.707107, "11"},
			{ 0.5, -0.5, 0.707107, "12"},
			{ 0, -0.707107, 0.707107, "13"},
			{ -0.5, -0.5, 0.707107, "14"},
			{ -0.707107, 0, 0.707107, "15"},
			{ -0.5, 0.5, 0.707107, "16"},
			{ 0, 0, 1, "T"},
			});

		createPreset("Hexadecagon (16)", {
			{ -0.19509, 0.980785, 0, "1"},
			{ 0.19509, 0.980785, 0, "2"},
			{ 0.55557, 0.83147, 0, "3"},
			{ 0.83147, 0.55557, 0, "4"},
			{ 0.980785, 0.19509, 0, "5"},
			{ 0.980785, -0.19509, 0, "6"},
			{ 0.831469, -0.55557, 0, "7"},
			{ 0.55557, -0.83147, 0, "8"},
			{ 0.19509, -0.980785, 0, "9"},
			{ -0.195091, -0.980785, 0, "10"},
			{ -0.55557, -0.831469, 0, "11"},
			{ -0.83147, -0.55557, 0, "12"},
			{ -0.980785, -0.19509, 0, "13"},
			{ -0.980785, 0.195091, 0, "14"},
			{ -0.831469, 0.55557, 0, "15"},
			{ -0.55557, 0.83147, 0, "16"},
			});

		createPreset("Hexagon (6)", {
			{ -0.5, 0.866025, 0, "1"},
			{ 0.5, 0.866025, 0, "2"},
			{ 1, 0, 0, "3"},
			{ 0.5, -0.866025, 0, "4"},
			{ -0.5, -0.866025, 0, "5"},
			{ -1, 0, 0, "6"},
			});

		createPreset("ICST Kompositionsstudio", {
			{ -0.174474, 0.431839, 0, "1"},
			{ 0.178236, 0.4303, 0, "2"},
			{ 0.39866, 0.165131, 0, "3"},
			{ 0.402457, -0.166703, 0, "4"},
			{ 0.181906, -0.439159, 0, "5"},
			{ -0.184527, -0.445487, 0, "6"},
			{ -0.391067, -0.161985, 0, "7"},
			{ -0.401192, 0.166179, 0, "8"},
			{ -0.200185, 0.195353, 0.332164, "9"},
			{ 0.219268, 0.200922, 0.334973, "10"},
			{ 0.229422, -0.198032, 0.335414, "11"},
			{ -0.191004, -0.205545, 0.333212, "12"},
			{ 0.023209, -0.009283, 0.433527, "13"},
			{ 0.413814, 0.436069, -0.123401, "14"},
			{ 0.425756, -0.433253, -0.1269, "15"},
			{ -0.394811, -0.450989, -0.125219, "16"},
			{ -0.395405, 0.426999, -0.121577, "17"},
			});

		createPreset("LRCS", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0, -1, 0, "SB"},
			});

		createPreset("Mono", {
			{ 0, 1, 0, "Mono"},
			});

		createPreset("Octagon (8)", {
			{ -0.382683, 0.92388, 0, "1"},
			{ 0.382683, 0.92388, 0, "2"},
			{ 0.92388, 0.382683, 0, "3"},
			{ 0.92388, -0.382683, 0, "4"},
			{ 0.382683, -0.92388, 0, "5"},
			{ -0.382683, -0.92388, 0, "6"},
			{ -0.92388, -0.382683, 0, "7"},
			{ -0.92388, 0.382683, 0, "8"},
			});

		createPreset("Quadro", {
			{ -0.707107, 0.707107, 0, "L"},
			{ 0.707107, 0.707107, 0, "R"},
			{ -0.707107, -0.707107, 0, "SBL"},
			{ 0.707107, -0.707107, 0, "SBR"},
			});

		createPreset("Square (4)", {
			{ -0.707107, 0.707107, 0, "1"},
			{ 0.707107, 0.707107, 0, "2"},
			{ 0.707107, -0.707107, 0, "3"},
			{ -0.707107, -0.707107, 0, "4"},
			});

		createPreset("Square (5)", {
			{ -0.707107, 0.707107, 0, "1"},
			{ 0.707107, 0.707107, 0, "2"},
			{ 0.707107, -0.707107, 0, "3"},
			{ -0.707107, -0.707107, 0, "4"},
			{ 0, 0, 1, "T"},
			});

		createPreset("Stereo-15degrees", {
			{ -0.258819, 0.965926, 0, "L"},
			{ 0.258819, 0.965926, 0, "R"},
			});

		createPreset("Stereo-30degrees", {
			{ -0.5, 0.866025, 0, "L"},
			{ 0.5, 0.866025, 0, "R"},
			});

		createPreset("Stereo-S_(90degrees)", {
			{ -1, 0, 0, "L"},
			{ 1, 0, 0, "R"},
			});

		createPreset("Stereo-W_(45degrees)", {
			{ -0.707107, 0.707107, 0, "L"},
			{ 0.707107, 0.707107, 0, "R"},
			});

        notifyPresetListChanged();
    }
    
    void loadDefaultPreset(AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        for(File f : presetFiles)
        {
            if(f.getFileNameWithoutExtension() == DEFAULT_PRESET_NAME)
            {
                loadFromXmlFile(f, pSpeakerSet, pAmbiSettings);
                break;
            }
        }
    }
    
    bool loadFromXmlFile(const File file, AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        XmlDocument doc(file);
        std::unique_ptr<XmlElement> rootElement = doc.getDocumentElementIfTagMatches("AmbisonicsPreset");

        if (rootElement == nullptr || rootElement->getTagName() != "AmbisonicsPreset")
            return false;
        
        pSpeakerSet->loadFromXml(rootElement->getChildByName("Points"));
    
        pAmbiSettings->loadFromPresetXml(rootElement->getChildByName("General"));

        return pSpeakerSet->size() > 0;
    }
    
    bool writeToXmlFile(const File file, AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        XmlElement* rootElement = new XmlElement("AmbisonicsPreset");

        XmlElement* xmlSpeakers = new XmlElement("Points");
        pSpeakerSet->writeToXmlElement(xmlSpeakers);
        rootElement->addChildElement(xmlSpeakers);
        
        XmlElement* xmlSettings = new XmlElement("General");
        pAmbiSettings->writeToPresetXmlElement(xmlSettings);
        rootElement->addChildElement(xmlSettings);

        bool success = rootElement->writeTo(file);
        delete rootElement;
        return success;
    }
    
    AmbiSettings getDefaultAmbiSettings()
    {
        AmbiSettings settings;
        
        settings.setDistanceScaler(DEFAULT_DISTANCE_SCALER);
        settings.setWeightMode(AmbiSettings::INPHASE);
            
        return settings;
    }
    
    void createPreset(String name, int numberOfSpeakers)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        AmbiSpeakerSet speakerSet;
        
        File file = getPathForPresetName(name);
        
        if(numberOfSpeakers == 2)
        {
            speakerSet.addNew(Uuid().toString(), Point3D<double>(-1.0, 0.0, 0.0), "L", TrackColors::getSpeakerColor());
            speakerSet.addNew(Uuid().toString(), Point3D<double>(1.0, 0.0, 0.0), "R", TrackColors::getSpeakerColor());
        }
        else
        {
            Point<float> projectedPoint(0.0, 1.0);
            projectedPoint = projectedPoint.rotatedAboutOrigin(float(PI / numberOfSpeakers));
            for (int i = 0; i < numberOfSpeakers; i++)
            {
                speakerSet.addNew(Uuid().toString(), Point3D<double>(projectedPoint.getX(), projectedPoint.getY(), 0.0), String(i + 1), TrackColors::getSpeakerColor());
                projectedPoint = projectedPoint.rotatedAboutOrigin(-float(PI * 2 / numberOfSpeakers));
            }
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }

    void createPreset(String name, std::initializer_list<SSpeaker> speakerList)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        AmbiSpeakerSet speakerSet;

        File file = getPathForPresetName(name);

        for (const SSpeaker speaker : speakerList)
        {
            speakerSet.addNew(Uuid().toString(), Point3D<double>(speaker.x, speaker.y, speaker.z), speaker.name, TrackColors::getSpeakerColor());
            if(speaker.isSubwoofer)
            {
				auto s = speakerSet.get(speakerSet.size() - 1);
				auto f = s->getFilterInfo()->get(0);
				s->setFilterBypass(false);
				s->setGain(0.251188643150958, false);
                f->filterType = FilterInfo::LowPass;
                f->cutOffFrequencyHz = 50;
                f->qValue = 0.8165930509567261;
            }
        }
        
        writeToXmlFile(file, &speakerSet, &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }
};
