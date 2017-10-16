/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/AmbiSettings.h"

#define NO_TEST_SOUND	-1
//==============================================================================
/**
*/
class AmbisonicsDecoderAudioProcessor  : public AudioProcessor, public Timer, public ActionListener
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

	void timerCallback() override;
	void startSpeakerTest(int channelNb);
	
	void actionListenerCallback(const String& message) override;

	// ambsonic specific
	Array<AmbiPoint>* getSpeakerArray() const;
	Array<AmbiPoint>* getMovingPointsArray() const;
	AmbiSettings* getAmbiSettings() const;

private:
	ScopedPointer<Array<AmbiPoint>> pSpeakerArray;
	ScopedPointer<Array<AmbiPoint>> pMovingPointsArray;
	ScopedPointer<AmbiSettings> pAmbiSettings;
	int testSoundChannel;
	Random random;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicsDecoderAudioProcessor)
};
