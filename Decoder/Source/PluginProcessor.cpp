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
	pTestSoundGenerator = new TestSoundGenerator(&speakerSet);
}

AmbisonicsDecoderAudioProcessor::~AmbisonicsDecoderAudioProcessor()
{
	delete pTestSoundGenerator;
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
    
    // IIR filter
    while(size > iirFilters.size())
    {
        dsp::IIR::Filter<float>* newFilter = new dsp::IIR::Filter<float>();
        newFilter->prepare(iirFilterSpec);
        newFilter->reset();
        iirFilters.add(newFilter);
    }
        
    while(size < iirFilters.size())
    {
        iirFilters.removeLast();
    }
    
	for(int i = 0; i < iirFilters.size() && i < speakerSet.size(); i++)
	{
		FilterInfo* filterInfo = speakerSet.get(i)->getFilterInfo();
		switch (filterInfo->filterType)
		{
		case FilterInfo::LowPass: 
			iirFilters[i]->coefficients = dsp::IIR::Coefficients<float>::makeLowPass(iirFilterSpec.sampleRate, filterInfo->cutOffFrequencyHz, filterInfo->qValue);
			break;
		case FilterInfo::BandPass: 
			iirFilters[i]->coefficients = dsp::IIR::Coefficients<float>::makeBandPass(iirFilterSpec.sampleRate, filterInfo->cutOffFrequencyHz, filterInfo->qValue);
			break;
		case FilterInfo::HighPass:
			iirFilters[i]->coefficients = dsp::IIR::Coefficients<float>::makeHighPass(iirFilterSpec.sampleRate, filterInfo->cutOffFrequencyHz, filterInfo->qValue);
			break;

		case FilterInfo::None:
		default: 
			break;
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
        if(speakerSet.get(i)->getFilterInfo()->filterType == FilterInfo::LowPass)
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
			bool isSubwoofer = pt->getFilterInfo()->filterType == FilterInfo::LowPass;
            int currentAmbisonicsOrder = isSubwoofer ? subwooferAmbisonicsOrder : CURRENT_AMBISONICS_ORDER;
            int usedChannelCount = isSubwoofer ? subwooferAmbisonicsChannelCount : totalNumInputChannels;
            
			pt->getPoint()->getAmbisonicsCoefficients(JucePlugin_MaxNumInputChannels, &currentCoefficients[0], !ambiSettings.getDirectionFlip(), true);
			
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
            
            if(pt->getFilterInfo()->filterType != FilterInfo::None)
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                {
                    channelData[iSample] = iirFilters[iSpeaker]->processSample(channelData[iSample]);
                }
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
	
	// load last speaker preset
	PresetInfo preset;
	preset.setName("LastState");
	for (int i = 0; i < speakerSet.size(); i++)
	{
		AmbiSpeaker* pt = speakerSet.get(i);
		if(pt != nullptr)
			preset.getPoints()->add(new AmbiSpeaker(pt));
	}
	preset.getAmbiSettings()->setDistanceScaler(ambiSettings.getDistanceScaler());
	preset.getAmbiSettings()->setDirectionFlip(ambiSettings.getDirectionFlip());
	for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
		preset.getAmbiSettings()->getAmbiOrderWeightPointer()[i] = ambiSettings.getAmbiOrderWeightPointer()[i];
	XmlElement* speakerSettings = new XmlElement(XML_TAG_PRESET_ROOT);
	preset.CreateXmlRoot(speakerSettings);
	xml->addChildElement(speakerSettings);

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
			
			// load last speaker preset
			PresetInfo preset;
			XmlElement* presetElement = xmlState->getChildByName(XML_TAG_PRESET_ROOT);
			if (presetElement != nullptr && preset.LoadFromXmlRoot(presetElement))
			{
				speakerSet.clear();
				for (int i = 0; i < preset.getPoints()->size(); i++)
				{
					speakerSet.add(new AmbiSpeaker(preset.getPoints()->getUnchecked(i)));
				}
				ambiSettings.setDistanceScaler(preset.getAmbiSettings()->getDistanceScaler());
				ambiSettings.setDirectionFlip(preset.getAmbiSettings()->getDirectionFlip());

				for (int i = 0; i < NB_OF_AMBISONICS_GAINS; i++)
				{
					ambiSettings.getAmbiOrderWeightPointer()[i] = preset.getAmbiSettings()->getAmbiOrderWeightPointer()[i];
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicsDecoderAudioProcessor();
}
