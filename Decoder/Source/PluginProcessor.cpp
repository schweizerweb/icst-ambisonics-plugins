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
	pDecoderSettings = new DecoderSettings();
	pTestSoundGenerator = new TestSoundGenerator();
}

AmbisonicsDecoderAudioProcessor::~AmbisonicsDecoderAudioProcessor()
{
	pSpeakerArray = nullptr;
	pMovingPointsArray = nullptr;
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

void AmbisonicsDecoderAudioProcessor::checkDelayBuffers()
{
	// add or remove delay buffers if number of speakers changed
	while(pSpeakerArray->size() > delayBuffers.size())
	{
		delayBuffers.add(new DelayBuffer());
	}
	if (pSpeakerArray->size() < delayBuffers.size())
		delayBuffers.removeLast(delayBuffers.size() - pSpeakerArray->size());

	// check delays
	double maxDist = delayHelper.getMaxNormalizedDistance(pSpeakerArray);
	for (int i = 0; i < pSpeakerArray->size(); i++)
	{
		int requiredDelay = delayHelper.getDelayCompensationSamples(pAmbiSettings, &pSpeakerArray->getReference(i), maxDist, getSampleRate());		
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

void AmbisonicsDecoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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
	xml->setAttribute("uiWidth", pDecoderSettings->lastUIWidth);
	xml->setAttribute("uiHeight", pDecoderSettings->lastUIHeight);
	xml->setAttribute("oscReceive", pDecoderSettings->oscReceive);
	xml->setAttribute("oscReceivePort", pDecoderSettings->oscReceivePort);

	// load last speaker preset
	PresetInfo* preset = new PresetInfo();
	preset->setName("LastState");
	for (AmbiPoint pt : *pSpeakerArray)
		preset->getPoints()->add(new AmbiPoint(pt));
	preset->getAmbiSettings()->setDistanceScaler(pAmbiSettings->getDistanceScaler());
	preset->getAmbiSettings()->setDirectionFlip(pAmbiSettings->getDirectionFlip());
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		preset->getAmbiSettings()->getAmbiOrderWeightPointer()[i] = pAmbiSettings->getAmbiOrderWeightPointer()[i];
	XmlElement* speakerSettings = new XmlElement(XML_TAG_PRESET_ROOT);
	preset->CreateXmlRoot(speakerSettings);
	xml->addChildElement(speakerSettings);

	copyXmlToBinary(*xml, destData);
	xml->deleteAllChildElements();
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
			pDecoderSettings->lastUIWidth = jmax(xmlState->getIntAttribute("uiWidth", pDecoderSettings->lastUIWidth), 300);
			pDecoderSettings->lastUIHeight = jmax(xmlState->getIntAttribute("uiHeight", pDecoderSettings->lastUIHeight), 600);
			pDecoderSettings->oscReceive = xmlState->getBoolAttribute("oscReceive", false);
			pDecoderSettings->oscReceivePort = xmlState->getIntAttribute("oscReceivePort", 5011);
			
			// load last speaker preset
			PresetInfo* preset = new PresetInfo();
			XmlElement* presetElement = xmlState->getChildByName(XML_TAG_PRESET_ROOT);
			if (presetElement != nullptr && preset->LoadFromXmlRoot(presetElement))
			{
				pSpeakerArray->clear();
				for (AmbiPoint* pt : *preset->getPoints())
				{
					pSpeakerArray->add(AmbiPoint(*pt));
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
