/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
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

//==============================================================================
/**
*/
class AmbisonicEncoderAudioProcessor  : public AudioProcessor, ActionListener
{
public:
    //==============================================================================
    AmbisonicEncoderAudioProcessor();
    ~AmbisonicEncoderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

	void applyDistanceGain(double* pCoefficientArray, int arraySize, double distance) const;
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

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
	double lastCoefficients[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumOutputChannels];
	VarDelayBuffer delayBuffers[JucePlugin_MaxNumInputChannels];
    AirAbsorbtionFilter airAbsorbtionFilters[JucePlugin_MaxNumInputChannels];
    dsp::ProcessSpec iirFilterSpec;
    ScalingInfo scalingInfo;
    std::unique_ptr<ZoomSettings> zoomSettings;
    double lastScaler;
    AnimatorDataset animatorDataset;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessor)
};
