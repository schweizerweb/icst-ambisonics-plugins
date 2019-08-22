/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/AmbiPoint.h"
#include "../../Common/OSCHandler.h"
#include "AmbiOSCSender.h"
#include "AmbiOSCSenderExt.h"


//==============================================================================
/**
*/
class AmbisonicEncoderAudioProcessor  : public AudioProcessor
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

	void applyDistanceGain(double* pCoefficientArray, int arraySize, double distance);
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

	AmbiDataSet* getSources();
	EncoderSettings* getEncoderSettings();
	void initializeOsc();
	Array<AudioParameterSet>* getAudioParams();
	StatusMessageHandler* getStatusMessageHandler();

private:
	AmbiDataSet sources;
	EncoderSettings encoderSettings;
	OSCHandler* pOscHandler;
	AmbiOSCSender* pOscSender;
	AmbiOSCSenderExt* pOscSenderExt;
	Array<AudioParameterSet> audioParams;
	StatusMessageHandler statusMessageHandler;
	double lastCoefficients[JucePlugin_MaxNumInputChannels][JucePlugin_MaxNumOutputChannels];

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbisonicEncoderAudioProcessor)
};
