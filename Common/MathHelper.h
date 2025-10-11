#pragma once
#include "JuceHeader.h"

class MathHelper
{
public:
    static juce::Quaternion<double> EulerToQuaternion(double roll, double pitch, double yaw);
    static juce::Vector3D<double> QuaternionToEuler(const juce::Quaternion<double>& q);
};
