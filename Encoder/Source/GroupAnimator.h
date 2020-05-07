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

class GroupAnimator : Timer, AudioProcessorParameter::Listener
{
public:
    void initialize(AudioProcessor* pAudioProcessor, AmbiDataSet* pDataSet);
    
private:
    void timerCallback() override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    void toggleOnOff();
    
private:
    
    AudioParameterBool* audioParameterAnimationToggle;
    AudioParameterFloat* audioParameterGroupRotation;
    AudioParameterFloat* audioParameterOriginRotation;
    AudioParameterFloat* audioParameterGroupStretch;

    AmbiDataSet* pSourceSet;


};

