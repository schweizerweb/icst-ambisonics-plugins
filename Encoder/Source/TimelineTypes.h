#pragma once

#include "JuceHeader.h"
#include <cmath>

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

// Shared time-unit display helpers for the animator UI. These only affect how time values are
// shown/entered - stored clip/timeline data (ms_t) is never converted or rounded in place.

// Formats a millisecond value for display in the given unit, without a unit suffix (e.g. "1250"
// in milliseconds mode, "1.25" in seconds mode) - for editable fields whose label already states
// the unit.
inline juce::String formatTimeValue(ms_t timeMs, bool displayInSeconds)
{
    if (!displayInSeconds)
        return juce::String(static_cast<juce::int64>(timeMs));

    juce::String text(timeMs / 1000.0, 3);

    // Trim trailing zeros for a clean display (1.250 -> 1.25, 1.000 -> 1), while still round-tripping
    // exactly back to the original millisecond value via parseTimeValue().
    while (text.endsWithChar('0'))
        text = text.dropLastCharacters(1);
    if (text.endsWithChar('.'))
        text = text.dropLastCharacters(1);

    return text;
}

// Same as formatTimeValue(), with the unit suffix appended - for read-only displays (tooltips,
// status messages, timeline ruler) where there's no separate label to state the unit.
inline juce::String formatTimeValueWithUnit(ms_t timeMs, bool displayInSeconds)
{
    return formatTimeValue(timeMs, displayInSeconds) + (displayInSeconds ? " s" : " ms");
}

// Parses text the user entered in the given display unit back to milliseconds.
inline ms_t parseTimeValue(const juce::String& text, bool displayInSeconds)
{
    if (!displayInSeconds)
        return static_cast<ms_t>(text.getLargeIntValue());

    return static_cast<ms_t>(std::llround(text.getDoubleValue() * 1000.0));
}