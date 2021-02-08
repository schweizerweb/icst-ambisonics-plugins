/*
  ==============================================================================

    GroupAnimator.h
    Created: 7 May 2020 10:25:14pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiDataSet.h"
#define ANIMATION_SET_COUNT 4

class GroupAnimator : AudioProcessorParameter::Listener
{
public:
    void initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet);
    void doStep(float seconds);

private:
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    void toggleOnOff(int parameterIndex);
    
private:
    struct GroupAnimationSet
    {
        AudioParameterBool* audioParameterAnimationToggle;
        AudioParameterFloat* audioParameterGroupRotationX;
        AudioParameterFloat* audioParameterGroupRotationY;
        AudioParameterFloat* audioParameterGroupRotationZ;
        AudioParameterFloat* audioParameterOriginRotationX;
        AudioParameterFloat* audioParameterOriginRotationY;
        AudioParameterFloat* audioParameterOriginRotationZ;
        AudioParameterFloat* audioParameterGroupStretch;
        int toggleIndex;
    };
    
    GroupAnimationSet animationSets[ANIMATION_SET_COUNT];
    AmbiDataSet* pSourceSet;
};

