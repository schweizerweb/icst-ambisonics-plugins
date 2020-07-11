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

class GroupAnimator : Timer, AudioProcessorParameter::Listener
{
public:
    void initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet);
    
private:
    void timerCallback() override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    void toggleOnOff(int parameterIndex);
    
private:
    struct GroupAnimationSet
    {
        AudioParameterBool* audioParameterAnimationToggle;
        AudioParameterFloat* audioParameterGroupRotation;
        AudioParameterFloat* audioParameterOriginRotation;
        AudioParameterFloat* audioParameterGroupStretch;
        int toggleIndex;
    };
    
    GroupAnimationSet animationSets[ANIMATION_SET_COUNT];
    AmbiDataSet* pSourceSet;
};

