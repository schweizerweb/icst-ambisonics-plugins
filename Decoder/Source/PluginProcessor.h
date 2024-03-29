/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once

#include "JuceHeader.h"
#include "../../Common/AmbiSettings.h"
#include "../../Common/TestSoundGenerator.h"
#include "DecoderSettings.h"
#include "../../Common/DelayBuffer.h"
#include "../../Common/DelayHelper.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "../../Common/AmbiSourceSet.h"
#include "DecoderPresetHelper.h"
#include "../../Common/ZoomSettings.h"
#include "../../Common/ChannelLayout.h"
#include "../../Common/RadarOptions.h"

//==============================================================================
/**
*/
class AmbisonicsDecoderAudioProcessor  : public AudioProcessor, ActionListener
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessor();
    ~AmbisonicsDecoderAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

	void checkDelayBuffers();
    void checkFilters();
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    using AudioProcessor::processBlock;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void numChannelsChanged() override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void actionListenerCallback(const String &message) override;
    ScalingInfo* getScalingInfo();
    
	// ambsonic specific
	AmbiSpeakerSet* getSpeakerSet();
	AmbiSourceSet* getMovingPoints();
	AmbiSettings* getAmbiSettings();
	DecoderSettings* getDecoderSettings();
	TestSoundGenerator* getTestSoundGenerator() const;
	dsp::ProcessSpec* getFilterSpecification();
    DecoderPresetHelper* getPresetHelper();
    ZoomSettings* getZoomSettingsPointer();
    ChannelLayout* getChannelLayout();
    RadarOptions* getRadarOptions();
    
private:
    ScalingInfo scalingInfo;
    std::unique_ptr<AmbiSpeakerSet> speakerSet;
	std::unique_ptr<AmbiSourceSet> movingPoints;
	AmbiSettings ambiSettings;
	DecoderSettings decoderSettings;
	TestSoundGenerator* pTestSoundGenerator;
	OwnedArray<DelayBuffer> delayBuffers;
	DelayHelper delayHelper;
    std::unique_ptr<DecoderPresetHelper> presetHelper;
    std::unique_ptr<ZoomSettings> zoomSettings;
    ChannelLayout channelLayout;
    RadarOptions radarOptions;
    
    dsp::IIR::Filter<float> iirFilters[MAX_NUM_CHANNELS][MAX_FILTER_COUNT];
    FilterBankInfo filterInfo[MAX_NUM_CHANNELS];
    dsp::ProcessSpec iirFilterSpec;
    

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessor)
};
