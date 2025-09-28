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
#include "../../Common/TrackColors.h"
#include "../../Common/EncoderConstants.h"

#define XML_ROOT_TAG "AMBISONICENCODERPLUGINSETTINGS"
#define XML_TAG_ENCODER_SETTINGS "EncoderSettings"
#define XML_TAG_ENCODER_ANIMATOR "Animator"
#define XML_ATTRIBUTE_VERSION "AmbiPluginVersion"

//==============================================================================
AmbisonicEncoderAudioProcessor::AmbisonicEncoderAudioProcessor()
     : AudioProcessor (BusesProperties() // workaround for VST3 (for some strange reason, 64 channels are only allowed if not initialized with 64)
#if MULTI_ENCODER_MODE
                       .withInput ("Input", ((PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_VST3) ? AudioChannelSet::discreteChannels(4) : AudioChannelSet::discreteChannels(64)), true)
#else
                       .withInput  ("Input", AudioChannelSet::mono(), true)
#endif
                       .withOutput ("Output", ((PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_VST3) ? AudioChannelSet::discreteChannels(4) : AudioChannelSet::discreteChannels(64)), true)
                       )
{
    channelLayout.setNumChannels(getBusesLayout().getMainInputChannels(), getBusesLayout().getMainOutputChannels());
    
    sources.reset(new AmbiSourceSet(getScalingInfo()));
	pOscHandler = new OSCHandlerEncoder(sources.get(), &statusMessageHandler, &encoderSettings, getScalingInfo());
	pOscSender = new AmbiOSCSender(sources.get());
	pOscSenderExt = new AmbiOSCSenderExt(sources.get(), &statusMessageHandler, getScalingInfo());

    pOscHandler->initialize();
	initializeOscSender();
    initializeAudioParameter();
    
    zoomSettings.reset(new ZoomSettings(getScalingInfo()));
    presetHelper.reset(new EncoderPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder"), this, getScalingInfo()));
    presetHelper->initialize();
	distanceEncodingPresetHelper.reset(new DistanceEncodingPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder/DistanceEncoding"), this));
	distanceEncodingPresetHelper->initialize();
    customOscRxPresetHelper.reset(new CustomOscRxPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder/CustomOscRx"), this));
    customOscRxPresetHelper->initialize();
    customOscTxPresetHelper.reset(new CustomOscTxPresetHelper(File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder/CustomOscTx"), this));
    customOscTxPresetHelper->initialize();

#if(!MULTI_ENCODER_MODE)
    // initialize mono encoder with one source
    if (sources->size() == 0)
    {
		scalingInfo.SetScaler(1.0);
        String name = dawParameter.updateTrackPropertiesWorking ? dawParameter.lastTrackProperties.name.value_or("1") : "1";
        Colour color = dawParameter.updateTrackPropertiesWorking ? dawParameter.lastTrackProperties.colour.value_or(TrackColors::getColor(0)) : TrackColors::getColor(0);
        sources->addNew(Uuid().toString(), Point3D<double>(0.0, 0.0, 0.0, audioParams.sourceParams[0]), name, color);
    }
#else
    if(!presetHelper->loadDefaultPreset(&audioParams, sources.get()))
    {
        // do not do anything because it might block the user interface on initialization
		//AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Default preset", "Default preset not found, please restore presets using the Preset Manager!");
    }
#endif
    
    radarOptions.setTrackColorAccordingToName = !MULTI_ENCODER_MODE;
    radarOptions.maxNumberEditablePoints = channelLayout.getNumInputChannels();
    radarOptions.editablePointsAsSquare = false;
    radarOptions.audioParams = getAudioParams();
    radarOptions.dawParameter = getDawParameter();
    radarOptions.scalingInfo = getScalingInfo();
    radarOptions.zoomSettings = getZoomSettingsPointer();
    radarOptions.checkNameFieldEditable = !MULTI_ENCODER_MODE;
    radarOptions.allowGroup = MULTI_ENCODER_MODE;
    radarOptions.allowDelete = MULTI_ENCODER_MODE;
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
    sources->initialize(this);
#endif
	encoderSettings.distanceEncodingParams.initialize(this);
    audioParams.initialize(this, getScalingInfo(), sources.get(), MAXIMUM_NUMBER_OF_GROUPS);
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
	iirFilterSpec.maximumBlockSize = (uint32_t)samplesPerBlock;
	iirFilterSpec.sampleRate = sampleRate;
}

void AmbisonicEncoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AmbisonicEncoderAudioProcessor::numChannelsChanged()
{
//    AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, "Num channels changed", "New channel count: " + String(getBusesLayout().getMainInputChannels()) + "/" + String(getBusesLayout().getMainOutputChannels()) + " ---- can change layout: ");

    // handle output channels
    channelLayout.setNumChannels(getBusesLayout().getMainInputChannels(), getBusesLayout().getMainOutputChannels());
    
    int maxAmbiOrder = channelLayout.getMaxAmbiOrder(true);
    if(encoderSettings.ambiOrder > maxAmbiOrder)
    {
        encoderSettings.ambiOrder = maxAmbiOrder;
    }
    
    // handle input channels
    for(int i = channelLayout.getNumInputChannels(); i < sources->size(); i++)
        sources->get(i)->setEnabled(false);
    
    radarOptions.maxNumberEditablePoints = channelLayout.getNumInputChannels();
}

bool AmbisonicEncoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    int numOut = layouts.getMainOutputChannels();
    int numIn = layouts.getMainInputChannels();
    if(numOut < 4 || (numOut > 16 && numOut != 64))
        return false;

    if(numIn < 1 || (numIn > 24 && numIn != 64))
        return false;
    
#if(!MULTI_ENCODER_MODE)
    if(layouts.getMainInputChannels() != 1)
        return false;
#endif
    
    return true;
}

void AmbisonicEncoderAudioProcessor::applyDistanceGain(double* pCoefficientArray, int arraySize, double distance) const
{
	if (!encoderSettings.distanceEncodingFlag || approximatelyEqual(encoderSettings.distanceEncodingParams.getUnitCircleRadius(), 0.0f))
		return;

	double wFactor, otherFactor;
	encoderSettings.distanceEncodingParams.calculateAttenuation(distance, &wFactor, &otherFactor);

	pCoefficientArray[0] *= wFactor;
	for (int i = 1; i < arraySize; i++)
		pCoefficientArray[i] *= otherFactor;
}

void AmbisonicEncoderAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    // playhead handling
    playheadState.sampleRate.store (getSampleRate(), std::memory_order_relaxed);

    if (auto* ph = getPlayHead())
    {
        if (auto pos = ph->getPosition())
        {
            if (pos->getTimeInSeconds().hasValue())
                playheadState.timeSeconds.store (*pos->getTimeInSeconds());

            if (pos->getBpm().hasValue())
                playheadState.bpm.store (*pos->getBpm());
                
            playheadState.playing.store (pos->getIsPlaying());
            
            playheadState.looping.store (pos->getIsLooping(), std::memory_order_relaxed);

                if (playheadState.looping.load(std::memory_order_relaxed))
                {
                    if (auto lp = pos->getLoopPoints())
                    {
                        // lp->ppqStart / lp->ppqEnd sind *PPQ* (quarter-notes)
                        double bpm = 120.0;
                        if (auto b = pos->getBpm()) bpm = *b;

                        const double qps = bpm / 60.0; // quarters per second

                        const double ppqStart = lp->ppqStart;
                        const double ppqEnd   = lp->ppqEnd;

                        if (qps > 0.0 && std::isfinite(ppqStart) && std::isfinite(ppqEnd) && ppqEnd > ppqStart)
                        {
                            playheadState.loopStartSeconds.store (ppqStart / qps, std::memory_order_relaxed);
                            playheadState.loopEndSeconds.store   (ppqEnd   / qps, std::memory_order_relaxed);
                        }
                    }
                }

            playheadState.valid.store (true, std::memory_order_release);
        }
        else
        {
            playheadState.valid.store (false, std::memory_order_release);
        }
    }
    else
    {
        playheadState.valid.store (false, std::memory_order_release);
    }
    
	// Audio handling
    const float masterGainFactor = float(Decibels::decibelsToGain(sources->getMasterGain()));
	const int totalNumInputChannels = jmin(getTotalNumInputChannels(), sources->size(), buffer.getNumChannels());
	const int totalUsedOutputChannels = jmin(getTotalNumOutputChannels(), encoderSettings.getAmbiChannelCount(), buffer.getNumChannels());
	double currentCoefficients[64];
	float* outputBufferPointers[64];
	int iChannel;
	AudioSampleBuffer inputBuffer;

	// copy input buffer, clear output and get write pointers
	inputBuffer.makeCopyOf(buffer);
	buffer.clear();
    
    // in case of scaling change with doppler enabled, send an empty buffer and reset delay buffers
    double newScaler = scalingInfo.GetScaler();
    if(encoderSettings.dopplerEncodingFlag && !approximatelyEqual(lastScaler, newScaler))
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
	for (iChannel = 0; iChannel < totalUsedOutputChannels; iChannel++)
		outputBufferPointers[iChannel] = buffer.getWritePointer(iChannel);
	
    bool soloOnly = sources->anySolo();
    
    // loop through input channels
	for (int iSource = 0; iSource < totalNumInputChannels; iSource++)
	{
		AmbiSource* source = sources->get(iSource);
		if (source == nullptr
            || !source->getEnabled()
            || ((source->getMute() || soloOnly) && !source->getSolo()))
			continue;

        Vector3D<double> sourceVector = sources->getAbsSourcePoint(iSource);
        Point3D<double> sourcePoint(sourceVector.x, sourceVector.y, sourceVector.z);
        double sourcePointDistance = sourcePoint.getDistance();
        
		// air absorbtion filter
		if (encoderSettings.distanceEncodingFlag && airAbsorbtionFilters[iSource].checkFilter(&encoderSettings.distanceEncodingParams, sourcePointDistance, &iirFilterSpec))
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
        memset(currentCoefficients, 0, 64 * sizeof(double));
		sourcePoint.getAmbisonicsCoefficients(encoderSettings.getAmbiChannelCount(), &currentCoefficients[0], true, true);
		applyDistanceGain(&currentCoefficients[0], 64, sourcePointDistance);
		
		if (encoderSettings.dopplerEncodingFlag)
		{
			delayBuffers[iSource].check(int(DelayHelper::getDelaySamples(scalingInfo.GetScaler() * MathConstants<float>::sqrt2, getSampleRate())));
            localBuffer.copyFrom(0, 0, inputBuffer, iSource, 0, inputBuffer.getNumSamples());
			// check doppler delay buffers
			float currentDelayInSamples = DelayHelper::getDelaySamples(sourcePointDistance, getSampleRate());
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
			for (iChannel = 0; iChannel < totalUsedOutputChannels; iChannel++)
				outputBufferPointers[iChannel][iSample] += sourceGain * masterGainFactor * float(inputData[iSample] * (fractionNew * currentCoefficients[iChannel] + fractionOld * lastCoefficients[iSource][iChannel]));
		}

		// keep coefficients
		memcpy(&lastCoefficients[iSource], &currentCoefficients, 64 * sizeof(double));
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
    xml->setAttribute(XML_ATTRIBUTE_VERSION, ProjectInfo::versionNumber);
    
	// save general encoder settings
	xml->addChildElement(encoderSettings.getAsXmlElement(XML_TAG_ENCODER_SETTINGS));
	sources->writeToXmlElement(xml);
    zoomSettings->writeToXmlElement(xml);
    xml->addChildElement(animatorDataset.getAsXmlElement(XML_TAG_ENCODER_ANIMATOR));
    
    auto* xTimelines = xml->createNewChildElement("Timelines");
    for (auto* tm : timelines)
        xTimelines->addChildElement (tm->toXml().release());

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
            int versionNumber = xmlState->getIntAttribute(XML_ATTRIBUTE_VERSION, 0);
            
            if(versionNumber > ProjectInfo::versionNumber)
            {
                AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Outdated AmbiEncoder version", "The state to be loaded has been saved with a newer version of the AmbiEncoder plugin");
            }
            // future implementation of backward compatibility
            
			// load general encoder settings
			encoderSettings.loadFromXml(xmlState->getChildByName(XML_TAG_ENCODER_SETTINGS));
            // load last source preset
			sources->loadFromXml(xmlState.get(), &audioParams);
            sources->resetIds();
            scalingInfo.SetScaler(sources->getDistanceScaler());
            zoomSettings->loadFromXml(xmlState.get());
            animatorDataset.loadFromXml(xmlState->getChildByName(XML_TAG_ENCODER_ANIMATOR));
            if (auto* xTimelines = xmlState->getChildByName("Timelines"))
            {
                timelines.clear(true);
                forEachXmlChildElementWithTagName (*xTimelines, xTimeline, "Timeline")
                {
                    auto* tm = new TimelineModel();
                    tm->fromXml (*xTimeline);
                    timelines.add (tm);
                }
                //listeners.call (&Listener::timelineListChanged);
            }
            if (timelines.isEmpty())
            {
                populateDefaultTimelineModels();
            }
		}
	}

    pOscHandler->initialize();
	initializeOscSender();
}

static inline MovementClip makeMovementClip(juce::String id, ms_t start, ms_t length, juce::Colour col)
{
    MovementClip c;
    c.id = std::move(id);
    c.start = start;
    c.length = length;
    c.colour = col;
    c.useStartPoint = false;
    // Initialize point arrays with default values if needed
    return c;
}

static inline ActionClip makeActionClip(juce::String id, ms_t start, ms_t length, juce::Colour col)
{
    ActionClip c;
    c.id = std::move(id);
    c.start = start;
    c.length = length;
    c.colour = col;
    return c;
}

static inline ActionLayer makeActionLayer(juce::String name, std::initializer_list<ActionClip> clips)
{
    ActionLayer L;
    L.name = std::move(name);
    for (const auto& c : clips)
        L.clips.add(std::move(c));
    return L;
}

void AmbisonicEncoderAudioProcessor::populateDefaultTimelineModels()
{
    timelines.clear(true);

    // Timeline 1: Choreography
    {
        auto* t = new TimelineModel();
        
        // Movement layer (always layer 0)
        t->movement.clips.add(makeMovementClip("Main Movement", 1000, 400, juce::Colours::orange));
        t->movement.clips.add(makeMovementClip("Secondary Move", 1600, 220, juce::Colours::orangered));
        t->movement.clips.add(makeMovementClip("Final Position", 1900, 800, juce::Colours::goldenrod));
        
        // Action layer
        t->actions.clips.add(makeActionClip("Rotation X", 1200, 2000, juce::Colours::slateblue));
        t->actions.clips.add(makeActionClip("Rotation Y", 800, 1500, juce::Colours::lightsteelblue));
        t->actions.clips.add(makeActionClip("Scale", 500, 1000, juce::Colours::mediumseagreen));
        t->actions.clips.add(makeActionClip("Stretch", 1800, 600, juce::Colours::seagreen));
        
        timelines.add(t);
    }

    // Timeline 2: Movement Patterns
    {
        auto* t = new TimelineModel();
        
        // Movement layer
        t->movement.clips.add(makeMovementClip("EaseIn Path", 0, 800, juce::Colours::cornflowerblue));
        t->movement.clips.add(makeMovementClip("Hold Position", 800, 400, juce::Colours::lightsteelblue));
        t->movement.clips.add(makeMovementClip("EaseOut Path", 1200, 700, juce::Colours::royalblue));
        
        // Action layer
        t->actions.clips.add(makeActionClip("X Offset", 300, 900, juce::Colours::mediumseagreen));
        t->actions.clips.add(makeActionClip("Y Offset", 900, 600, juce::Colours::seagreen));
        t->actions.clips.add(makeActionClip("Z Offset", 600, 800, juce::Colours::darkseagreen));
        
        timelines.add(t);
    }

    // Timeline 3: FX and Automation
    {
        auto* t = new TimelineModel();
        
        // Movement layer
        t->movement.clips.add(makeMovementClip("Base Movement", 500, 2500, juce::Colours::purple));
        
        // Action layer
        t->actions.clips.add(makeActionClip("Flash", 500, 120, juce::Colours::yellow));
        t->actions.clips.add(makeActionClip("Strobe", 1500, 300, juce::Colours::gold));
        t->actions.clips.add(makeActionClip("Dim", 2200, 700, juce::Colours::darkkhaki));
        
        t->actions.clips.add(makeActionClip("Vignette", 1000, 1200, juce::Colours::darkslategrey));
        t->actions.clips.add(makeActionClip("Bloom", 1800, 600, juce::Colours::plum));
        t->actions.clips.add(makeActionClip("Blur", 800, 1000, juce::Colours::lightblue));
        
        timelines.add(t);
    }

    // Timeline 4: Complex Example with Action Definitions
    {
        auto* t = new TimelineModel();
        
        // Movement layer
        t->movement.clips.add(makeMovementClip("Complex Movement", 0, 3000, juce::Colours::teal));
        
        ActionClip rotationClip = makeActionClip("3D Rotation", 500, 1000, juce::Colours::orange);
        rotationClip.actions.add(ActionDefinition{ActionType::RotationX, TimingType::AbsoluteTarget, 45.0});
        rotationClip.actions.add(ActionDefinition{ActionType::RotationY, TimingType::RelativeDuringClip, 90.0});
        t->actions.clips.add(rotationClip);
        
        ActionClip stretchClip = makeActionClip("Dynamic Stretch", 1600, 800, juce::Colours::red);
        stretchClip.actions.add(ActionDefinition{ActionType::Stretch, TimingType::AbsolutePerSecond, 2.0});
        t->actions.clips.add(stretchClip);
        
        timelines.add(t);
    }

    //listeners.call (&Listener::timelineListChanged);
}

ChannelLayout* AmbisonicEncoderAudioProcessor::getChannelLayout()
{
    return &channelLayout;
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

CustomOscRxPresetHelper* AmbisonicEncoderAudioProcessor::getCustomOscRxPresetHelper()
{
    return customOscRxPresetHelper.get();
}

CustomOscTxPresetHelper* AmbisonicEncoderAudioProcessor::getCustomOscTxPresetHelper()
{
    return customOscTxPresetHelper.get();
}

ScalingInfo* AmbisonicEncoderAudioProcessor::getScalingInfo()
{
    return &scalingInfo;
}

ZoomSettings* AmbisonicEncoderAudioProcessor::getZoomSettingsPointer()
{
    return zoomSettings.get();
}

OSCHandlerEncoder* AmbisonicEncoderAudioProcessor::getOscHandler()
{
    return pOscHandler;
}

AnimatorDataset* AmbisonicEncoderAudioProcessor::getAnimatorDataset()
{
    return &animatorDataset;
}

RadarOptions* AmbisonicEncoderAudioProcessor::getRadarOptions()
{
    return &radarOptions;
}

juce::OwnedArray<TimelineModel>* AmbisonicEncoderAudioProcessor::getTimelines() { return &timelines; }

void AmbisonicEncoderAudioProcessor::updateTrackProperties(const TrackProperties& properties)
{
	dawParameter.updateTrackPropertiesWorking = true;
	dawParameter.lastTrackProperties = properties;

#if (!MULTI_ENCODER_MODE)
	if (sources->size() > 0)
	{
		sources->get(0)->setName(properties.name.value_or("1"));
		sources->get(0)->setColor(properties.colour.value_or(TrackColors::getColor(0)));
	}
#endif
}

EncoderSettings* AmbisonicEncoderAudioProcessor::getEncoderSettings()
{
	return &encoderSettings;
}

void AmbisonicEncoderAudioProcessor::initializeOscSender()
{
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
    if(message.startsWith(presetHelper->UniqueActionMessageSelectPreset()))
    {
        File presetFile(message.substring(presetHelper->UniqueActionMessageSelectPreset().length()));
        presetHelper->loadFromXmlFile(presetFile, &audioParams, sources.get());
        presetHelper->notifyPresetChanged();
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmbisonicEncoderAudioProcessor();
}
