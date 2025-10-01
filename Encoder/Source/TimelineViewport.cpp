#include "TimelineViewport.h"

TimelineViewport::TimelineViewport()
{
    timelineComponent = std::make_unique<TimelineComponent>();
    setViewedComponent(timelineComponent.get(), false);
    
    // Show only vertical scrollbar
    setScrollBarsShown(true, false, true, false);
    setScrollBarThickness(16);
}

void TimelineViewport::resized()
{
    Viewport::resized();
    updateContentSize();
}

void TimelineViewport::setTimelines(juce::OwnedArray<TimelineModel>* newTimelines)
{
    timelineComponent->setTimelines(newTimelines);
    updateContentSize();
}

void TimelineViewport::updateContentSize()
{
    if (timelineComponent)
    {
        // Calculate required content size
        const int contentWidth = getMaximumVisibleWidth();
        const int contentHeight = timelineComponent->calculateTotalContentHeight();
        
        timelineComponent->setSize(contentWidth, contentHeight);
    }
}
