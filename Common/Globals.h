/*
  ==============================================================================

    Globals.h
    Created: 1 Apr 2020 1:53:08pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
class Globals
{
public:
    static float CartesianMin();
    static float CartesianMax();
    static float DistanceMax();
    static bool IsInfinite();
    static void SetScaler(double newScaler);
    static double GetScaler();
    
private:
    static double Scaler;
};
