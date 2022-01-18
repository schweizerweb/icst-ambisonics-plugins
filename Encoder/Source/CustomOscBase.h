/*
  ==============================================================================

    CustomOscBase.h
    Created: 18 Jan 2022 9:55:33am
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "UserDefinedParameter.h"
#include "../../Common/ScalingInfo.h"

class CustomOscBase
{
public:
    CustomOscBase(ScalingInfo* pScaling);
    
    bool setOscPath(String path);
    String getOscPath();

protected:
    bool analyzeString(std::string parameterString, Array<UserDefinedParameter>* pArray);
    float dualMap(double value, double maxValue, UserDefinedParameter* pParam);
    String oscPath;
    Array<UserDefinedParameter> parametersInPath;
    Array<UserDefinedParameter> realParameters;
    ScalingInfo* pScalingInfo;
};
