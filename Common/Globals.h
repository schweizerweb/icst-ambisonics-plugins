/*
  ==============================================================================

    Globals.h
    Created: 1 Apr 2020 1:53:08pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
namespace Globals
{
    float CartesianMin();
    float CartesianMax();
    float DistanceMax();
    bool IsInfinite();
    void SetScaler(double newScaler);
    double GetScaler();

    constexpr double Infinite = 0.0;
    inline double _global_scaler;
};
