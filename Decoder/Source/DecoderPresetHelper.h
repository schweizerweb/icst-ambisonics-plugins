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

#define DEFAULT_PRESET_NAME "Standard - Stereo 90deg"

class DecoderPresetHelper : public PresetHelper
{
public:
    DecoderPresetHelper(File presetDirectory, ActionListener* pActionListener, ScalingInfo* pScaling) : PresetHelper(presetDirectory, pActionListener), pScalingInfo(pScaling)
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
        std::unique_ptr<AmbiSpeakerSet> testSet(new AmbiSpeakerSet(pScalingInfo));
        AmbiSettings testSettings;
        if(loadFromXmlFile(presetFile, testSet.get(), &testSettings))
        {
            return true;
        }
        
        return false;
    }
    
    void restoreDefaultsInternal() override
    {
        const double cos30 = 0.8660253882408143;
        const double sin45 = 0.7071067811865475;
        
		createPreset("Dolby Atmos 11.1", {
	        { -0.5, cos30, 0, "L"},
	        { 0.5, cos30, 0, "R"},
	        { 0, 1, 0, "C"},
	        { 0.25, 0.933013, -0.258819, "LFE", true},
	        { -0.939693, -0.34202, 0, "SL"},
	        { 0.939693, -0.34202, 0, "SR"},
	        { 0, 0, 1, "T"},
	        { -0.433013, 0.75, 0.5, "HFL"},
	        { 0, cos30, 0.5, "HFC"},
	        { 0.433013, 0.75, 0.5, "HFR"},
	        { -0.813798, -0.296198, 0.5, "HBL"},
	        { 0.813798, -0.296198, 0.5, "HBR"},
		    });

		createPreset("Dolby Atmos 13.1", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.5, -cos30, 0, "SBL"},
			{ 0.5, -cos30, 0, "SBR"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, 0, 1, "T"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0, cos30, 0.5, "HFC"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("Regular 3D - Hexagon", {
			{ -0.5, cos30, 0, "1"},
			{ 0.5, cos30, 0, "2"},
			{ 1, 0, 0, "3"},
			{ 0.5, -cos30, 0, "4"},
			{ -0.5, -cos30, 0, "5"},
			{ -1, 0, 0, "6"},
			{ 0, sin45, sin45, "7"},
			{ 0.612372, 0.353553, sin45, "8"},
			{ 0.612372, -0.353553, sin45, "9"},
			{ 0, -sin45, sin45, "10"},
			{ -0.612372, -0.353553, sin45, "11"},
			{ -0.612372, 0.353553, sin45, "12"},
			});

		createPreset("Dolby 4.0", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ -0.829399, -0.301877, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("Dolby 5.0 Film", {
			{ -0.5, cos30, 0, "L"},
			{ 0, 1, 0, "C"},
			{ 0.5, cos30, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("Dolby 5.0 ITU", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("Dolby Atmos 5.0.2", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, sin45, "HFL"},
			{ 0.696364, 0.122788, sin45, "HFR"},
			});

		createPreset("Dolby Atmos 5.0.4", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, sin45, "HFL"},
			{ 0.696364, 0.122788, sin45, "HFR"},
			{ -0.664463, -0.241845, sin45, "HBL"},
			{ 0.664463, -0.241845, sin45, "HBR"},
			});

		createPreset("Dolby 5.1 Film", {
			{ -0.5, cos30, 0, "L"},
			{ 0, 1, 0, "C"},
			{ 0.5, cos30, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0.258819, 0.965926, 0, "LFE", true},
			});

		createPreset("Dolby 5.1 ITU", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			});

		createPreset("Dolby Atmos 5.1.2", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, sin45, "LC"},
			{ 0.696364, 0.122788, sin45, "RC"},
			});

		createPreset("Dolby Atmos 5.1.4", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.5, 0.5, sin45, "HFL"},
			{ 0.5, 0.5, sin45, "HFR"},
			{ -0.5, -0.5, sin45, "HBL"},
			{ 0.5, -0.5, sin45, "HBR"},
			});

		createPreset("Dolby 6.0", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, -1, 0, "BC"},
			});

		createPreset("Dolby 6.1", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ 0, -1, 0, "BC"},
			});

		createPreset("Dolby Atmos 7.0.2", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -sin45, -sin45, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			{ -0.696364, 0.122788, sin45, "TL"},
			{ 0.696364, 0.122788, sin45, "TR"},
			});

		createPreset("Dolby Atmos 7.0.4", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -sin45, -sin45, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			{ -0.353553, 0.612372, sin45, "HFL"},
			{ 0.353553, 0.612372, sin45, "HFR"},
			{ -0.5, -0.5, sin45, "HBL"},
			{ 0.5, -0.5, sin45, "HBR"},
			});

		createPreset("Dolby 7.0", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.642788, -0.766044, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			});

		createPreset("Dolby Atmos 7.1.2", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.696364, 0.122788, sin45, "TL"},
			{ 0.696364, 0.122788, sin45, "TR"},
			});

		createPreset("Dolby Atmos 7.1.4", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.5, -cos30, 0, "SBL"},
			{ 0.5, -cos30, 0, "SBR"},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.5, 0.5, sin45, "HFL"},
			{ 0.5, 0.5, sin45, "HFC"},
			{ -0.5, -0.5, sin45, "HRL"},
			{ 0.5, -0.5, sin45, "HBR"},
			});

		createPreset("Dolby 7.1", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.642788, -0.766044, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			});

		createPreset("Dolby Atmos 8.0", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("Dolby Atmos 9.1.2", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -sin45, -sin45, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			{ -cos30, 0.5, 0, "SSL"},
			{ cos30, 0.5, 0, "SSR"},
			{ -0.696364, 0.122788, sin45, "TL"},
			{ 0.696364, 0.122788, sin45, "TR"},
			});

		createPreset("Dolby Atmos 9.1.6", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -sin45, -sin45, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			{ -1, 0, 0, "SL"},
			{ 1, 0, 0, "SR"},
			{ -cos30, 0.5, 0, "SSL"},
			{ cos30, 0.5, 0, "SSR"},
			{ -0.5, 0.5, sin45, "HFL"},
			{ 0.5, 0.5, sin45, "HFR"},
			{ -0.5, -0.5, sin45, "HRL"},
			{ 0.5, -0.5, sin45, "HRR"},
			{ -sin45, 0, sin45, "HSL"},
			{ sin45, 0, sin45, "HSR"},
			});

		createPreset("Dolby Atmos 9.1", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0.25, 0.933013, -0.258819, "LFE", true},
			{ -0.939693, -0.34202, 0, "SL"},
			{ 0.939693, -0.34202, 0, "SR"},
			{ -0.433013, 0.75, 0.5, "HFL"},
			{ 0.433013, 0.75, 0.5, "HFR"},
			{ -0.813798, -0.296198, 0.5, "HBL"},
			{ 0.813798, -0.296198, 0.5, "HBR"},
			});

		createPreset("Regular 3D - Cube A", {
			{ -sin45, sin45, 0, "1"},
			{ sin45, sin45, 0, "2"},
			{ sin45, -sin45, 0, "3"},
			{ -sin45, -sin45, 0, "4"},
			{ -0.5, 0.5, sin45, "5"},
			{ 0.5, 0.5, sin45, "6"},
			{ 0.5, -0.5, sin45, "7"},
			{ -0.5, -0.5, sin45, "8"},
			});

		createPreset("Regular 3D - Cube B", {
			{ -sin45, sin45, 0, "1"},
			{ sin45, sin45, 0, "2"},
			{ sin45, -sin45, 0, "3"},
			{ -sin45, -sin45, 0, "4"},
			{ 0, sin45, sin45, "5"},
			{ sin45, 0, sin45, "6"},
			{ 0, -sin45, sin45, "7"},
			{ -sin45, 0, sin45, "8"},
			});

		createPreset("Regular 2D - Dodecagon", {
			{ -0.5, cos30, 0, "1"},
			{ 0, 1, 0, "2"},
			{ 0.5, cos30, 0, "3"},
			{ cos30, 0.5, 0, "4"},
			{ 1, 0, 0, "5"},
			{ cos30, -0.5, 0, "6"},
			{ 0.5, -cos30, 0, "7"},
			{ 0, -1, 0, "8"},
			{ -0.5, -cos30, 0, "9"},
			{ -cos30, -0.5, 0, "10"},
			{ -1, 0, 0, "11"},
			{ -cos30, 0.5, 0, "12"},
			});

		createPreset("Regular 3D - Dome", {
			{ -0.382683, 0.92388, 0, "1"},
			{ 0.382683, 0.92388, 0, "2"},
			{ 0.92388, 0.382683, 0, "3"},
			{ 0.92388, -0.382683, 0, "4"},
			{ 0.382683, -0.92388, 0, "5"},
			{ -0.382683, -0.92388, 0, "6"},
			{ -0.92388, -0.382683, 0, "7"},
			{ -0.92388, 0.382683, 0, "8"},
			{ 0, sin45, sin45, "9"},
			{ 0.5, 0.5, sin45, "10"},
			{ sin45, 0, sin45, "11"},
			{ 0.5, -0.5, sin45, "12"},
			{ 0, -sin45, sin45, "13"},
			{ -0.5, -0.5, sin45, "14"},
			{ -sin45, 0, sin45, "15"},
			{ -0.5, 0.5, sin45, "16"},
			{ 0, 0, 1, "T"},
			});

		createPreset("Regular 2D - Hexadecagon (16)", {
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

		createPreset("Regular 2D - Hexagon (6)", {
			{ -0.5, cos30, 0, "1"},
			{ 0.5, cos30, 0, "2"},
			{ 1, 0, 0, "3"},
			{ 0.5, -cos30, 0, "4"},
			{ -0.5, -cos30, 0, "5"},
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

		createPreset("Dolby LRCS", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ 0, -1, 0, "SB"},
			});

		createPreset("Standard - Mono", {
			{ 0, 1, 0, "Mono"},
			});

		createPreset("Regular 2D - Octagon (8)", {
			{ -0.382683, 0.92388, 0, "1"},
			{ 0.382683, 0.92388, 0, "2"},
			{ 0.92388, 0.382683, 0, "3"},
			{ 0.92388, -0.382683, 0, "4"},
			{ 0.382683, -0.92388, 0, "5"},
			{ -0.382683, -0.92388, 0, "6"},
			{ -0.92388, -0.382683, 0, "7"},
			{ -0.92388, 0.382683, 0, "8"},
			});

		createPreset("Regular 2D - Quadro (4)", {
			{ -sin45, sin45, 0, "L"},
			{ sin45, sin45, 0, "R"},
			{ -sin45, -sin45, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
			});

		createPreset("Regular 2D - Square (4)", {
			{ -sin45, sin45, 0, "1"},
			{ sin45, sin45, 0, "2"},
			{ sin45, -sin45, 0, "3"},
			{ -sin45, -sin45, 0, "4"},
			});

		createPreset("Regular 3D - Square (5)", {
			{ -sin45, sin45, 0, "1"},
			{ sin45, sin45, 0, "2"},
			{ sin45, -sin45, 0, "3"},
			{ -sin45, -sin45, 0, "4"},
			{ 0, 0, 1, "T"},
			});

		createPreset("Standard - Stereo 15deg", {
			{ -0.258819, 0.965926, 0, "L"},
			{ 0.258819, 0.965926, 0, "R"},
			});

		createPreset("Standard - Stereo 30deg", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			});

		createPreset(DEFAULT_PRESET_NAME, {
			{ -1, 0, 0, "L"},
			{ 1, 0, 0, "R"},
			});

		createPreset("Standard - Stereo 45deg", {
			{ -sin45, sin45, 0, "L"},
			{ sin45, sin45, 0, "R"},
			});

		createPreset("Dolby Atmos 10.2", {
		    { -0.5, cos30, 0, "L"},
		    { 0.5, cos30, 0, "R"},
		    { 0, 1, 0, "C"},
		    { -0.939693, -0.34202, 0, "LS"},
		    { 0.939693, -0.34202, 0, "RS"},
		    { -0.819152, 0.573576, 0, "LW"},
		    { 0.819152, 0.573576, 0, "RW"},
		    { 0, -sin45, sin45, "BS"},
		    { -0.5, 0.5, sin45, "LH"},
		    { 0.5, 0.5, sin45, "RH"},
		    { -0.965926, 0, -0.258819, "LFE-L"},
		    { 0.965926, 0, -0.258819, "LFE-R"},
			});

		createPreset("Dolby Atmos 22.2", {
			{ -0.836516, 0.482963, 0.258819, "MFL"},
			{ -0.482963, 0.836516, 0.258819, "MFL/C"},
			{ 0, 0.965926, 0.258819, "MFC"},
			{ 0.482963, 0.836516, 0.258819, "MFC/R"},
			{ 0.836516, 0.482963, 0.258819, "MFR"},
			{ -0.965926, 0, 0.258819, "MSL"},
			{ 0.965926, 0, 0.258819, "MSR"},
			{ -0.620885, -0.739942, 0.258819, "MBL"},
			{ 0, -0.965926, 0.258819, "MBC"},
			{ 0.683013, -0.683013, 0.258819, "MBR"},
			{ -0.612372, 0.353553, sin45, "UFL"},
			{ 0, sin45, sin45, "UFC"},
			{ 0.612372, 0.353553, sin45, "UFR"},
			{ -sin45, 0, sin45, "USL"},
			{ 0, 0, 1, "USC"},
			{ sin45, 0, sin45, "USR"},
			{ -0.454519, -0.541675, sin45, "UBL"},
			{ 0, -sin45, sin45, "UBC"},
			{ 0.5, -0.5, sin45, "UBR"},
			{ -0.836516, 0.482963, -0.258819, "LFL"},
			{ 0, 0.965926, -0.258819, "LFC"},
			{ 0.482963, 0.836516, -0.258819, "LFR"},
			{ -0.965926, 0, -0.258819, "LFE-L"},
			{ 0.965926, 0, -0.258819, "LFE-R"},
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
    
    void createPreset(String name, std::initializer_list<SSpeaker> speakerList)
    {
        AmbiSettings defaultAmbiSettings = getDefaultAmbiSettings();
        std::unique_ptr<AmbiSpeakerSet> speakerSet(new AmbiSpeakerSet(pScalingInfo));

        File file = getPathForPresetName(name);

        for (const SSpeaker& speaker : speakerList)
        {
            speakerSet->addNew(Uuid().toString(), Point3D<double>(speaker.x, speaker.y, speaker.z), speaker.name, TrackColors::getSpeakerColor());
            if(speaker.isSubwoofer)
            {
				auto s = speakerSet->get(speakerSet->size() - 1);
				auto f = s->getFilterInfo()->get(0);
				s->setFilterBypass(false);
				s->setGain(Decibels::decibelsToGain(-6.0), false);
				s->setColor(TrackColors::getSubwooferColor());
                f->filterType = FilterInfo::LowPass;
                f->cutOffFrequencyHz = 50;
                f->qValue = 0.8165930509567261;
            }
        }
        
        writeToXmlFile(file, speakerSet.get(), &defaultAmbiSettings);
        presetFiles.addIfNotAlreadyThere(file);
    }
    
private:
    ScalingInfo* pScalingInfo;
};