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

#include "AirAbsorbtionFilter.h"
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/AmbiPoint.h"
#include "OSCHandlerEncoder.h"
#include "../../Common/DawParameter.h"
#include "AmbiOSCSender.h"
#include "AmbiOSCSenderExt.h"
#include "DistanceEncodingPresetHelper.h"
#include "../../Common/VarDelayBuffer.h"
#include "../../Common/DelayHelper.h"
#include "EncoderPresetHelper.h"
#include "../../Common/AudioParams.h"
#include "GroupAnimator.h"
#include "../../Common/ScalingInfo.h"
#include "CustomOscRxPresetHelper.h"
#include "CustomOscTxPresetHelper.h"
#include "AnimatorDataset.h"
#include "../../Common/ZoomSettings.h"
#include "../../Common/ChannelLayout.h"
#include "../../Common/RadarOptions.h"

//==============================================================================
/**
*/
class AmbisonicEncoderAudioProcessor  : public AudioProcessor, ActionListener
{
public:
    //==============================================================================
    AmbisonicEncoderAudioProcessor();
    ~AmbisonicEncoderAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   
    // channel handling
    void numChannelsChanged() override;
    
	void applyDistanceGain(double* pCoefficientArray, int arraySize, double distance) const;
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

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void actionListenerCallback(const juce::String &message) override;
    
    ChannelLayout* getChannelLayout();
	AmbiSourceSet* getSources();
	EncoderSettings* getEncoderSettings();
	void initializeOscSender();
    void initializeAudioParameter();
	AudioParams* getAudioParams();
	StatusMessageHandler* getStatusMessageHandler();
	DawParameter* getDawParameter();
    EncoderPresetHelper* getPresetHelper();
    DistanceEncodingPresetHelper* getDistanceEncodingPresetHelper();
    CustomOscRxPresetHelper* getCustomOscRxPresetHelper();
    CustomOscTxPresetHelper* getCustomOscTxPresetHelper();
    ScalingInfo* getScalingInfo();
    AnimatorDataset* getAnimatorDataset();
    ZoomSettings* getZoomSettingsPointer();
    OSCHandlerEncoder* getOscHandler();
    RadarOptions* getRadarOptions();
    
	void updateTrackProperties(const TrackProperties& properties) override;

private:
	std::unique_ptr<AmbiSourceSet> sources;
	EncoderSettings encoderSettings;
	OSCHandlerEncoder* pOscHandler;
	AmbiOSCSender* pOscSender;
	AmbiOSCSenderExt* pOscSenderExt;
	AudioParams audioParams;
	StatusMessageHandler statusMessageHandler;
	DawParameter dawParameter;
    std::unique_ptr<EncoderPresetHelper> presetHelper;
    std::unique_ptr<DistanceEncodingPresetHelper> distanceEncodingPresetHelper;
    std::unique_ptr<CustomOscTxPresetHelper> customOscTxPresetHelper;
    std::unique_ptr<CustomOscRxPresetHelper> customOscRxPresetHelper;
	double lastCoefficients[64][64];
	VarDelayBuffer delayBuffers[64];
    AirAbsorbtionFilter airAbsorbtionFilters[64];
    dsp::ProcessSpec iirFilterSpec;
    ScalingInfo scalingInfo;
    std::unique_ptr<ZoomSettings> zoomSettings;
    double lastScaler;
    AnimatorDataset animatorDataset;
    ChannelLayout channelLayout;
    RadarOptions radarOptions;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessor)
};
