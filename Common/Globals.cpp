/*
  ==============================================================================

    Globals.cpp
    Created: 1 Apr 2020 1:53:59pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "Globals.h"
#define SQRT2 1.41421356237
#define INFINITE 0.0f

float Globals::Scaler = INFINITE;

float Globals::CartesianMin() { return Scaler == INFINITE ? -100000 : -Scaler; }
float Globals::CartesianMax() { return Scaler == INFINITE ? 100000 : Scaler; }
float Globals::DistanceMax() { return SQRT2 * CartesianMax(); }
bool Globals::IsInfinite() { return Scaler == INFINITE; }
float Globals::GetScaler() { return CartesianMax(); }
void Globals::SetScaler(float newScaler) { Scaler = newScaler; }
