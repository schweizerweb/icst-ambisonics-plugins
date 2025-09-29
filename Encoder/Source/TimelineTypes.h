#pragma once

#include "JuceHeader.h"

using ms_t = int64_t;

struct PlayheadSnapshot
{
    bool valid = false;
    ms_t timeMs = 0;
    bool playing = false;
    double bpm = 120.0;
    bool looping = false;
    ms_t loopStartMs = 0;
    ms_t loopEndMs = 0;
};