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

		createPreset("Dolby 7.0", {
			{ -0.5, cos30, 0, "L"},
			{ 0.5, cos30, 0, "R"},
			{ 0, 1, 0, "C"},
			{ -1, 0, 0, "SSL"},
			{ 1, 0, 0, "SSR"},
			{ -0.642788, -0.766044, 0, "SBL"},
			{ sin45, -sin45, 0, "SBR"},
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
			{ -1.74474, 4.31839, 0, "1"},
			{ 1.78236, 4.303, 0, "2"},
			{ 3.9866, 1.65131, 0, "3"},
			{ 4.02457, -1.66703, 0, "4"},
			{ 1.81906, -4.39159, 0, "5"},
			{ -1.84527, -4.45487, 0, "6"},
			{ -3.91067, -1.61985, 0, "7"},
			{ -4.01192, 1.66179, 0, "8"},
			{ -2.00185, 1.95353, 3.32164, "9"},
			{ 2.19268, 2.00922, 3.34973, "10"},
			{ 2.29422, -1.98032, 3.35414, "11"},
			{ -1.91004, -2.05545, 3.33212, "12"},
			{ 0.23209, -0.09283, 4.33527, "13"},
			{ 4.13814, 4.36069, -1.23401, "14"},
			{ 4.25756, -4.33253, -1.269, "15"},
			{ -3.94811, -4.50989, -1.25219, "16"},
			{ -3.95405, 4.26999, -1.21577, "17"},
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
        
        pSpeakerSet->loadFromXml(rootElement.get());
        pAmbiSettings->loadFromPresetXml(rootElement->getChildByName("General"));

        return pSpeakerSet->size() > 0;
    }
    
    bool writeToXmlFile(const File file, AmbiSpeakerSet* pSpeakerSet, AmbiSettings* pAmbiSettings)
    {
        XmlElement* rootElement = new XmlElement("AmbisonicsPreset");

        pSpeakerSet->writeToXmlElement(rootElement);
        
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
