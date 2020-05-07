/*
  ==============================================================================

    Globals.cpp
    Created: 1 Apr 2020 1:53:59pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "Globals.h"
#define SQRT2 1.41421356237f
#define INFINITE 0.0

double Globals::Scaler = INFINITE;

float Globals::CartesianMin() { return Scaler == INFINITE ? -100000 : -(float)Scaler; }
float Globals::CartesianMax() { return Scaler == INFINITE ? 100000 : (float)Scaler; }
float Globals::DistanceMax() { return SQRT2 * CartesianMax(); }
bool Globals::IsInfinite() { return Scaler == INFINITE; }
double Globals::GetScaler() { return CartesianMax(); }
void Globals::SetScaler(double newScaler) { Scaler = newScaler; }
