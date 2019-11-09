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
	pOscHandler = new OSCHandler(&sources, &statusMessageHandler);
	pOscSender = new AmbiOSCSender(&sources);
	pOscSenderExt = new AmbiOSCSenderExt(&sources);
	initializeOsc();

	for (int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
	{
		String indexStr = String(i + 1);
		
		AudioParameterSet set;
		set.pX = new AudioParameterFloatAmbi("X" + indexStr, "X " + indexStr, "Point " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::XMin, Constants::XMax), 0.0f, &sources, i, AudioParameterFloatAmbi::X);
		set.pY = new AudioParameterFloatAmbi("Y" + indexStr, "Y " + indexStr, "Point " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::YMin, Constants::YMax), 0.0f, &sources, i, AudioParameterFloatAmbi::Y);
		set.pZ = new AudioParameterFloatAmbi("Z" + indexStr, "Z " + indexStr, "Point " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::ZMin, Constants::ZMax), 0.0f, &sources, i, AudioParameterFloatAmbi::Z);

		audioParams.add(set);
		addParameter(set.pX);
		addParameter(set.pY);
		addParameter(set.pZ);
	}
}

AmbisonicEncoderAudioProcessor::~AmbisonicEncoderAudioProcessor()
{
	delete pOscHandler;
	delete pOscSender;
	delete pOscSenderExt;
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

void AmbisonicEncoderAudioProcessor::applyDistanceGain(double* pCoefficientArray, int arraySize, double distance)
{
	if (!encoderSettings.distanceEncodingFlag || encoderSettings.unitCircleRadius == 0.0)
		return;

	double scaledDistance = distance * (1.0 / encoderSettings.unitCircleRadius);
	double wFactor = atan(scaledDistance * PI / 2.0) / (scaledDistance * PI / 2.0);
	double otherFactor = (1 - exp(-scaledDistance)) * wFactor;
	pCoefficientArray[0] *= wFactor;
	for (int i = 1; i < arraySize; i++)
		pCoefficientArray[i] *= otherFactor;
}

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
	// Audio handling
	const int totalNumInputChannels = jmin(getTotalNumInputChannels(), sources.size());
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
		AmbiPoint* source = sources.get(iSource);
		const float sourceGain = float(source->getGain());

		// keep RMS
		sources.setRms(iSource, inputBuffer.getRMSLevel(iSource, 0, inputBuffer.getNumSamples()), encoderSettings.oscSendFlag);

		// calculate ambisonics coefficients
		Point3D<double>* pSourcePoint = source->getPoint();
		pSourcePoint->getAmbisonicsCoefficients(JucePlugin_MaxNumOutputChannels, &currentCoefficients[0], !encoderSettings.directionFlip, true);
		applyDistanceGain(&currentCoefficients[0], JucePlugin_MaxNumOutputChannels, pSourcePoint->getDistance());
		const float* inputData = inputBuffer.getReadPointer(iSource);
		
		// create B-format
		int numSamples = buffer.getNumSamples();
		for (int iSample = 0; iSample < numSamples; iSample++)
		{
			double fractionNew = 1.0 / numSamples * iSample;
			double fractionOld = 1.0 - fractionNew;
			for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += sourceGain * channelScaler * float(inputData[iSample] * (fractionNew * currentCoefficients[iChannel] + fractionOld * lastCoefficients[iSource][iChannel]));
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
	XmlElement* xml = new XmlElement(XML_ROOT_TAG);

	// save general encoder settings
	xml->addChildElement(encoderSettings.getAsXmlElement(XML_TAG_ENCODER_SETTINGS));
	sources.writeToXmlElement(xml);
	
	copyXmlToBinary(*xml, destData);
	xml->deleteAllChildElements();
	delete xml;
}

void AmbisonicEncoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName(XML_ROOT_TAG))
		{
			// load general encoder settings
			encoderSettings.loadFromXml(xmlState->getChildByName(XML_TAG_ENCODER_SETTINGS));

			// load last speaker preset
			sources.loadFromXml(xmlState.get(), &audioParams);
		}
	}

	initializeOsc();
}

AmbiSourceSet* AmbisonicEncoderAudioProcessor::getSources()
{
	return &sources;
}

Array<AudioParameterSet>* AmbisonicEncoderAudioProcessor::getAudioParams()
{
	return &audioParams;
}

StatusMessageHandler* AmbisonicEncoderAudioProcessor::getStatusMessageHandler()
{
	return &statusMessageHandler;
}

EncoderSettings* AmbisonicEncoderAudioProcessor::getEncoderSettings()
{
	return &encoderSettings;
}

void AmbisonicEncoderAudioProcessor::initializeOsc()
{
	if(encoderSettings.oscReceiveFlag)
	{
		if (!pOscHandler->start(encoderSettings.oscReceivePort))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Receiver on Port " + String(encoderSettings.oscReceivePort));
			encoderSettings.oscReceiveFlag = false;
		}
	}
	else
	{
		pOscHandler->stop();
	}

	if(encoderSettings.oscSendFlag)
	{
		if (!pOscSender->start(encoderSettings.oscSendTargetHost, encoderSettings.oscSendPort, encoderSettings.oscSendIntervalMs))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Sender on " + encoderSettings.oscSendTargetHost + ":" + String(encoderSettings.oscSendPort));
			encoderSettings.oscSendFlag = false;
		}
	}
	else
	{
		pOscSender->stop();
	}

	if (encoderSettings.oscSendExtFlag)
	{
		if (!pOscSenderExt->start(encoderSettings.oscSendExtTargetHost, encoderSettings.oscSendExtPort))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, "Error starting OSC Sender for external usage on " + encoderSettings.oscSendExtTargetHost + ":" + String(encoderSettings.oscSendExtPort));
			encoderSettings.oscSendExtFlag = false;
		}
	}
	else
	{
		pOscSenderExt->stop();
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicEncoderAudioProcessor();
}
