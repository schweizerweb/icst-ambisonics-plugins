/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioParameterFloatAmbi.h"
#include "../../Common/TrackColors.h"
#include "EncoderConstants.h"

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
    sources.reset(new AmbiSourceSet(getScalingInfo()));
	pOscHandler = new OSCHandlerEncoder(sources.get(), &statusMessageHandler, &encoderSettings.distanceEncodingParams, getScalingInfo());
	pOscSender = new AmbiOSCSender(sources.get());
	pOscSenderExt = new AmbiOSCSenderExt(sources.get(), &statusMessageHandler, getScalingInfo());

	initializeOsc();
    initializeAudioParameter();
    
    presetHelper.reset(new EncoderPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder"), this, getScalingInfo()));
    presetHelper->initialize();
	distanceEncodingPresetHelper.reset(new DistanceEncodingPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder/DistanceEncoding"), this));
	distanceEncodingPresetHelper->initialize();

#if(!MULTI_ENCODER_MODE)
    // initialize mono encoder with one source
    if (sources->size() == 0)
    {
		scalingInfo.SetScaler(1.0);
        String name = dawParameter.updateTrackPropertiesWorking ? dawParameter.lastTrackProperties.name : "1";
        Colour color = dawParameter.updateTrackPropertiesWorking ? dawParameter.lastTrackProperties.colour : TrackColors::getColor(0);
        sources->addNew(Uuid().toString(), Point3D<double>(0.0, 0.0, 0.0, audioParams.sourceParams[0]), name, color);
    }
#else
    if(!presetHelper->loadDefaultPreset(&audioParams, sources.get(), &encoderSettings))
    {
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Default preset", "Default preset not found, please restore presets using the Preset Manager!");
    }
#endif
}

AmbisonicEncoderAudioProcessor::~AmbisonicEncoderAudioProcessor()
{
	delete pOscHandler;
	delete pOscSender;
	delete pOscSenderExt;
}

void AmbisonicEncoderAudioProcessor::initializeAudioParameter()
{
#if MULTI_ENCODER_MODE
    encoderSettings.initialize(this);
#endif
	encoderSettings.distanceEncodingParams.initialize(this);
    // points (X, Y, Z, Gain)
     for (int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
     {
        String indexStr = String(i + 1);
         
        AudioParameterSet set;
        set.pScaling = getScalingInfo();
        set.pX = new AudioParameterFloatAmbi("X" + indexStr, "X " + indexStr, "Point " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::X);
        set.pY = new AudioParameterFloatAmbi("Y" + indexStr, "Y " + indexStr, "Point " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::Y);
        set.pZ = new AudioParameterFloatAmbi("Z" + indexStr, "Z " + indexStr, "Point " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::Z);
        set.pGain = new AudioParameterFloatAmbi("Gain" + indexStr, "Gain" + indexStr, "Point " + indexStr + ": Gain", AudioProcessorParameter::genericParameter, NormalisableRange<float>((float)Constants::GainDbMin, (float)Constants::GainDbMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::Gain);
         
        audioParams.sourceParams.add(set);
        addParameter(set.pX);
        addParameter(set.pY);
        addParameter(set.pZ);
        addParameter(set.pGain);
    }
    
    for (int i = 0; i < MAXIMUM_NUMBER_OF_GROUPS; i++)
    {
        String indexStr = String(i + 1);
        
        AudioParameterSet set;
        set.pScaling = getScalingInfo();
        set.pX = new AudioParameterFloatAmbi("GX" + indexStr, "GX " + indexStr, "Group " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::GX);
        set.pY = new AudioParameterFloatAmbi("GY" + indexStr, "GY " + indexStr, "Group " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::GY);
        set.pZ = new AudioParameterFloatAmbi("GZ" + indexStr, "GZ " + indexStr, "Group " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, sources.get(), i, AudioParameterFloatAmbi::GZ);
        
        audioParams.groupParams.add(set);
        addParameter(set.pX);
        addParameter(set.pY);
        addParameter(set.pZ);
    }
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
void AmbisonicEncoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	iirFilterSpec.numChannels = 1;
	iirFilterSpec.maximumBlockSize = samplesPerBlock;
	iirFilterSpec.sampleRate = sampleRate;
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

void AmbisonicEncoderAudioProcessor::applyDistanceGain(double* pCoefficientArray, int arraySize, double distance) const
{
	if (!encoderSettings.distanceEncodingFlag || encoderSettings.distanceEncodingParams.getUnitCircleRadius() == 0.0)
		return;

	double wFactor, otherFactor;
	encoderSettings.distanceEncodingParams.calculateAttenuation(distance, &wFactor, &otherFactor);

	pCoefficientArray[0] *= wFactor;
	for (int i = 1; i < arraySize; i++)
		pCoefficientArray[i] *= otherFactor;
}

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
	// Audio handling
    const float masterGainFactor = float(Decibels::decibelsToGain(encoderSettings.getMasterGain()));
	const int totalNumInputChannels = jmin(getTotalNumInputChannels(), sources->size());
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	double currentCoefficients[JucePlugin_MaxNumOutputChannels];
	float* outputBufferPointers[JucePlugin_MaxNumOutputChannels];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	// copy input buffer, clear output and get write pointers
	inputBuffer.makeCopyOf(buffer);
	buffer.clear();
    
    // in case of scaling change with doppler enabled, send an empty buffer and reset delay buffers
    double newScaler = scalingInfo.GetScaler();
    if(encoderSettings.dopplerEncodingFlag && lastScaler != newScaler)
    {
        for (int iSource = 0; iSource < totalNumInputChannels; iSource++)
        {
            delayBuffers[iSource].initialize(0);
        }
        lastScaler = newScaler;
        return;
    }
    
    // prepare a buffer to reuse for doppler operation
	AudioSampleBuffer localBuffer(1, inputBuffer.getNumSamples());

    // prepare write pointers
	for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
		outputBufferPointers[iChannel] = buffer.getWritePointer(iChannel);
	
    // loop through input channels
	for (int iSource = 0; iSource < totalNumInputChannels; iSource++)
	{
		AmbiSource* source = sources->get(iSource);
		if (source == nullptr || !source->getEnabled())
			continue;

		Point3D<double>* pSourcePoint = source->getPoint();

		// air absorbtion filter
		if (encoderSettings.distanceEncodingFlag && airAbsorbtionFilters[iSource].checkFilter(&encoderSettings.distanceEncodingParams, pSourcePoint->getDistance(), &iirFilterSpec))
		{
            float* writePointer = inputBuffer.getWritePointer(iSource);
            AirAbsorbtionFilter* filter = &airAbsorbtionFilters[iSource];
			for (int iSample = 0; iSample < inputBuffer.getNumSamples(); iSample++)
			{
				writePointer[iSample] = filter->processSample(writePointer[iSample]);
			}
		}

		// keep RMS
		sources->setRms(iSource, inputBuffer.getRMSLevel(iSource, 0, inputBuffer.getNumSamples()), encoderSettings.oscSendFlag);

		// calculate ambisonics coefficients
		pSourcePoint->getAmbisonicsCoefficients(JucePlugin_MaxNumOutputChannels, &currentCoefficients[0], true, true);
		applyDistanceGain(&currentCoefficients[0], JucePlugin_MaxNumOutputChannels, pSourcePoint->getDistance());
		
		if (encoderSettings.dopplerEncodingFlag)
		{
			delayBuffers[iSource].check(int(DelayHelper::getDelaySamples(scalingInfo.GetScaler() * MathConstants<float>::sqrt2, getSampleRate())));
            localBuffer.copyFrom(0, 0, inputBuffer, iSource, 0, inputBuffer.getNumSamples());
			// check doppler delay buffers
			float currentDelayInSamples = DelayHelper::getDelaySamples(pSourcePoint->getDistance(), getSampleRate());
			delayBuffers[iSource].process(currentDelayInSamples, localBuffer.getReadPointer(0), inputBuffer.getWritePointer(iSource), inputBuffer.getNumSamples());
		}

		const float* inputData = inputBuffer.getReadPointer(iSource);
		const float sourceGain = float(source->getGain());

		// create B-format
		int numSamples = buffer.getNumSamples();
		for (int iSample = 0; iSample < numSamples; iSample++)
		{
			double fractionNew = 1.0 / numSamples * iSample;
			double fractionOld = 1.0 - fractionNew;
			for (iChannel = 0; iChannel < totalNumOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += sourceGain * masterGainFactor * float(inputData[iSample] * (fractionNew * currentCoefficients[iChannel] + fractionOld * lastCoefficients[iSource][iChannel]));
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
	return new AmbisonicEncoderAudioProcessorEditor(*this);
}

//==============================================================================
void AmbisonicEncoderAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	XmlElement* xml = new XmlElement(XML_ROOT_TAG);

	// save general encoder settings
	xml->addChildElement(encoderSettings.getAsXmlElement(XML_TAG_ENCODER_SETTINGS));
	sources->writeToXmlElement(xml);
	
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
            scalingInfo.SetScaler(encoderSettings.getDistanceScaler());
			// load last source preset
			sources->loadFromXml(xmlState.get(), &audioParams);
            sources->resetIds();
		}
	}

	initializeOsc();
}

AmbiSourceSet* AmbisonicEncoderAudioProcessor::getSources()
{
	return sources.get();
}

AudioParams* AmbisonicEncoderAudioProcessor::getAudioParams()
{
	return &audioParams;
}

StatusMessageHandler* AmbisonicEncoderAudioProcessor::getStatusMessageHandler()
{
	return &statusMessageHandler;
}

DawParameter* AmbisonicEncoderAudioProcessor::getDawParameter()
{
	return &dawParameter;
}

EncoderPresetHelper* AmbisonicEncoderAudioProcessor::getPresetHelper()
{
    return presetHelper.get();
}

DistanceEncodingPresetHelper* AmbisonicEncoderAudioProcessor::getDistanceEncodingPresetHelper()
{
	return distanceEncodingPresetHelper.get();
}

ScalingInfo* AmbisonicEncoderAudioProcessor::getScalingInfo()
{
    return &scalingInfo;
}

#if (!MULTI_ENCODER_MODE)
void AmbisonicEncoderAudioProcessor::updateTrackProperties(const TrackProperties& properties)
{
	dawParameter.updateTrackPropertiesWorking = true;
	dawParameter.lastTrackProperties = properties;

	if (sources->size() > 0)
	{
		sources->get(0)->setName(properties.name);
		sources->get(0)->setColor(properties.colour);
	}
}
#endif

EncoderSettings* AmbisonicEncoderAudioProcessor::getEncoderSettings()
{
	return &encoderSettings;
}

void AmbisonicEncoderAudioProcessor::initializeOsc()
{
	if (encoderSettings.oscReceiveFlag)
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

	if (encoderSettings.oscSendFlag)
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

	String message;
	if (!pOscSenderExt->start(&encoderSettings, &message))
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, JucePlugin_Name, ("Error starting OSC Sender for external usage: \r\n" + message));
	}
}

void AmbisonicEncoderAudioProcessor::actionListenerCallback(const juce::String &message)
{
    if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        presetHelper->loadFromXmlFile(presetFile, &audioParams, sources.get(), &encoderSettings);
        presetHelper->notifyPresetChanged();
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicEncoderAudioProcessor();
}
