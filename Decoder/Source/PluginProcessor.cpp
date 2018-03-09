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
	pAmbiSettings = new AmbiSettings();
	pDecoderSettings = new DecoderSettings();
	pTestSoundGenerator = new TestSoundGenerator();
}

AmbisonicsDecoderAudioProcessor::~AmbisonicsDecoderAudioProcessor()
{
	pAmbiSettings = nullptr;
	pDecoderSettings = nullptr;
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

void AmbisonicsDecoderAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String AmbisonicsDecoderAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void AmbisonicsDecoderAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void AmbisonicsDecoderAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AmbisonicsDecoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AmbisonicsDecoderAudioProcessor::checkDelayBuffers()
{
	// add or remove delay buffers if number of speakers changed
	while(speakerArray.size() > delayBuffers.size())
	{
		delayBuffers.add(new DelayBuffer());
	}
	if (speakerArray.size() < delayBuffers.size())
		delayBuffers.removeLast(delayBuffers.size() - speakerArray.size());

	// check delays
	double maxDist = delayHelper.getMaxNormalizedDistance(&speakerArray);
	for (int i = 0; i < speakerArray.size(); i++)
	{
		int requiredDelay = delayHelper.getDelayCompensationSamples(pAmbiSettings, speakerArray.getUnchecked(i), maxDist, getSampleRate());		
		delayBuffers.getUnchecked(i)->checkAndAdjustSize(requiredDelay);
	}
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

void AmbisonicsDecoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    double currentCoefficients[JucePlugin_MaxNumInputChannels];
	const float* inputBufferPointers[JucePlugin_MaxNumInputChannels];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	checkDelayBuffers();

	// copy input buffer and get read pointers
	inputBuffer.makeCopyOf(buffer);
	for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		inputBufferPointers[iChannel] = inputBuffer.getReadPointer(iChannel);

	// clear output buffers if less than #input channels used, the others will be overwritten later
	for (int i = speakerArray.size(); i < totalNumInputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for(int iSpeaker = 0; iSpeaker < speakerArray.size() && iSpeaker < totalNumOutputChannels; iSpeaker++)
	{
		// calculate ambisonics coefficients
		Point3D<double>* pSpeakerPoint = speakerArray.getUnchecked(iSpeaker)->getPoint();
		double speakerGain = speakerArray.getUnchecked(iSpeaker)->getGain();
		pSpeakerPoint->getAmbisonicsCoefficients(JucePlugin_MaxNumInputChannels, &currentCoefficients[0], pAmbiSettings->getDirectionFlip());
		for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		{
			currentCoefficients[iChannel] *= pAmbiSettings->getAmbiChannelWeight(iChannel);
		}
		// apply to B-format and create output
		float* channelData = buffer.getWritePointer(iSpeaker);
		for(int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
		{
			float currentSample = 0.0f;
			for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
				currentSample += float(speakerGain * inputBufferPointers[iChannel][iSample] * currentCoefficients[iChannel]);
			channelData[iSample] = delayBuffers.getUnchecked(iSpeaker)->processNextSample(currentSample);
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
    ScopedPointer<XmlElement> xml = new XmlElement("AMBISONICDECODERPLUGINSETTINGS");

	// save general decoder settings
	pDecoderSettings->saveToXml(xml.get());
	
	// load last speaker preset
	ScopedPointer<PresetInfo> preset = new PresetInfo();
	preset->setName("LastState");
	for (AmbiPoint* pt : speakerArray)
		preset->getPoints()->add(new AmbiPoint(*pt));
	preset->getAmbiSettings()->setDistanceScaler(pAmbiSettings->getDistanceScaler());
	preset->getAmbiSettings()->setDirectionFlip(pAmbiSettings->getDirectionFlip());
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		preset->getAmbiSettings()->getAmbiOrderWeightPointer()[i] = pAmbiSettings->getAmbiOrderWeightPointer()[i];
	XmlElement* speakerSettings = new XmlElement(XML_TAG_PRESET_ROOT);
	preset->CreateXmlRoot(speakerSettings);
	xml->addChildElement(speakerSettings);

	copyXmlToBinary(*xml, destData);

	xml->deleteAllChildElements();
	preset = nullptr;
}

void AmbisonicsDecoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName("AMBISONICDECODERPLUGINSETTINGS"))
		{
			// load general decoder settings
			pDecoderSettings->loadFromXml(xmlState.get());
			
			// load last speaker preset
			PresetInfo* preset = new PresetInfo();
			XmlElement* presetElement = xmlState->getChildByName(XML_TAG_PRESET_ROOT);
			if (presetElement != nullptr && preset->LoadFromXmlRoot(presetElement))
			{
				speakerArray.clear();
				for (int i = 0; i < preset->getPoints()->size(); i++)
				{
					speakerArray.add(new AmbiPoint(*preset->getPoints()->getUnchecked(i)));
				}
				pAmbiSettings->setDistanceScaler(preset->getAmbiSettings()->getDistanceScaler());
				pAmbiSettings->setDirectionFlip(preset->getAmbiSettings()->getDirectionFlip());

				for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
				{
					pAmbiSettings->getAmbiOrderWeightPointer()[i] = preset->getAmbiSettings()->getAmbiOrderWeightPointer()[i];
				}
			}
		}
	}
}

OwnedArray<AmbiPoint>* AmbisonicsDecoderAudioProcessor::getSpeakerArray()
{
	return &speakerArray;
}

OwnedArray<AmbiPoint>* AmbisonicsDecoderAudioProcessor::getMovingPointsArray()
{
	return &movingPointsArray;
}

AmbiSettings* AmbisonicsDecoderAudioProcessor::getAmbiSettings() const
{
	return pAmbiSettings;
}

DecoderSettings* AmbisonicsDecoderAudioProcessor::getDecoderSettings() const
{
	return pDecoderSettings;
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
