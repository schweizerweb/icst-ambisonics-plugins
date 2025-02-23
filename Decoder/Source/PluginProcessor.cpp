/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../Common/FFTAnalyzer.h"

#define XML_ATTRIBUTE_VERSION "AmbiPluginVersion"
#define XML_ROOT_TAG "AMBISONICDECODERPLUGINSETTINGS"
#define XML_TAG_GENERAL "General"
#define XML_TAG_AMBISONICS_PRESET "AmbisonicsPreset"


//==============================================================================
AmbisonicsDecoderAudioProcessor::AmbisonicsDecoderAudioProcessor()
     : AudioProcessor (BusesProperties() // workaround for VST3 (for some strange reason, 64 channels are only allowed if not initialized with 64)
                       .withInput  ("Input",  ((PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_VST3) ? AudioChannelSet::discreteChannels(4) : AudioChannelSet::discreteChannels(64)), true)
                       .withOutput ("Output", ((PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_VST3) ? AudioChannelSet::discreteChannels(4) : AudioChannelSet::discreteChannels(64)), true)
                       )
{
    speakerSet.reset(new AmbiSpeakerSet(&scalingInfo));
    movingPoints.reset(new AmbiSourceSet(&scalingInfo));
    pTestSoundGenerator = new TestSoundGenerator(speakerSet.get());
    
    zoomSettings.reset(new ZoomSettings(getScalingInfo()));

    speakerPresetHelper.reset(new SpeakerPresetHelper(File(Constants::getBasePresetsDirectory()), this, &scalingInfo));
    speakerPresetHelper->initialize();
    speakerPresetHelper->loadDefaultPreset(speakerSet.get());

    decodingPresetHelper.reset(new DecodingPresetHelper(File(Constants::getBasePresetsDirectory() + "/Decoding"), this));
    decodingPresetHelper->initialize();
    decodingPresetHelper->loadDefaultPreset(&ambiSettings);

    radarOptions.setTrackColorAccordingToName = false;
    radarOptions.maxNumberEditablePoints = getBusesLayout().getMainOutputChannels();
    radarOptions.editablePointsAsSquare = true;
    radarOptions.scalingInfo = getScalingInfo();
    radarOptions.zoomSettings = getZoomSettingsPointer();
    radarOptions.allowGroup = false;
    radarOptions.allowDelete = false;
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
    
    speakerIIRFilterSpec.numChannels = 1;
    speakerIIRFilterSpec.maximumBlockSize = (uint32)samplesPerBlock;
    speakerIIRFilterSpec.sampleRate = sampleRate;

    bFormatIIRFilterSpec.numChannels = MAX_NB_OF_AMBISONICS_CHANNELS;
    bFormatIIRFilterSpec.maximumBlockSize = (uint32)samplesPerBlock;
    bFormatIIRFilterSpec.sampleRate = sampleRate;
}

void AmbisonicsDecoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AmbisonicsDecoderAudioProcessor::checkDelayBuffers()
{
	int size = speakerSet->size();
	// add or remove delay buffers if number of speakers changed
	while(size > delayBuffers.size())
	{
		delayBuffers.add(new DelayBuffer());
	}
	if (size < delayBuffers.size())
		delayBuffers.removeLast(delayBuffers.size() - size);

	// check delays
	double maxDist = speakerSet->getMaxNormalizedDistance();
	for (int i = 0; i < size && i < speakerSet->size(); i++)
	{
		AmbiPoint* pt = speakerSet->get(i);
		if (pt != nullptr)
		{
			int requiredDelay = delayHelper.getDelayCompensationSamples(pt->getRawPoint()->getDistance(), maxDist, getSampleRate());
			delayBuffers.getUnchecked(i)->checkAndAdjustSize(requiredDelay);
		}
	}
}

bool AmbisonicsDecoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    int numIn = layouts.getMainInputChannels();
    int numOut = layouts.getMainOutputChannels();
    if (numIn < 4 || (numIn > 16 && numIn != 64))
        return false;

    if (numOut < 1 || (numOut > 24 && numOut != 64))
        return false;

    return true;
}

void AmbisonicsDecoderAudioProcessor::updateBFormatFilters()
{
    for (int iDec = 0; iDec < MAX_NB_OF_DECODER_SECTIONS; iDec++)
    {
        if (!ambiSettings.multiDecoderSections[iDec].filterInfo.getFilterBypass())
        {
            auto filterBankInfo = &(ambiSettings.multiDecoderSections[iDec].filterInfo);

            for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
            {
                FilterInfo* pFilter = filterBankInfo->get(iFilter);
                if (pFilter == nullptr)
                {
                    Logger::writeToLog("Error in Filterbank!");
                    return;
                }
                if (!bFormatFilterInfo[iDec].get(iFilter)->equals(pFilter))
                {
                    if (bFormatFilterInfo[iDec].get(iFilter)->filterType != pFilter->filterType)
                    {
                        bFormatFilter[iDec][iFilter].prepare(bFormatIIRFilterSpec);
                        bFormatFilter[iDec][iFilter].reset();
                    }

                    bFormatFilterInfo[iDec].get(iFilter)->copyFrom(pFilter);
                    auto newCoeff = pFilter->getCoefficients(bFormatIIRFilterSpec.sampleRate);
                    if (newCoeff != nullptr && newCoeff->coefficients != bFormatFilter[iDec][iFilter].state->coefficients)
                    {
                        bFormatFilter[iDec][iFilter].state->coefficients = newCoeff->coefficients;
                    }
                } 
            }
        }
        else
        {
            bFormatFilterInfo[iDec].setFilterBypass(true);
        }
    }

}

void AmbisonicsDecoderAudioProcessor::checkSpeakerFilters()
{
    int size = speakerSet->size();

    for(int iSpeaker = 0; iSpeaker < size; iSpeaker++)
    {
        if (!speakerSet->get(iSpeaker)->getFilterBypass())
        {
            for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
            {
                FilterInfo* pFilter = speakerSet->get(iSpeaker)->getFilterInfo()->get(iFilter);
                if (pFilter == nullptr)
                {
                    Logger::writeToLog("Error in Filterbank!");
                    return;
                }
                if (!speakerFilterInfo[iSpeaker].get(iFilter)->equals(pFilter) && iSpeaker < MAX_NUM_CHANNELS)
                {
                    if (speakerFilterInfo[iSpeaker].get(iFilter)->filterType != pFilter->filterType)
                    {
                        speakerIIRFilters[iSpeaker][iFilter].prepare(speakerIIRFilterSpec);
                        speakerIIRFilters[iSpeaker][iFilter].reset();
                    }

                    speakerFilterInfo[iSpeaker].get(iFilter)->copyFrom(pFilter);
                    auto newCoeff = pFilter->getCoefficients(speakerIIRFilterSpec.sampleRate);
                    if (newCoeff != nullptr)
                    {
                        speakerIIRFilters[iSpeaker][iFilter].coefficients = newCoeff;
                    }
                }
            }
        }
    }
}

void AmbisonicsDecoderAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    const int totalNumInputChannels  = jmin(getTotalNumInputChannels(), ambiSettings.getMaxUsedChannelCount(), buffer.getNumChannels());
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const bool isMultiDecoderMode = ambiSettings.getMultiDecoderFlag();
    double currentCoefficients[MAX_NUM_CHANNELS];
	const float* inputBufferPointers[MAX_NB_OF_DECODER_SECTIONS][MAX_NUM_CHANNELS];
	int iChannel;

	std::vector<AudioSampleBuffer> multiInputBuffers;
	std::vector<AudioSampleBuffer*> inputBuffers;
    std::vector<AmbiSettings*> ambiSettingsVector;
    std::vector<double> decoderGains;
    std::vector<unsigned long long> speakerMaskVector;
    auto analyzer = FFTAnalyzer::getInstance();

    // clear output buffers if less than #input channels used, the others will be overwritten later
    for (int i = speakerSet->size(); i < totalNumInputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

	checkDelayBuffers();
    checkSpeakerFilters();

    if (isMultiDecoderMode)
    {
        updateBFormatFilters();
        int nbDec = ambiSettings.getUsedDecoderCount();
        int bufferPointerIndex = 0;
        for (int iDec = 0; iDec < nbDec; iDec++)
        {
            if (!ambiSettings.multiDecoderSections[iDec].mute)
            {
                // make copy of input buffer and add to processing list
                AudioSampleBuffer inputBuffer;
                inputBuffer.makeCopyOf(buffer);
                dsp::AudioBlock <float> block(inputBuffer);
                for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
                {
                    if (!ambiSettings.multiDecoderSections[iDec].filterInfo.getFilterBypass() && bFormatFilterInfo[iDec].isActive(iFilter))
                    {
                        bFormatFilter[iDec][iFilter].process(dsp::ProcessContextReplacing<float>(block));
                    }
                }

                multiInputBuffers.push_back(inputBuffer);
                inputBuffers.push_back(&multiInputBuffers.back());
                ambiSettingsVector.push_back(&ambiSettings.multiDecoderSections[iDec].ambiSettings);
                decoderGains.push_back(ambiSettings.multiDecoderSections[iDec].gain);
                speakerMaskVector.push_back(ambiSettings.multiDecoderSections[iDec].speakerMask);

                // get read pointers
                for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
                    inputBufferPointers[bufferPointerIndex][iChannel] = inputBuffers[bufferPointerIndex]->getReadPointer(iChannel);

                if (analyzer->isActive(FFT_INDEX_OFFSET_MULTIDECODER + iDec))
                {
                    for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                        analyzer->pushNextSampleIntoFifo(inputBufferPointers[bufferPointerIndex][0][iSample]);  // push W-signal to FFT-Analyzer
                }

                bufferPointerIndex++;
            }
        }
    }
    else
    {
        // make copy of input buffer and add to processing list
        AudioSampleBuffer inputBuffer;
        inputBuffer.makeCopyOf(buffer);
        multiInputBuffers.push_back(inputBuffer);
        inputBuffers.push_back(&multiInputBuffers.back());
        ambiSettingsVector.push_back(ambiSettings.singleDecoder.get());
        decoderGains.push_back(1.0);
        speakerMaskVector.push_back(0xffffffffffffffff);

        // get read pointers
        for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
            inputBufferPointers[0][iChannel] = inputBuffers[0]->getReadPointer(iChannel);
    }

    bool soloOnly = speakerSet->anySolo();

    for(int iSpeaker = 0; iSpeaker < speakerSet->size() && iSpeaker < totalNumOutputChannels; iSpeaker++)
	{
		AmbiSpeaker* pt = speakerSet->get(iSpeaker);
        if (pt == nullptr || ((pt->getMute() || soloOnly) && !pt->getSolo()))
        {
            buffer.clear(iSpeaker, 0, buffer.getNumSamples());
            if (analyzer->isActive(iSpeaker))
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                    analyzer->pushNextSampleIntoFifo(0);
            }
        }
		else
		{
            float* channelData = buffer.getWritePointer(iSpeaker);
            AudioSampleBuffer sumBuf(1, buffer.getNumSamples());
            sumBuf.clear();

            float* sumData = sumBuf.getWritePointer(0);
            for (int iDec = 0; iDec < inputBuffers.size(); iDec++)
            {
                AmbiSettings* pAmbi = ambiSettingsVector[iDec];
                if (!(speakerMaskVector[iDec] & (static_cast<unsigned long long>(1) << (iSpeaker))))
                    continue;

                // calculate ambisonics coefficients
                double speakerGain = pt->getGain();
                int currentAmbisonicsOrder = pAmbi->getAmbiOrder();
                int usedChannelCount = jmin(totalNumInputChannels, ((pAmbi->getAmbiOrder() + 1) * (pAmbi->getAmbiOrder() + 1)));

                pt->getRawPoint()->getAmbisonicsCoefficients(channelLayout.getNumInputChannels(), &currentCoefficients[0], true, true);

                // gain of the W-signal depends on the used ambisonic order
                if (currentAmbisonicsOrder > 0)
                    currentCoefficients[0] *= (currentAmbisonicsOrder / (2.0 * currentAmbisonicsOrder + 1));

                for (iChannel = 0; iChannel < usedChannelCount; iChannel++)
                {
                    currentCoefficients[iChannel] *= pAmbi->getAmbiChannelWeight(iChannel);
                }
                // apply to B-format and create output
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                {
                    double currentSample = 0.0;
                    for (iChannel = 0; iChannel < usedChannelCount; iChannel++)
                        currentSample += speakerGain * inputBufferPointers[iDec][iChannel][iSample] * currentCoefficients[iChannel];
                    
                    sumData[iSample] += float(decoderGains[iDec] * currentSample);
                }
            }
            
            // apply delay
            DelayBuffer* buf = delayBuffers[iSpeaker];
            if (buf != nullptr)
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                {
                    channelData[iSample] = buf->processNextSample(sumData[iSample]);
                }
            }

            // inject test sound
            pTestSoundGenerator->process(channelData, buffer.getNumSamples(), iSpeaker);

            // apply speaker filters
            if(!pt->getFilterBypass() && pt->getFilterInfo()->anyActive())
            {
                for (int iSample = 0; iSample < buffer.getNumSamples(); iSample++)
                {
                    for (int iFilter = 0; iFilter < MAX_FILTER_COUNT; iFilter++)
                    {
                        if (speakerFilterInfo[iSpeaker].get(iFilter)->filterType != FilterInfo::None)
                        {
                            channelData[iSample] = speakerIIRFilters[iSpeaker][iFilter].processSample(channelData[iSample]);
                        }
                    }
                }
            }

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
    XmlElement* xml = new XmlElement(XML_ROOT_TAG);
    xml->setAttribute(XML_ATTRIBUTE_VERSION, ProjectInfo::versionNumber);
    
	// save general decoder settings
	decoderSettings.saveToXml(xml);
	
    speakerSet->writeToXmlElement(xml);
    
    XmlElement* ambiSettingsXml = new XmlElement(XML_TAG_GENERAL);
    ambiSettings.writeToPresetXmlElement(ambiSettingsXml);
    
    XmlElement* presetSettings = new XmlElement(XML_TAG_AMBISONICS_PRESET);
    presetSettings->addChildElement(ambiSettingsXml);
    xml->addChildElement(presetSettings);
    
    zoomSettings->writeToXmlElement(xml);

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
		if (xmlState->hasTagName(XML_ROOT_TAG))
		{
            int versionNumber = xmlState->getIntAttribute(XML_ATTRIBUTE_VERSION, 0);
            
            if(versionNumber > ProjectInfo::versionNumber)
            {
                AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Outdated AmbiDecoder version", "The state to be loaded has been saved with a newer version of the AmbiDecoder plugin");
            }
            // future implementation of backward compatibility
            
			// load general decoder settings
			decoderSettings.loadFromXml(xmlState.get());
			
            XmlElement* presetElement = xmlState->getChildByName(XML_TAG_AMBISONICS_PRESET);
            if (presetElement != nullptr)
            {
                speakerSet->loadFromXml(xmlState.get());
                
                XmlElement* ambiXml = presetElement->getChildByName(XML_TAG_GENERAL);
                if(ambiXml != nullptr)
                {
                    ambiSettings.loadFromPresetXml(ambiXml);
                }
            }
            
            zoomSettings->loadFromXml(xmlState.get());
		}
	}
}

AmbiSpeakerSet* AmbisonicsDecoderAudioProcessor::getSpeakerSet()
{
	return speakerSet.get();
}

AmbiSourceSet* AmbisonicsDecoderAudioProcessor::getMovingPoints()
{
	return movingPoints.get();
}

AmbiSettingsCollection* AmbisonicsDecoderAudioProcessor::getAmbiSettings()
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
	return &speakerIIRFilterSpec;
}

SpeakerPresetHelper* AmbisonicsDecoderAudioProcessor::getSpeakerPresetHelper()
{
    return speakerPresetHelper.get();
}

DecodingPresetHelper* AmbisonicsDecoderAudioProcessor::getDecodingPresetHelper()
{
    return decodingPresetHelper.get();
}

ScalingInfo* AmbisonicsDecoderAudioProcessor::getScalingInfo()
{
    return &scalingInfo;
}

ZoomSettings* AmbisonicsDecoderAudioProcessor::getZoomSettingsPointer()
{
    return zoomSettings.get();
}

ChannelLayout* AmbisonicsDecoderAudioProcessor::getChannelLayout()
{
    return &channelLayout;
}

RadarOptions* AmbisonicsDecoderAudioProcessor::getRadarOptions()
{
    return &radarOptions;
}

void AmbisonicsDecoderAudioProcessor::actionListenerCallback(const String &message)
{
    if(message.startsWith(speakerPresetHelper->UniqueActionMessageSelectPreset()))
    {
        File presetFile(message.substring(speakerPresetHelper->UniqueActionMessageSelectPreset().length()));
        speakerPresetHelper->loadFromXmlFile(presetFile, speakerSet.get());
        speakerPresetHelper->notifyPresetChanged();
    }
    else if(message.startsWith(decodingPresetHelper->UniqueActionMessageSelectPreset()))
    {
        File presetFile(message.substring(decodingPresetHelper->UniqueActionMessageSelectPreset().length()));
        decodingPresetHelper->loadFromXmlFile(presetFile, &ambiSettings);
        decodingPresetHelper->notifyPresetChanged();
    }
}

void AmbisonicsDecoderAudioProcessor::numChannelsChanged()
{
    // handle output channels
    channelLayout.setNumChannels(getBusesLayout().getMainInputChannels(), getBusesLayout().getMainOutputChannels());
    
    int maxAmbiOrder = channelLayout.getMaxAmbiOrder(false);
    ambiSettings.ensureMaxAmbiOrder(maxAmbiOrder);
    
    // handle output channels
    // while(channelLayout.getNumOutputChannels() < speakerSet->size())
    //     speakerSet->remove(speakerSet->size() - 1);
    
    // radarOptions.maxNumberEditablePoints = channelLayout.getNumOutputChannels();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicsDecoderAudioProcessor();
}
