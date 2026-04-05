#pragma once

#include <JuceHeader.h>
#include "TimelineComponent.h"

class TimelineViewport : public juce::Viewport
{
public:
    TimelineViewport();
    
    void setTimelines(juce::OwnedArray<TimelineModel>* newTimelines);
    
    TimelineComponent* getTimelineComponent() { return timelineComponent.get(); }
    
    void resized() override;

private:
    std::unique_ptr<TimelineComponent> timelineComponent;
    void updateContentSize();
};
