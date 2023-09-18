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



#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiDataSet.h"

#define ANIMATION_SET_COUNT 4

// First version of group animation using automation parameters
// Not used anymore!

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

