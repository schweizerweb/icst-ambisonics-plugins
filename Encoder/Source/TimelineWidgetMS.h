#pragma once

#include "JuceHeader.h"
#include "TimelineComponent.h"

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

class TimelineWidgetMS : public juce::Component,
                         private juce::Timer
{
public:
    TimelineWidgetMS();
    ~TimelineWidgetMS() override;
    
    void setModels(juce::OwnedArray<TimelineModel>* models);
    void setPlayheadProvider(std::function<PlayheadSnapshot()> provider);
    void setAutoFollow(bool shouldAutoFollow);
    
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<TimelineComponent> timeline;
    std::function<PlayheadSnapshot()> playheadProvider;
    
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineWidgetMS)
};
