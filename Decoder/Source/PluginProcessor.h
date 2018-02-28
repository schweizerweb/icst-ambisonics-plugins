/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/AmbiSettings.h"
#include "../../Common/TestSoundGenerator.h"
#include "DecoderSettings.h"
#include "../../Common/DelayBuffer.h"
#include "../../Common/DelayHelper.h"

//==============================================================================
/**
*/
class AmbisonicsDecoderAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AmbisonicsDecoderAudioProcessor();
    ~AmbisonicsDecoderAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

	void checkDelayBuffers();
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

	// ambsonic specific
	OwnedArray<AmbiPoint>* getSpeakerArray();
	OwnedArray<AmbiPoint>* getMovingPointsArray();
	AmbiSettings* getAmbiSettings() const;
	DecoderSettings* getDecoderSettings() const;
	TestSoundGenerator* getTestSoundGenerator() const;

private:
	OwnedArray<AmbiPoint> speakerArray;
	OwnedArray<AmbiPoint> movingPointsArray;
	ScopedPointer<AmbiSettings> pAmbiSettings;
	ScopedPointer<DecoderSettings> pDecoderSettings;
	ScopedPointer<TestSoundGenerator> pTestSoundGenerator;
	OwnedArray<DelayBuffer> delayBuffers;
	DelayHelper delayHelper;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessor)
};
