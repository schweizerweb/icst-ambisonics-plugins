#pragma once

#include "JuceHeader.h"
#include "AnimatorMainView.h"  // Include the new main view
#include "TimelineModel.h"
#include "TimelineTypes.h"

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
    std::unique_ptr<AnimatorMainView> mainView;  // Use main view instead of timeline component
    std::function<PlayheadSnapshot()> playheadProvider;
    
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineWidgetMS)
};
