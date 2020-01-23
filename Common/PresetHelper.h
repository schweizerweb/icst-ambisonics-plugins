/*
  ==============================================================================

    PresetBase.h
    Created: 21 Jan 2020 4:35:32pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#define ACTION_MESSAGE_PRESETS_CHANGED "PresetsChanged"

class PresetHelper
{
public:
    virtual ~PresetHelper() {};
    virtual bool checkValid(File presetFile) = 0;
};
