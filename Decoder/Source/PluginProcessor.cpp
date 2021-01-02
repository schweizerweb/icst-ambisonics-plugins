/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../Common/FFTAnalyzer.h"


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
	pTestSoundGenerator = new TestSoundGenerator(&speakerSet);
    
    presetHelper.reset(new DecoderPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiDecoder"), this));
    presetHelper->initialize();
    presetHelper->loadDefaultPreset(&speakerSet, &ambiSettings);
}

AmbisonicsDecoderAudioProcessor::~AmbisonicsDecoderAudioProcessor()
{
	delete pTestSoundGenerator;
    FFTAnalyzer::deleteInstance();
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
void AmbisonicsDecoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    iirFilterSpec.numChannels = 1;
    iirFilterSpec.maximumBlockSize = samplesPerBlock;
    iirFilterSpec.sampleRate = sampleRate;
}

void AmbisonicsDecoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AmbisonicsDecoderAudioProcessor::checkDelayBuffers()
{
	int size = speakerSet.size();
	// add or remove delay buffers if number of speakers changed
	while(size > delayBuffers.size())
	{
		delayBuffers.add(new DelayBuffer());
	}
	if (size < delayBuffers.size())
		delayBuffers.removeLast(delayBuffers.size() - size);

	// check delays
	double maxDist = speakerSet.getMaxNormalizedDistance();
	for (int i = 0; i < size && i < speakerSet.size(); i++)
	{
		AmbiPoint* pt = speakerSet.get(i);
		if (pt != nullptr)
		{
			int requiredDelay = delayHelper.getDelayCompensationSamples(&ambiSettings, pt, maxDist, getSampleRate());
			delayBuffers.getUnchecked(i)->checkAndAdjustSize(requiredDelay);
		}
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

void AmbisonicsDecoderAudioProcessor::checkFilters()
{
    int size = speakerSet.size();

    for(int iSpeaker = 0; iSpeaker < size; iSpeaker++)
    {
        if (!speakerSet.get(iSpeaker)->getFilterBypass())
        {
            for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
            {
                FilterInfo* pFilter = speakerSet.get(iSpeaker)->getFilterInfo()->get(iFilter);
                if (pFilter == nullptr)
                {
                    Logger::writeToLog("Error in Filterbank!");
                    return;
                }
                if (!filterInfo[iSpeaker].get(iFilter)->equals(pFilter))
                {
                    if (filterInfo[iSpeaker].get(iFilter)->filterType != pFilter->filterType)
                    {
                        iirFilters[iSpeaker][iFilter].prepare(iirFilterSpec);
                        iirFilters[iSpeaker][iFilter].reset();
                    }

                    filterInfo[iSpeaker].get(iFilter)->copyFrom(pFilter);
                    auto newCoeff = pFilter->getCoefficients(iirFilterSpec.sampleRate);
                    if (newCoeff != nullptr)
                    {
                        iirFilters[iSpeaker][iFilter].coefficients = newCoeff;
                    }
                }
            }
        }
    }
}

void AmbisonicsDecoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    double currentCoefficients[JucePlugin_MaxNumInputChannels];
	const float* inputBufferPointers[JucePlugin_MaxNumInputChannels];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	checkDelayBuffers();
    checkFilters();

	// copy input buffer and get read pointers
	inputBuffer.makeCopyOf(buffer);
	for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		inputBufferPointers[iChannel] = inputBuffer.getReadPointer(iChannel);

	// clear output buffers if less than #input channels used, the others will be overwritten later
	for (int i = speakerSet.size(); i < totalNumInputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

    int lowPassCount = 0;
    for (int i = 0; i < speakerSet.size(); i++)
    {
        if(!speakerSet.get(i)->getFilterBypass() && speakerSet.get(i)->getFilterInfo()->isLowPass())
            lowPassCount++;
    }
    int subwooferAmbisonicsOrder = lowPassCount >= 4 ? 1 : 0;
    int subwooferAmbisonicsChannelCount = lowPassCount >= 4 ? 4 : 1;
    
	for(int iSpeaker = 0; iSpeaker < speakerSet.size() && iSpeaker < totalNumOutputChannels; iSpeaker++)
	{
		AmbiSpeaker* pt = speakerSet.get(iSpeaker);
		if (pt == nullptr)
			buffer.clear(iSpeaker, 0, buffer.getNumSamples());
		else
		{
			// calculate ambisonics coefficients
			double speakerGain = pt->getGain();
			bool isSubwoofer = pt->getFilterBypass() && pt->getFilterInfo()->isLowPass();
            int currentAmbisonicsOrder = isSubwoofer ? subwooferAmbisonicsOrder : CURRENT_AMBISONICS_ORDER;
            int usedChannelCount = isSubwoofer ? subwooferAmbisonicsChannelCount : totalNumInputChannels;
            
			pt->getPoint()->getAmbisonicsCoefficients(JucePlugin_MaxNumInputChannels, &currentCoefficients[0], true, true);
			
			// gain of the W-signal depends on the used ambisonic order
            if(currentAmbisonicsOrder > 0)
                currentCoefficients[0] *= (currentAmbisonicsOrder / (2.0 * currentAmbisonicsOrder + 1));

			for (iChannel = 0; iChannel < usedChannelCount; iChannel++)
			{
				currentCoefficients[iChannel] *= ambiSettings.getAmbiChannelWeight(iChannel);
			}
			// apply to B-format and create output
			float* channelData = buffer.getWritePointer(iSpeaker);
			for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
			{
				float currentSample = 0.0f;
				for (iChannel = 0; iChannel < usedChannelCount; iChannel++)
					currentSample += float(speakerGain * inputBufferPointers[iChannel][iSample] * currentCoefficients[iChannel]);
				
				DelayBuffer* buf = delayBuffers[iSpeaker];
				if(buf != nullptr)
					channelData[iSample] = buf->processNextSample(currentSample);
			}
            
            pTestSoundGenerator->process(channelData, buffer.getNumSamples(), iSpeaker);

            // apply filters
            if(!pt->getFilterBypass() && pt->getFilterInfo()->anyActive())
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                {
                    for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
                    {
                        if (filterInfo[iSpeaker].get(iFilter)->filterType != FilterInfo::None)
                        {
                            channelData[iSample] = iirFilters[iSpeaker][iFilter].processSample(channelData[iSample]);
                        }
                    }
                }
            }

            auto analyzer = FFTAnalyzer::getInstance();
            if (analyzer->isActive(iSpeaker))
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                    analyzer->pushNextSampleIntoFifo(channelData[iSample]);
            }
		}
	}
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
    XmlElement* xml = new XmlElement("AMBISONICDECODERPLUGINSETTINGS");

	// save general decoder settings
	decoderSettings.saveToXml(xml);
	
    XmlElement* speakerSettings = new XmlElement("Points");
    speakerSet.writeToXmlElement(speakerSettings);
    
    XmlElement* ambiSettingsXml = new XmlElement("General");
    ambiSettings.writeToPresetXmlElement(ambiSettingsXml);
    
    XmlElement* presetSettings = new XmlElement("AmbisonicsPreset");
    presetSettings->addChildElement(speakerSettings);
    presetSettings->addChildElement(ambiSettingsXml);
    xml->addChildElement(presetSettings);

	copyXmlToBinary(*xml, destData);

	xml->deleteAllChildElements();
	delete xml;
}

void AmbisonicsDecoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
	{
		// make sure that it's actually our type of XML object..
		if (xmlState->hasTagName("AMBISONICDECODERPLUGINSETTINGS"))
		{
			// load general decoder settings
			decoderSettings.loadFromXml(xmlState.get());
			
            XmlElement* presetElement = xmlState->getChildByName("AmbisonicsPreset");
            if (presetElement != nullptr)
            {
                XmlElement* speakerXml = presetElement->getChildByName("Points");
                if(speakerXml != nullptr)
                {
                    speakerSet.loadFromXml(speakerXml);
                }
                
                XmlElement* ambiXml = presetElement->getChildByName("General");
                if(ambiXml != nullptr)
                {
                    ambiSettings.loadFromPresetXml(ambiXml);
                }
            }
		}
	}
}

AmbiSpeakerSet* AmbisonicsDecoderAudioProcessor::getSpeakerSet()
{
	return &speakerSet;
}

AmbiSourceSet* AmbisonicsDecoderAudioProcessor::getMovingPoints()
{
	return &movingPoints;
}

AmbiSettings* AmbisonicsDecoderAudioProcessor::getAmbiSettings()
{
	return &ambiSettings;
}

DecoderSettings* AmbisonicsDecoderAudioProcessor::getDecoderSettings()
{
	return &decoderSettings;
}

TestSoundGenerator* AmbisonicsDecoderAudioProcessor::getTestSoundGenerator() const
{
	return pTestSoundGenerator;
}

dsp::ProcessSpec* AmbisonicsDecoderAudioProcessor::getFilterSpecification()
{
	return &iirFilterSpec;
}

DecoderPresetHelper* AmbisonicsDecoderAudioProcessor::getPresetHelper()
{
    return presetHelper.get();
}

void AmbisonicsDecoderAudioProcessor::actionListenerCallback(const String &message)
{
    if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        presetHelper->loadFromXmlFile(presetFile, &speakerSet, &ambiSettings);
        presetHelper->notifyPresetChanged();
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicsDecoderAudioProcessor();
}
