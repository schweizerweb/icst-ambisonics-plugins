/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioParameterFloatAmbi.h"

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
	pEncoderSettings = new EncoderSettings();
	pOscHandler = new OSCHandler(&sourcesArray);
	pOscSender = new AmbiOSCSender(&sourcesArray);

	for (int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
	{
		String indexStr = String(i + 1);
		
		AudioParameterSet set;
		set.pA = new AudioParameterFloatAmbi("Azimuth" + indexStr, "Azimuth " + indexStr, "Point " + indexStr + ": Azimuth", AudioProcessorParameter::genericParameter, NormalisableRange<float>(float(-PI), float(PI)), 0.0f, &sourcesArray, i, AudioParameterFloatAmbi::Azimuth);
		set.pE = new AudioParameterFloatAmbi("Elevation" + indexStr, "Elevation " + indexStr, "Point " + indexStr + ": Elevation", AudioProcessorParameter::genericParameter, NormalisableRange<float>(0.0f, float(PI)), 0.0f, &sourcesArray, i, AudioParameterFloatAmbi::Elevation);
		set.pD = new AudioParameterFloatAmbi("Distance" + indexStr, "Distance " + indexStr, "Point " + indexStr + ": Distance", AudioProcessorParameter::genericParameter, NormalisableRange<float>(0.0f, 1.0f), 0.0f, &sourcesArray, i, AudioParameterFloatAmbi::Distance);
		
		audioParams.add(set);
		addParameter(set.pA);
		addParameter(set.pE);
		addParameter(set.pD);
	}
}

AmbisonicEncoderAudioProcessor::~AmbisonicEncoderAudioProcessor()
{
	pEncoderSettings = nullptr;
	pOscHandler = nullptr;
	pOscSender = nullptr;
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

void AmbisonicEncoderAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String AmbisonicEncoderAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void AmbisonicEncoderAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void AmbisonicEncoderAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
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

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
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
	
	for (int iSource = 0; iSource < sourcesArray.size() && iSource < totalNumInputChannels; iSource++)
	{
		// keep RMS
		sourcesArray[iSource]->setRms(inputBuffer.getRMSLevel(iSource, 0, inputBuffer.getNumSamples()), pEncoderSettings->oscSendFlag);

		// calculate ambisonics coefficients
		Point3D<double>* pSourcePoint = sourcesArray[iSource]->getPoint();
		pSourcePoint->getAmbisonicsCoefficients(JucePlugin_MaxNumOutputChannels, &currentCoefficients[0], false);
		const float* inputData = inputBuffer.getReadPointer(iSource);
		
		// create B-format
		for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
		{
			for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += float(inputData[iSample] * currentCoefficients[iChannel]);
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
	for (AmbiPoint* pt : sourcesArray)
		sourcesElement->addChildElement(pt->getAsXmlElement(XML_TAG_SOURCE));
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
			sourcesArray.clear();
			if (sourcesElement != nullptr)
			{
				int index = 0;
				XmlElement* xmlPoint = sourcesElement->getChildByName(XML_TAG_SOURCE);
				while (xmlPoint != nullptr)
				{
					if (audioParams.size() > index)
						sourcesArray.add(new AmbiPoint(xmlPoint, audioParams[index]));
					else
						sourcesArray.add(new AmbiPoint(xmlPoint, AudioParameterSet()));

					xmlPoint = xmlPoint->getNextElement();
					index++;
				}
			}
		}
	}

	initializeOsc();
}

OwnedArray<AmbiPoint>* AmbisonicEncoderAudioProcessor::getSourcesArray()
{
	return &sourcesArray;
}

Array<AudioParameterSet>* AmbisonicEncoderAudioProcessor::getAudioParams()
{
	return &audioParams;
}

EncoderSettings* AmbisonicEncoderAudioProcessor::getEncoderSettings() const
{
	return pEncoderSettings;
}

void AmbisonicEncoderAudioProcessor::initializeOsc() const
{
	if(pEncoderSettings->oscReceiveFlag)
	{
		if (!pOscHandler->start(pEncoderSettings->oscReceivePort))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Receiver on Port " + String(pEncoderSettings->oscReceivePort));
			pEncoderSettings->oscReceiveFlag = false;
		}
	}
	else
	{
		pOscHandler->stop();
	}

	if(pEncoderSettings->oscSendFlag)
	{
		if (!pOscSender->start(pEncoderSettings->oscSendTargetHost, pEncoderSettings->oscSendPort, pEncoderSettings->oscSendIntervalMs))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Sender on " + pEncoderSettings->oscSendTargetHost + ":" + String(pEncoderSettings->oscSendPort));
			pEncoderSettings->oscSendFlag = false;
		}
	}
	else
	{
		pOscSender->stop();
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicEncoderAudioProcessor();
}
