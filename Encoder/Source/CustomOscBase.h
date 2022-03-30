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
    
    bool setOscPath(String path, String* pErrorMessage);
    String getOscPath();

protected:
    bool analyzeString(std::string parameterString, Array<UserDefinedParameter>* pArray);
    UserDefinedParameter* analyzeEscapedString(String fullPath, int* pIndex, String* pErrorMessage);
    float dualMap(double value, double maxValue, UserDefinedParameter* pParam);
    String oscPath;
    OwnedArray<UserDefinedParameter> parametersInPath;
    OwnedArray<UserDefinedParameter> realParameters;
    ScalingInfo* pScalingInfo;
};
