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
	sources = new AmbiDataSet();
	pEncoderSettings = new EncoderSettings();
	pOscHandler = new OSCHandler(sources, &statusMessageHandler);
	pOscSender = new AmbiOSCSender(sources);
	initializeOsc();

	for (int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
	{
		String indexStr = String(i + 1);
		
		AudioParameterSet set;
		set.pX = new AudioParameterFloatAmbi("X" + indexStr, "X " + indexStr, "Point " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::XMin, Constants::XMax), 0.0f, sources, i, AudioParameterFloatAmbi::X);
		set.pY = new AudioParameterFloatAmbi("Y" + indexStr, "Y " + indexStr, "Point " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::YMin, Constants::YMax), 0.0f, sources, i, AudioParameterFloatAmbi::Y);
		set.pZ = new AudioParameterFloatAmbi("Z" + indexStr, "Z " + indexStr, "Point " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::ZMin, Constants::ZMax), 0.0f, sources, i, AudioParameterFloatAmbi::Z);

		audioParams.add(set);
		addParameter(set.pX);
		addParameter(set.pY);
		addParameter(set.pZ);
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

void AmbisonicEncoderAudioProcessor::updateTrackProperties(const TrackProperties& properties)
{
	if (sourcesArray.size() == 1)
	{
		sourcesArray.getUnchecked(0)->setName(properties.name);
		sourcesArray.getUnchecked(0)->setColor(properties.colour);
	}
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

void AmbisonicEncoderAudioProcessor::applyDistanceGain(double* pCoefficientArray, int arraySize, double distance)
{
	if (!pEncoderSettings->distanceEncodingFlag || pEncoderSettings->unitCircleRadius == 0.0)
		return;

	double scaledDistance = distance * (1.0 / pEncoderSettings->unitCircleRadius);
	double wFactor = atan(scaledDistance * PI / 2.0) / scaledDistance * PI / 2.0;
	double otherFactor = (1 - exp(-scaledDistance)) * wFactor;
	pCoefficientArray[0] *= wFactor;
	for (int i = 1; i < arraySize; i++)
		pCoefficientArray[i] *= otherFactor;
}

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
	// Audio handling
	const int totalNumInputChannels = jmin(getTotalNumInputChannels(), sources->size());
	const float channelScaler = 1.0f / totalNumInputChannels;
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
	
	for (int iSource = 0; iSource < totalNumInputChannels; iSource++)
	{
		AmbiPoint* source = sources->get(iSource);

		// keep RMS
		sources->setRms(iSource, inputBuffer.getRMSLevel(iSource, 0, inputBuffer.getNumSamples()), pEncoderSettings->oscSendFlag);

		// calculate ambisonics coefficients
		Point3D<double>* pSourcePoint = source->getPoint();
		pSourcePoint->getAmbisonicsCoefficients(JucePlugin_MaxNumOutputChannels, &currentCoefficients[0], !pEncoderSettings->directionFlip, true);
		applyDistanceGain(&currentCoefficients[0], JucePlugin_MaxNumOutputChannels, pSourcePoint->getDistance());
		const float* inputData = inputBuffer.getReadPointer(iSource);
		
		// create B-format
		int numSamples = buffer.getNumSamples();
		for (int iSample = 0; iSample < numSamples; iSample++)
		{
			double fractionNew = 1.0 / numSamples * iSample;
			double fractionOld = 1.0 - fractionNew;
			for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += channelScaler * float(inputData[iSample] * (fractionNew * currentCoefficients[iChannel] + fractionOld * lastCoefficients[iSource][iChannel]));
		}

		// keep coefficients
		memcpy(&lastCoefficients[iSource], &currentCoefficients, JucePlugin_MaxNumOutputChannels * sizeof(double));
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
	for (int i = 0; i < sources->size(); i++)
	{
		AmbiPoint* pt = sources->get(i);
		if(pt != nullptr)
			sourcesElement->addChildElement(pt->getAsXmlElement(XML_TAG_SOURCE));
	}
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
			sources->clear();
			if (sourcesElement != nullptr)
			{
				int index = 0;
				XmlElement* xmlPoint = sourcesElement->getChildByName(XML_TAG_SOURCE);
				while (xmlPoint != nullptr)
				{
					if (audioParams.size() > index)
						sources->add(new AmbiPoint(xmlPoint, audioParams[index]));
					else
						sources->add(new AmbiPoint(xmlPoint, AudioParameterSet()));

					xmlPoint = xmlPoint->getNextElement();
					index++;
				}
			}
		}
	}

	initializeOsc();
}

AmbiDataSet* AmbisonicEncoderAudioProcessor::getSources() const
{
	return sources;
}

Array<AudioParameterSet>* AmbisonicEncoderAudioProcessor::getAudioParams()
{
	return &audioParams;
}

StatusMessageHandler* AmbisonicEncoderAudioProcessor::getStatusMessageHandler()
{
	return &statusMessageHandler;
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
