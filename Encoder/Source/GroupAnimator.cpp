/*
  ==============================================================================

    GroupAnimator.cpp
    Created: 7 May 2020 10:25:14pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "GroupAnimator.h"

void GroupAnimator::initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet)
{
    for(int i = 0; i < ANIMATION_SET_COUNT; i++)
    {
        String indexString = "G" + String(i+1) + " ";
        
        animationSets[i].audioParameterAnimationToggle = new AudioParameterBool(indexString + "Animation Toggle", indexString + "Animation Toggle", false, "Turn On/Off Animation");
    
        animationSets[i].audioParameterGroupRotationX = new AudioParameterFloat(indexString + "Rotation X", indexString + "Rotation X", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Group Rotation (X-Axis) in Degrees per Second");
        
        animationSets[i].audioParameterGroupRotationY = new AudioParameterFloat(indexString + "Rotation Y", indexString + "Rotation Y", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Group Rotation (Y-Axis) in Degrees per Second");
        
        animationSets[i].audioParameterGroupRotationZ = new AudioParameterFloat(indexString + "Rotation Z", indexString + "Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Group Rotation (Z-Axis) in Degrees per Second");
    
        animationSets[i].audioParameterOriginRotationX = new AudioParameterFloat(indexString + "Origin Rotation X", indexString + "Origin Rotation X", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Rotation around Origin (X-Axis) in Degrees per Second");
        
        animationSets[i].audioParameterOriginRotationY = new AudioParameterFloat(indexString + "Origin Rotation Y", indexString + "Origin Rotation Y", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Rotation around Origin (Y-Axis) in Degrees per Second");
        
        animationSets[i].audioParameterOriginRotationZ = new AudioParameterFloat(indexString + "Origin Rotation Z", indexString + "Origin Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, "Rotation around Origin (Z-Axis) in Degrees per Second");
    
        animationSets[i].audioParameterGroupStretch = new AudioParameterFloat(indexString + "Stretch", indexString + "Group Stretch", NormalisableRange<float>(-1.0f, 1.0f), 0.0f, "Group Stretch in Units per Second");
    
    
        pAudioProcessor->addParameter(animationSets[i].audioParameterAnimationToggle);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupRotationX);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupRotationY);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupRotationZ);
        pAudioProcessor->addParameter(animationSets[i].audioParameterOriginRotationX);
        pAudioProcessor->addParameter(animationSets[i].audioParameterOriginRotationY);
        pAudioProcessor->addParameter(animationSets[i].audioParameterOriginRotationZ);
        pAudioProcessor->addParameter(animationSets[i].audioParameterGroupStretch);
        
        animationSets[i].toggleIndex = animationSets[i].audioParameterAnimationToggle->getParameterIndex();
        animationSets[i].audioParameterAnimationToggle->addListener(this);
    }
    
    pSourceSet = pDataSet;
}

void GroupAnimator::toggleOnOff(int parameterIndex)
{
    for(int i = 0; i < ANIMATION_SET_COUNT && i < pSourceSet->groupCount(); i++)
    {
        if(animationSets[i].toggleIndex == parameterIndex)
        {
            // TODO: to be updated or deleted:
            /*
            bool enablePoints = !animationSets[i].audioParameterAnimationToggle->get();
            
            
            for(AmbiPoint* p: pSourceSet->get(i)->groupPoints)
            {
                p->getPoint()->getAudioParameterSet().setEnabled(enablePoints);
            }
            */
            break;
        }
    }
}

void GroupAnimator::doStep(float seconds)
{
    for(int i = 0; i < ANIMATION_SET_COUNT; i++)
    {
        if(!animationSets[i].audioParameterAnimationToggle->get() || i >= pSourceSet->groupCount())
            continue;
        
        float rotationX = animationSets[i].audioParameterGroupRotationX->get();
        float rotationY = animationSets[i].audioParameterGroupRotationY->get();
        float rotationZ = animationSets[i].audioParameterGroupRotationZ->get();
        if(rotationX != 0.0f || rotationY != 0.0f || rotationZ != 0.0f)
        {
            pSourceSet->rotateGroup(i, Constants::GradToRad(rotationX * seconds), Constants::GradToRad(rotationY * seconds), Constants::GradToRad(rotationZ * seconds));
        }
        
        float rotationOriginX = animationSets[i].audioParameterOriginRotationX->get();
        float rotationOriginY = animationSets[i].audioParameterOriginRotationY->get();
        float rotationOriginZ = animationSets[i].audioParameterOriginRotationZ->get();
        if(rotationOriginX != 0.0f || rotationOriginY != 0.0f || rotationOriginZ != 0.0f)
        {
            pSourceSet->rotateGroupAroundOrigin(i, Constants::GradToRad(rotationOriginX * seconds), Constants::GradToRad(rotationOriginY * seconds), Constants::GradToRad(rotationOriginZ * seconds), true);
        }
        
        float stretch = animationSets[i].audioParameterGroupStretch->get();
        if(stretch != 0.0f)
        {
            pSourceSet->stretchGroup(i, stretch * seconds);
        }
    }
}

void GroupAnimator::parameterValueChanged(int parameterIndex, float /*newValue*/)
{
    toggleOnOff(parameterIndex);
}

void GroupAnimator::parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/)
{
    
}
