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
    return approximatelyEqual(Scaler, Infinite) ? -100000.0f : -(float)Scaler;
}

float ScalingInfo::CartesianMax()
{
    return approximatelyEqual(Scaler, Infinite) ? 100000.0f : (float)Scaler;
}

float ScalingInfo::DistanceMax()
{
    return SQRT2 * CartesianMax();
}

bool ScalingInfo::IsInfinite() const
{
    return exactlyEqual(Scaler, Infinite);
}

double ScalingInfo::GetScaler()
{
    return CartesianMax();
}

void ScalingInfo::SetScaler(double newScaler)
{
    Scaler = newScaler;
}
