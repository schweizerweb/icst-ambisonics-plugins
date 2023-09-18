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

class EncoderConstants
{
public:
    const static float UnitCircleRadiusMin;
    const static float UnitCircleRadiusMax;
    const static float UnitCircleRadiusResolution;
    
    const static float DbUnitMin;
    const static float DbUnitMax;
    const static float DbUnitResolution;
    
    const static float DistanceAttenuationMin;
    const static float DistanceAttenuationMax;
    const static float DistanceAttenuationResolution;
    
    const static float CenterCurveMin;
    const static float CenterCurveMax;
    const static float CenterCurveResolution;
    
    const static float AdvancedFactorMin;
    const static float AdvancedFactorMax;
    const static float AdvancedFactorResolution;
    
    const static float AdvancedExponentMin;
    const static float AdvancedExponentMax;
    const static float AdvancedExponentResolution;

    const static float MasterGainMin;
    const static float MasterGainMax;
    const static float MasterGainResolution;

    const static float AirAbsorbtionIntensityMin;
    const static float AirAbsorbtionIntensityMax;
    const static float AirAbsorbtionIntensityResolution;

    // make sure enum EncodingMode and StringArray encodingModeString are in sync!
    enum EncodingMode { None = 0, Standard = 1, Advanced = 2, Exponential = 3, InverseProportional = 4 };
    // make sure enum AirAbsorbtionMode and StringArray airAbsorbtionModeString are in sync!
    enum AirAbsorbtionMode { Off = 0, LowPass = 1};

    const static StringArray encodingModeStrings;
    const static StringArray airAbsorbtionModeStrings;
    
};
