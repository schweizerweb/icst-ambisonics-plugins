/*
  ==============================================================================

    Globals.cpp
    Created: 1 Apr 2020 1:53:59pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "Globals.h"
#define SQRT2 1.41421356237f


float Globals::CartesianMin() { return _global_scaler == Infinite ? -100000 : -(float)_global_scaler; }
float Globals::CartesianMax() { return _global_scaler == Infinite ? 100000 : (float)_global_scaler; }
float Globals::DistanceMax() { return SQRT2 * CartesianMax(); }
bool Globals::IsInfinite() { return _global_scaler == Infinite; }
double Globals::GetScaler() { return CartesianMax(); }
void Globals::SetScaler(double newScaler) { _global_scaler = newScaler; }
