/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define XML_ROOT_TAG "AMBISONICENCODERPLUGINSETTINGS"
#define XML_TAG_ENCODER_SETTINGS "EncoderSettings"
#define XML_TAG_SOURCES "Sources"
#define XML_TAG_SOURCE "Source"

//==============================================================================
AmbisonicEncoderAudioProcessor::AmbisonicEncoderAudioProcessor()
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
	pSourcesArray = new Array<AmbiPoint>();
	pEncoderSettings = new EncoderSettings();
}

AmbisonicEncoderAudioProcessor::~AmbisonicEncoderAudioProcessor()
{
	pSourcesArray = nullptr;
}

//==============================================================================
const String AmbisonicEncoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmbisonicEncoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmbisonicEncoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double AmbisonicEncoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmbisonicEncoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmbisonicEncoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmbisonicEncoderAudioProcessor::setCurrentProgram (int index)
{
}

const String AmbisonicEncoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmbisonicEncoderAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AmbisonicEncoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AmbisonicEncoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmbisonicEncoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// MIDI In Handling
	int time;
	MidiMessage m;
	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{
		if (m.isController() && m.getChannel() <= pSourcesArray->size())
		{
			switch (m.getControllerNumber())
			{
			case 12: pSourcesArray->getReference(m.getChannel() - 1).getPoint()->setAzimuth(m.getControllerValue() / 127.0 * 2.0 * PI); pSourcesArray->getReference(m.getChannel() - 1).setName(String(m.getTimeStamp())); break;
			case 13: pSourcesArray->getReference(m.getChannel() - 1).getPoint()->setElevation(m.getControllerValue() / 127.0 * 0.5 * PI); break;
			case 14: pSourcesArray->getReference(m.getChannel() - 1).getPoint()->setDistance(m.getControllerValue() / 127.0); break;
			}
		}
	}

	// Audio handling
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	double currentCoefficients[JucePlugin_MaxNumOutputChannels];
	float* outputBufferPointers[JucePlugin_MaxNumOutputChannels];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	// copy input buffer, clear output and get write pointers
	inputBuffer.makeCopyOf(buffer);
	buffer.clear();
	for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
		outputBufferPointers[iChannel] = buffer.getWritePointer(iChannel);
	
	for (int iSource = 0; iSource < pSourcesArray->size() && iSource < totalNumInputChannels; iSource++)
	{
		// calculate ambisonics coefficients
		Point3D<double>* pSourcePoint = pSourcesArray->getReference(iSource).getPoint();
		pSourcePoint->getAmbisonicsCoefficients(JucePlugin_MaxNumOutputChannels, &currentCoefficients[0], false);
		const float* inputData = inputBuffer.getReadPointer(iSource);

		// create B-format
		for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
		{
			for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += inputData[iSample] * currentCoefficients[iChannel];
		}
	}
}

//==============================================================================
bool AmbisonicEncoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmbisonicEncoderAudioProcessor::createEditor()
{
    return new AmbisonicEncoderAudioProcessorEditor (*this);
}

//==============================================================================
void AmbisonicEncoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	ScopedPointer<XmlElement> xml = new XmlElement(XML_ROOT_TAG);

	// save general encoder settings
	xml->addChildElement(pEncoderSettings->getAsXmlElement(XML_TAG_ENCODER_SETTINGS));
	
	// load sources
	XmlElement* sourcesElement = new XmlElement(XML_TAG_SOURCES);
	for (AmbiPoint pt : *pSourcesArray)
		sourcesElement->addChildElement(pt.getAsXmlElement(XML_TAG_SOURCE));
	xml->addChildElement(sourcesElement);

	copyXmlToBinary(*xml, destData);
	xml->deleteAllChildElements();
}

void AmbisonicEncoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName(XML_ROOT_TAG))
		{
			// load general encoder settings
			pEncoderSettings->loadFromXml(xmlState->getChildByName(XML_TAG_ENCODER_SETTINGS));

			// load last speaker preset
			XmlElement* sourcesElement = xmlState->getChildByName(XML_TAG_SOURCES);
			pSourcesArray->clear();
			if (sourcesElement != nullptr)
			{
				XmlElement* xmlPoint = sourcesElement->getChildByName(XML_TAG_SOURCE);
				while (xmlPoint != nullptr)
				{
					pSourcesArray->add(AmbiPoint(xmlPoint));
					xmlPoint = xmlPoint->getNextElement();
				}
			}
		}
	}
}

Array<AmbiPoint>* AmbisonicEncoderAudioProcessor::getSourcesArray() const
{
	return pSourcesArray;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicEncoderAudioProcessor();
}
