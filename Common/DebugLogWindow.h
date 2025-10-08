#pragma once

#include <JuceHeader.h>
#include "DebugLogHandler.h"

class DebugLogWindow : public juce::DocumentWindow
{
public:
    DebugLogWindow(DebugLogHandler& handler);
    ~DebugLogWindow() override;

    void closeButtonPressed() override;
    void addMessages(const juce::StringArray& messages, const std::vector<juce::Colour>& colours);

private:
    DebugLogHandler& logHandler;
    juce::TextEditor textEditor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugLogWindow)
};
