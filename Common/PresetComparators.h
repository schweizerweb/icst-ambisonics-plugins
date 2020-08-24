/*
  ==============================================================================

    PresetComparators.h
    Created: 26 Jan 2020 9:22:16pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class PresetNameComparatorAscending
{
public:
    int compareElements (File first, File second)
    {
        return (first < second) ? -1 : ((first == second) ? 0 : 1);
    }
};

class PresetNameComparatorDescending
{
public:
    int compareElements (File first, File second)
    {
        return (first > second) ? -1 : ((first == second) ? 0 : 1);
    }
};
class PresetDateComparatorAscending
{
public:
    int compareElements (File first, File second)
    {
        return (first.getCreationTime() < second.getCreationTime()) ? -1 : ((first.getCreationTime() == second.getCreationTime()) ? 0 : 1);
    }
};

class PresetDateComparatorDescending
{
public:
    int compareElements (File first, File second)
    {
        return (first.getCreationTime() > second.getCreationTime()) ? -1 : ((first.getCreationTime() == second.getCreationTime()) ? 0 : 1);
    }
};

