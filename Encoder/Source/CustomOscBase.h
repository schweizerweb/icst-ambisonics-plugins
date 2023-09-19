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
    bool checkChannelSelection(UserDefinedParameter* pParam);
    
    String oscPath;
    OwnedArray<UserDefinedParameter> parametersInPath;
    OwnedArray<UserDefinedParameter> realParameters;
    
    enum ChannelSelectionMode { Undefined, ConstantIndex, ConstantName, Parameter } channelSelectionMode;
    int constantChannelIndex;
    String constantChannelName;
    
    ScalingInfo* pScalingInfo;
};
