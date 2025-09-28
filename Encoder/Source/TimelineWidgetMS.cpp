#include "TimelineWidgetMS.h"

TimelineWidgetMS::TimelineWidgetMS()
{
    timeline = std::make_unique<TimelineComponent>();
    addAndMakeVisible(timeline.get());
    
    startTimerHz(30);
}

TimelineWidgetMS::~TimelineWidgetMS()
{
    stopTimer();
}

void TimelineWidgetMS::setModels(juce::OwnedArray<TimelineModel>* models)
{
    timeline->setTimelines(models);
}

void TimelineWidgetMS::setPlayheadProvider(std::function<PlayheadSnapshot()> provider)
{
    playheadProvider = provider;
}

void TimelineWidgetMS::setAutoFollow(bool shouldAutoFollow)
{
    timeline->setAutoFollow(shouldAutoFollow);
}

void TimelineWidgetMS::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void TimelineWidgetMS::resized()
{
    timeline->setBounds(getLocalBounds());
}

void TimelineWidgetMS::timerCallback()
{
    if (playheadProvider)
    {
        auto snapshot = playheadProvider();
        if (snapshot.valid)
        {
            timeline->setPlayheadPosition(snapshot.timeMs);
        }
    }
}
