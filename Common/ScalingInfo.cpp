/*
  ==============================================================================

    ScalingInfo.cpp
    Created: 7 Apr 2021 6:03:37pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScalingInfo.h"

#define SQRT2 1.41421356237f

double ScalingInfo::Infinite = 0.0;

ScalingInfo::ScalingInfo()
{
    Scaler = Infinite;
}

double ScalingInfo::compress(double value) const
{
    if(IsInfinite())
        return double(atan(value) / MathConstants<double>::halfPi);
        
    return double(value / Scaler);
}
    
double ScalingInfo::decompress(double value) const
{
    if(IsInfinite())
        return double(tan(value * MathConstants<double>::halfPi));
        
    return double(value * Scaler);
}

float ScalingInfo::CartesianMin()
{
    return Scaler == Infinite ? -100000 : -(float)Scaler;
}

float ScalingInfo::CartesianMax()
{
    return Scaler == Infinite ? 100000 : (float)Scaler;
}

float ScalingInfo::DistanceMax()
{
    return SQRT2 * CartesianMax();
}

bool ScalingInfo::IsInfinite() const
{
    return Scaler == Infinite;
}

double ScalingInfo::GetScaler()
{
    return CartesianMax();
}

void ScalingInfo::SetScaler(double newScaler)
{
    Scaler = newScaler;
}
