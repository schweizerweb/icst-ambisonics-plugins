/*
  ==============================================================================

    AudioParams.h
    Created: 2 May 2020 2:01:33pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "AudioParameterSet.h"
#include "AudioParameterFloatAmbi.h"
#include "AudioParameterBoolAmbi.h"
//#include "AmbiSourceSet.h"

class AmbiSourceSet;

class AudioParams : AudioParameterFloat::Listener
{
public:
    void initialize(AudioProcessor* pProcessor, ScalingInfo* pScalingInfo, AmbiSourceSet* pSources, int numberOfGroups) {
        
        this->pSourceSet = pSources;
        
        // points (X, Y, Z, Gain)
         for (int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
         {
            String indexStr = String(i + 1);
             
            AudioParameterSet set;
            set.pScaling = pScalingInfo;
            set.pX = new AudioParameterFloatAmbi("X" + indexStr, 1, "X " + indexStr, "Point " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::X);
            set.pY = new AudioParameterFloatAmbi("Y" + indexStr, 1, "Y " + indexStr, "Point " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::Y);
            set.pZ = new AudioParameterFloatAmbi("Z" + indexStr, 1, "Z " + indexStr, "Point " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::Z);
            set.pGain = new AudioParameterFloatAmbi("Gain" + indexStr, 1, "Gain" + indexStr, "Point " + indexStr + ": Gain", AudioProcessorParameter::genericParameter, NormalisableRange<float>((float)Constants::GainDbMin, (float)Constants::GainDbMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::Gain);
            set.pMute = new AudioParameterBoolAmbi("Mute" + indexStr, 1, "Mute" + indexStr, "Point " + indexStr + ": Mute", false, (AmbiDataSet*)pSourceSet, i, AudioParameterBoolAmbi::Mute);
             
            sourceParams.add(set);
            pProcessor->addParameter(set.pX);
            pProcessor->addParameter(set.pY);
            pProcessor->addParameter(set.pZ);
            pProcessor->addParameter(set.pGain);
            pProcessor->addParameter(set.pMute);
        }
        
        // groups
        for (int i = 0; i < numberOfGroups; i++)
        {
            String indexStr = String(i + 1);
            
            AudioParameterSet set;
            set.pScaling = pScalingInfo;
            set.pX = new AudioParameterFloatAmbi("GX" + indexStr, 1, "GX " + indexStr, "Group " + indexStr + ": X", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::GX);
            set.pY = new AudioParameterFloatAmbi("GY" + indexStr, 1, "GY " + indexStr, "Group " + indexStr + ": Y", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::GY);
            set.pZ = new AudioParameterFloatAmbi("GZ" + indexStr, 1, "GZ " + indexStr, "Group " + indexStr + ": Z", AudioProcessorParameter::genericParameter, NormalisableRange<float>(Constants::CompressedMin, Constants::CompressedMax), 0.0f, (AmbiDataSet*)pSourceSet, i, AudioParameterFloatAmbi::GZ);
            set.pQ0 = new AudioParameterFloat("G_Q0_" + indexStr, "G_Q0 " + indexStr, NormalisableRange<float>(-1, 1), 0.0f, "Group " + indexStr + ": Rotation Quaternion 0", AudioProcessorParameter::genericParameter);
            set.pQ1 = new AudioParameterFloat("G_Q1_" + indexStr, "G_Q1 " + indexStr, NormalisableRange<float>(-1, 1), 0.0f, "Group " + indexStr + ": Rotation Quaternion 1", AudioProcessorParameter::genericParameter);
            set.pQ2 = new AudioParameterFloat("G_Q2_" + indexStr, "G_Q2 " + indexStr, NormalisableRange<float>(-1, 1), 0.0f, "Group " + indexStr + ": Rotation Quaternion 2", AudioProcessorParameter::genericParameter);
            set.pQ3 = new AudioParameterFloat("G_Q3_" + indexStr, "G_Q3 " + indexStr, NormalisableRange<float>(-1, 1), 1.0f, "Group " + indexStr + ": Rotation Quaternion 3", AudioProcessorParameter::genericParameter);
            
            NormalisableRange<float> stretchRange(Constants::StretchMin, Constants::StretchMax);
            stretchRange.setSkewForCentre(1.0);
            set.pStretch = new AudioParameterFloat("G_Stretch_" + indexStr, "G_Stretch " + indexStr, stretchRange, 1.0, "Group " + indexStr + ": Stretch Factor", AudioProcessorParameter::genericParameter);
            
            groupParams.add(set);
            pProcessor->addParameter(set.pX);
            pProcessor->addParameter(set.pY);
            pProcessor->addParameter(set.pZ);
            pProcessor->addParameter(set.pQ0);
            pProcessor->addParameter(set.pQ1);
            pProcessor->addParameter(set.pQ2);
            pProcessor->addParameter(set.pQ3);
            pProcessor->addParameter(set.pStretch);
            
            paramIndexGroupIndexMap[set.pX->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pY->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pZ->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pQ0->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pQ1->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pQ2->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pQ3->getParameterIndex()] = i;
            paramIndexGroupIndexMap[set.pStretch->getParameterIndex()] = i;
            
            set.pQ0->addListener(this);
            set.pQ1->addListener(this);
            set.pQ2->addListener(this);
            set.pQ3->addListener(this);
            set.pStretch->addListener(this);
        }
    }
    
    void parameterValueChanged(int parameterIndex, float /*newValue*/) override {
        auto i = paramIndexGroupIndexMap.find(parameterIndex);
        if(i != paramIndexGroupIndexMap.end())
        {
            int index = i->second;
            if(parameterIndex == groupParams[index].pStretch->getParameterIndex())
            {
                ((AmbiDataSet*)pSourceSet)->setGroupStretch(index, groupParams[index].pStretch->get(), false);
            }
            else
            {
                // rotation
                ((AmbiDataSet*)pSourceSet)->setGroupRotation(index,
                    Quaternion<double>(
                        groupParams[index].pQ0->get(),
                        groupParams[index].pQ1->get(),
                        groupParams[index].pQ2->get(),
                        groupParams[index].pQ3->get()).normalised(),
                    false);
            }
        }
    }
    
    void parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/) override {
        // currently nothing to do
    }
    
    
    Array<AudioParameterSet> sourceParams;
    Array<AudioParameterSet> groupParams;
    std::map<int, int> paramIndexGroupIndexMap;
    AmbiSourceSet* pSourceSet;
};
