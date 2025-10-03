#include "TimelineWidgetMS.h"

TimelineWidgetMS::TimelineWidgetMS()
{
    mainView = std::make_unique<AnimatorMainView>();
    addAndMakeVisible(mainView.get());
    
    startTimerHz(30);
}

TimelineWidgetMS::~TimelineWidgetMS()
{
    stopTimer();
}

void TimelineWidgetMS::setModels(juce::OwnedArray<TimelineModel>* models)
{
    mainView->setTimelines(models);
}


void TimelineWidgetMS::setSelectionControl(PointSelection* pPointSelection)
{
    mainView->setSelectionControl(pPointSelection);
}

void TimelineWidgetMS::setSourceSet(AmbiSourceSet *pSources)
{
    mainView->setSourceSet(pSources);
}

void TimelineWidgetMS::setPlayheadProvider(std::function<PlayheadSnapshot()> provider)
{
    playheadProvider = provider;
    mainView->setPlayheadProvider(provider);
}

void TimelineWidgetMS::setAutoFollow(bool shouldAutoFollow)
{
    mainView->setAutoFollow(shouldAutoFollow);
}

void TimelineWidgetMS::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void TimelineWidgetMS::resized()
{
    mainView->setBounds(getLocalBounds());
}

void TimelineWidgetMS::timerCallback()
{
    if (playheadProvider)
    {
        auto snapshot = playheadProvider();
        if (snapshot.valid)
        {
            mainView->setPlayheadPosition(snapshot.timeMs);
        }
    }
}
