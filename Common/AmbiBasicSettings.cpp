/*
  ==============================================================================

    AmbiBasicSettings.cpp
    Created: 18 Nov 2019 9:35:45am
    Author:  chris

  ==============================================================================
*/

#include "AmbiBasicSettings.h"

AmbiBasicSettings::AmbiBasicSettings(int ambisonicsOrder): ambisonicsOrder(ambisonicsOrder)
{
}

int AmbiBasicSettings::getAmbisonicsOrder() const
{
    return ambisonicsOrder;
}

int AmbiBasicSettings::getAmbisonicsChannelCount() const
{
    return (ambisonicsOrder + 1) * (ambisonicsOrder + 1);
}

void AmbiBasicSettings::setAmbisonicsOrder(int order)
{
    ambisonicsOrder = order;
}
