#pragma once
#include <JuceHeader.h>

class UTF8Helpers
{
public:
    static juce::String utf8(const char* text)
    {
        return juce::String(juce::CharPointer_UTF8(text));
    }
    
    // Common symbols
    static juce::String checkMark() { return utf8(u8"✓"); }
    static juce::String xMark() { return utf8(u8"✗"); }
    static juce::String warning() { return utf8(u8"⚠"); }
    static juce::String dot() { return utf8(u8"●"); }
    static juce::String play() { return utf8(u8"▶"); }
    static juce::String pause() { return utf8(u8"⏸"); }
    static juce::String folder() { return utf8(u8"📁"); }
    static juce::String zoom() { return utf8(u8"🔍"); }
    static juce::String clock() { return utf8(u8"⏱"); }
    static juce::String clapperboard() { return utf8(u8"🎬"); }
    static juce::String importIcon() { return utf8(u8"📥"); }
    static juce::String exportIcon() { return utf8(u8"📤"); }
    static juce::String errorIcon() { return utf8(u8"❌"); }
    static juce::String successIcon() { return utf8(u8"✅"); }
    static juce::String scissors() { return utf8(u8"✂"); }
    static juce::String clipboard() { return utf8(u8"📋"); }
};