/*
  ==============================================================================

    GroupAnimator.cpp
    Created: 7 May 2020 10:25:14pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "GroupAnimator.h"
#define TIMER_FREQUENCY 20

void GroupAnimator::initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet)
{
    audioParameterAnimationToggle = new AudioParameterBool("Animation Toggle", "Animation Toggle", false, "Turn On/Off Animation");
    
    audioParameterGroupRotation = new AudioParameterFloat("Group Rotation Z", "Group Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Group Rotation (Z-Axis) in Degrees per Second");
    
    audioParameterOriginRotation = new AudioParameterFloat("Origin Rotation Z", "Origin Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Rotation around Origin (Z-Axis) in Degrees per Second");
    
    audioParameterGroupStretch = new AudioParameterFloat("Group Stretch", "Group Stretch", NormalisableRange<float>(-1.0f, 1.0f), 0.0f, "Group Stretch in Units per Second");
    
    audioParameterAnimationToggle->addListener(this);
    
    pAudioProcessor->addParameter(audioParameterAnimationToggle);
    pAudioProcessor->addParameter(audioParameterGroupRotation);
    pAudioProcessor->addParameter(audioParameterOriginRotation);
    pAudioProcessor->addParameter(audioParameterGroupStretch);
    
    pSourceSet = pDataSet;
    
    toggleOnOff();
}

void GroupAnimator::toggleOnOff()
{
    if(audioParameterAnimationToggle->get())
    {
        startTimerHz(TIMER_FREQUENCY);
    }
    else
    {
        stopTimer();
    }
    
    if(pSourceSet->groupCount() > 0)
    {
        for(AmbiPoint* p: pSourceSet->getGroup(0)->groupPoints)
        {
            p->getPoint()->getAudioParameterSet().setEnabled(!audioParameterAnimationToggle->get());
        }
    }
}

void GroupAnimator::timerCallback()
{
    if(pSourceSet->groupCount() > 0)
    {
        float rotationZ = audioParameterGroupRotation->get();
        if(rotationZ != 0.0f)
        {
            pSourceSet->rotateGroup(0, 0.0, 0.0, Constants::GradToRad(rotationZ / TIMER_FREQUENCY));
        }
        
        float rotationOriginZ = audioParameterOriginRotation->get();
        if(rotationOriginZ != 0.0f)
        {
            pSourceSet->rotateGroupAroundOrigin(0, 0.0, 0.0, Constants::GradToRad(rotationOriginZ / TIMER_FREQUENCY), true);
        }
        
        float stretch = audioParameterGroupStretch->get();
        if(stretch != 0.0f)
        {
            pSourceSet->stretchGroup(0, stretch / TIMER_FREQUENCY);
        }
    }
}

void GroupAnimator::parameterValueChanged(int parameterIndex, float newValue)
{
    toggleOnOff();
}

void GroupAnimator::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    
}
