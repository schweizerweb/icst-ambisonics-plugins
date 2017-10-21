/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AmbisonicsDecoderAudioProcessor::AmbisonicsDecoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	pSpeakerArray = new Array<AmbiPoint>();
	pMovingPointsArray = new Array<AmbiPoint>();
	pAmbiSettings = new AmbiSettings();
	pTestSoundGenerator = new TestSoundGenerator();
}

AmbisonicsDecoderAudioProcessor::~AmbisonicsDecoderAudioProcessor()
{
	pSpeakerArray = nullptr;
	pMovingPointsArray = nullptr;
	pAmbiSettings = nullptr;
	pTestSoundGenerator = nullptr;
}

//==============================================================================
const String AmbisonicsDecoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmbisonicsDecoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmbisonicsDecoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double AmbisonicsDecoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmbisonicsDecoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmbisonicsDecoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmbisonicsDecoderAudioProcessor::setCurrentProgram (int index)
{
}

const String AmbisonicsDecoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmbisonicsDecoderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AmbisonicsDecoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AmbisonicsDecoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmbisonicsDecoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AmbisonicsDecoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    double currentCoefficients[JucePlugin_MaxNumInputChannels];
	const float* inputBufferPointers[JucePlugin_MaxNumInputChannels];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	// copy input buffer and get read pointers
	inputBuffer.makeCopyOf(buffer);
	for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		inputBufferPointers[iChannel] = inputBuffer.getReadPointer(iChannel);

	// clear output buffers if less than #input channels used, the others will be overwritten later
	for (int i = pSpeakerArray->size(); i < totalNumInputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for(int iSpeaker = 0; iSpeaker < pSpeakerArray->size() && iSpeaker < totalNumOutputChannels; iSpeaker++)
	{
		// calculate ambisonics coefficients
		Point3D<double>* pSpeakerPoint = pSpeakerArray->getReference(iSpeaker).getPoint();
		double speakerGain = pSpeakerArray->getReference(iSpeaker).getGain();
		pSpeakerPoint->getAmbisonicsCoefficients(&currentCoefficients[0], pAmbiSettings->getDirectionFlip());
		for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		{
			currentCoefficients[iChannel] *= pAmbiSettings->getAmbiChannelWeight(iChannel);
		}
		// apply to B-format and create output
		float* channelData = buffer.getWritePointer(iSpeaker);
		for(int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
		{
			channelData[iSample] = 0.0f;
			for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
				channelData[iSample] += speakerGain * inputBufferPointers[iChannel][iSample] * currentCoefficients[iChannel];
		}
	}

	pTestSoundGenerator->process(&buffer);
}

//==============================================================================
bool AmbisonicsDecoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmbisonicsDecoderAudioProcessor::createEditor()
{
    return new AmbisonicsDecoderAudioProcessorEditor (*this);
}

//==============================================================================
void AmbisonicsDecoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AmbisonicsDecoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

Array<AmbiPoint>* AmbisonicsDecoderAudioProcessor::getSpeakerArray() const
{
	return pSpeakerArray;
}

Array<AmbiPoint>* AmbisonicsDecoderAudioProcessor::getMovingPointsArray() const
{
	return pMovingPointsArray;
}

AmbiSettings* AmbisonicsDecoderAudioProcessor::getAmbiSettings() const
{
	return pAmbiSettings;
}

TestSoundGenerator* AmbisonicsDecoderAudioProcessor::getTestSoundGenerator() const
{
	return pTestSoundGenerator;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicsDecoderAudioProcessor();
}
