/*
  ==============================================================================

    ScalingInfo.h
    Created: 7 Apr 2021 6:03:37pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
class ScalingInfo
{
public:
    ScalingInfo();
    float CartesianMin();
    float CartesianMax();
    float DistanceMax();
    bool IsInfinite() const;
    void SetScaler(double newScaler);
    double GetScaler();
    static double Infinite;
    
    double compress(double d) const;
    double decompress(double d) const;
private:
    double Scaler;
};

