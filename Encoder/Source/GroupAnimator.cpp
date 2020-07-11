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
    for(int i = 0; i < ANIMATION_SET_COUNT; i++)
    {
        String indexString = "G" + String(i+1) + " ";
        
        animationSets[i].audioParameterAnimationToggle = new AudioParameterBool(indexString + "Animation Toggle", indexString + "Animation Toggle", false, "Turn On/Off Animation");
    
        animationSets[i].audioParameterGroupRotation = new AudioParameterFloat(indexString + "Rotation Z", indexString + "Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Group Rotation (Z-Axis) in Degrees per Second");
    
        animationSets[i].audioParameterOriginRotation = new AudioParameterFloat(indexString + "Origin Rotation Z", indexString + "Origin Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Rotation around Origin (Z-Axis) in Degrees per Second");
    
        animationSets[i].audioParameterGroupStretch = new AudioParameterFloat(indexString + "Stretch", indexString + "Group Stretch", NormalisableRange<float>(-1.0f, 1.0f), 0.0f, "Group Stretch in Units per Second");
    
    
        pAudioProcessor->addParameter(animationSets[i].audioParameterAnimationToggle);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupRotation);
        pAudioProcessor->addParameter(animationSets[i].audioParameterOriginRotation);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupStretch);
        
        animationSets[i].toggleIndex = animationSets[i].audioParameterAnimationToggle->getParameterIndex();
        animationSets[i].audioParameterAnimationToggle->addListener(this);
    }
    
    pSourceSet = pDataSet;
    startTimerHz(TIMER_FREQUENCY);
}

void GroupAnimator::toggleOnOff(int parameterIndex)
{
    for(int i = 0; i < ANIMATION_SET_COUNT && i < pSourceSet->groupCount(); i++)
    {
        if(animationSets[i].toggleIndex == parameterIndex)
        {
            bool enablePoints = !animationSets[i].audioParameterAnimationToggle->get();
            
            for(AmbiPoint* p: pSourceSet->getGroup(i)->groupPoints)
            {
                p->getPoint()->getAudioParameterSet().setEnabled(enablePoints);
            }
            
            break;
        }
    }
}

void GroupAnimator::timerCallback()
{
    for(int i = 0; i < ANIMATION_SET_COUNT; i++)
    {
        if(!animationSets[i].audioParameterAnimationToggle->get() || i >= pSourceSet->groupCount())
            continue;
        
        float rotationZ = animationSets[i].audioParameterGroupRotation->get();
        if(rotationZ != 0.0f)
        {
            pSourceSet->rotateGroup(i, 0.0, 0.0, Constants::GradToRad(rotationZ / TIMER_FREQUENCY));
        }
        
        float rotationOriginZ = animationSets[i].audioParameterOriginRotation->get();
        if(rotationOriginZ != 0.0f)
        {
            pSourceSet->rotateGroupAroundOrigin(i, 0.0, 0.0, Constants::GradToRad(rotationOriginZ / TIMER_FREQUENCY), true);
        }
        
        float stretch = animationSets[i].audioParameterGroupStretch->get();
        if(stretch != 0.0f)
        {
            pSourceSet->stretchGroup(i, stretch / TIMER_FREQUENCY);
        }
    }
}

void GroupAnimator::parameterValueChanged(int parameterIndex, float newValue)
{
    toggleOnOff(parameterIndex);
}

void GroupAnimator::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
    
}
