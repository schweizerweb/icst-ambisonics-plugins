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



#include "GroupAnimator.h"

void GroupAnimator::initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet)
{
    for(int i = 0; i < ANIMATION_SET_COUNT; i++)
    {
        String indexString = "G" + String(i+1) + " ";
        
        animationSets[i].audioParameterAnimationToggle = new AudioParameterBool(ParameterID(indexString + "Animation Toggle", Constants::audioParamVersion), indexString + "Animation Toggle", false, AudioParameterBoolAttributes().withLabel("Turn On/Off Animation"));
    
        animationSets[i].audioParameterGroupRotationX = new AudioParameterFloat(ParameterID(indexString + "Rotation X", Constants::audioParamVersion), indexString + "Rotation X", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Group Rotation (X-Axis) in Degrees per Second"));
        
        animationSets[i].audioParameterGroupRotationY = new AudioParameterFloat(ParameterID(indexString + "Rotation Y", Constants::audioParamVersion), indexString + "Rotation Y", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Group Rotation (Y-Axis) in Degrees per Second"));
        
        animationSets[i].audioParameterGroupRotationZ = new AudioParameterFloat(ParameterID(indexString + "Rotation Z", Constants::audioParamVersion), indexString + "Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Group Rotation (Z-Axis) in Degrees per Second"));
    
        animationSets[i].audioParameterOriginRotationX = new AudioParameterFloat(ParameterID(indexString + "Origin Rotation X", Constants::audioParamVersion), indexString + "Origin Rotation X", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Rotation around Origin (X-Axis) in Degrees per Second"));
        
        animationSets[i].audioParameterOriginRotationY = new AudioParameterFloat(ParameterID(indexString + "Origin Rotation Y", Constants::audioParamVersion), indexString + "Origin Rotation Y", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Rotation around Origin (Y-Axis) in Degrees per Second"));
        
        animationSets[i].audioParameterOriginRotationZ = new AudioParameterFloat(ParameterID(indexString + "Origin Rotation Z", Constants::audioParamVersion), indexString + "Origin Rotation Z", NormalisableRange<float>(-360.0f, 360.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Rotation around Origin (Z-Axis) in Degrees per Second"));
    
        animationSets[i].audioParameterGroupStretch = new AudioParameterFloat(ParameterID(indexString + "Stretch", Constants::audioParamVersion), indexString + "Group Stretch", NormalisableRange<float>(-1.0f, 1.0f), 0.0f, AudioParameterFloatAttributes().withLabel("Group Stretch in Units per Second"));
    
    
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
        if(!exactlyEqual(rotationX, 0.0f) || !exactlyEqual(rotationY, 0.0f) || !exactlyEqual(rotationZ, 0.0f))
        {
            pSourceSet->rotateGroup(i, Constants::GradToRad(rotationX * seconds), Constants::GradToRad(rotationY * seconds), Constants::GradToRad(rotationZ * seconds));
        }
        
        float rotationOriginX = animationSets[i].audioParameterOriginRotationX->get();
        float rotationOriginY = animationSets[i].audioParameterOriginRotationY->get();
        float rotationOriginZ = animationSets[i].audioParameterOriginRotationZ->get();
        if(!exactlyEqual(rotationOriginX, 0.0f) || !exactlyEqual(rotationOriginY, 0.0f) || !exactlyEqual(rotationOriginZ, 0.0f))
        {
            pSourceSet->rotateGroupAroundOrigin(i, Constants::GradToRad(rotationOriginX * seconds), Constants::GradToRad(rotationOriginY * seconds), Constants::GradToRad(rotationOriginZ * seconds), true);
        }
        
        float stretch = animationSets[i].audioParameterGroupStretch->get();
        if(!exactlyEqual(stretch, 0.0f))
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
