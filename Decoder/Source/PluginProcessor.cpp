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

    presetHelper.reset(new DecoderPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiDecoder"), this, &scalingInfo));
    presetHelper->initialize();
    presetHelper->loadDefaultPreset(speakerSet.get(), &ambiSettings);
    
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
    
    iirFilterSpec.numChannels = 1;
    iirFilterSpec.maximumBlockSize = (uint32)samplesPerBlock;
    iirFilterSpec.sampleRate = sampleRate;
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

void AmbisonicsDecoderAudioProcessor::checkFilters()
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
                if (!filterInfo[iSpeaker].get(iFilter)->equals(pFilter) && iSpeaker < MAX_NUM_CHANNELS)
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

void AmbisonicsDecoderAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
    const int totalNumInputChannels  = jmin(getTotalNumInputChannels(), ambiSettings.getAmbiChannelCount(), buffer.getNumChannels());
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    double currentCoefficients[MAX_NUM_CHANNELS];
	const float* inputBufferPointers[MAX_NUM_CHANNELS];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	checkDelayBuffers();
    checkFilters();

	// copy input buffer and get read pointers
	inputBuffer.makeCopyOf(buffer);
	for (iChannel = 0; iChannel < totalNumInputChannels; iChannel++)
		inputBufferPointers[iChannel] = inputBuffer.getReadPointer(iChannel);

	// clear output buffers if less than #input channels used, the others will be overwritten later
	for (int i = speakerSet->size(); i < totalNumInputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

    int lowPassCount = 0;
    for (int i = 0; i < speakerSet->size(); i++)
    {
        if(!speakerSet->get(i)->getFilterBypass() && speakerSet->get(i)->getFilterInfo()->isLowPass())
            lowPassCount++;
    }
    int subwooferAmbisonicsOrder = lowPassCount >= 4 ? 1 : 0;
    int subwooferAmbisonicsChannelCount = lowPassCount >= 4 ? 4 : 1;
    
	for(int iSpeaker = 0; iSpeaker < speakerSet->size() && iSpeaker < totalNumOutputChannels; iSpeaker++)
	{
		AmbiSpeaker* pt = speakerSet->get(iSpeaker);
		if (pt == nullptr || pt->getMute())
			buffer.clear(iSpeaker, 0, buffer.getNumSamples());
		else
		{
			// calculate ambisonics coefficients
			double speakerGain = pt->getGain();
			bool isSubwoofer = pt->getFilterBypass() && pt->getFilterInfo()->isLowPass();
            int currentAmbisonicsOrder = isSubwoofer ? subwooferAmbisonicsOrder : ambiSettings.getAmbiOrder();
            int usedChannelCount = isSubwoofer ? subwooferAmbisonicsChannelCount : ((ambiSettings.getAmbiOrder() + 1) * (ambiSettings.getAmbiOrder() + 1));
            
			pt->getRawPoint()->getAmbisonicsCoefficients(channelLayout.getNumInputChannels(), &currentCoefficients[0], true, true);
			
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
    if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        presetHelper->loadFromXmlFile(presetFile, speakerSet.get(), &ambiSettings);
        presetHelper->notifyPresetChanged();
    }
}

void AmbisonicsDecoderAudioProcessor::numChannelsChanged()
{
    // handle output channels
    channelLayout.setNumChannels(getBusesLayout().getMainInputChannels(), getBusesLayout().getMainOutputChannels());
    
    int maxAmbiOrder = channelLayout.getMaxAmbiOrder(false);
    if(ambiSettings.getAmbiOrder() > maxAmbiOrder)
    {
        ambiSettings.setAmbiOrder(maxAmbiOrder);
    }
    
    // handle output channels
    while(channelLayout.getNumOutputChannels() < speakerSet->size())
        speakerSet->remove(speakerSet->size() - 1);
    
    radarOptions.maxNumberEditablePoints = channelLayout.getNumOutputChannels();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicsDecoderAudioProcessor();
}
