#pragma once
#include "JuceHeader.h"

class MathHelper
{
public:
    static juce::Quaternion<double> EulerToQuaternion(double roll, double pitch, double yaw);
};