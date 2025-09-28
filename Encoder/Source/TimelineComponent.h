#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"

class TimelineComponent : public juce::Component,
                          public juce::ScrollBar::Listener,
                          public juce::ComboBox::Listener,  // Added for timeline selector
                          public juce::MouseListener
{
public:
    TimelineComponent();
    ~TimelineComponent() override;

    void setTimelines(juce::OwnedArray<TimelineModel>* timelines);
    void setPlayheadPosition(ms_t timeMs);
    void setAutoFollow(bool shouldAutoFollow);
    void setCurrentTimeline(int index);
    int getCurrentTimelineIndex() const { return currentTimelineIndex; }
    int getNumTimelines() const { return timelines != nullptr ? timelines->size() : 0; }

    // ScrollBar::Listener
    void scrollBarMoved(juce::ScrollBar* scrollBar, double newRangeStart) override;
    
    // ComboBox::Listener
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    // Component
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // MouseListener
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;

private:
    struct ClipBounds
    {
        int timelineIndex;
        int layerIndex;
        int clipIndex;
        bool isMovementClip;
        juce::Rectangle<float> bounds;
        bool isResizeLeft;
        bool isResizeRight;
    };

    struct DragState
    {
        bool isDragging = false;
        bool isResizing = false;
        int timelineIndex = -1;
        int layerIndex = -1;
        int clipIndex = -1;
        bool isMovementClip = false;
        ms_t dragStartTime = 0;
        ms_t originalStart = 0;
        ms_t originalLength = 0;
        juce::Point<int> dragStartPosition;
    };

    juce::OwnedArray<TimelineModel>* timelines = nullptr;
    int currentTimelineIndex = 0;
    
    std::unique_ptr<juce::ScrollBar> horizontalScrollBar;
    std::unique_ptr<juce::ScrollBar> verticalScrollBar;
    std::unique_ptr<juce::ComboBox> timelineSelector;
    
    ms_t playheadPosition = 0;
    bool autoFollow = false;
    
    // Coordinate conversion methods
    float timeToX(ms_t time) const;
    ms_t xToTime(float x) const;
    float layerIndexToY(int layerIndex) const;
    int yToLayerIndex(float y) const;
    float timelineIndexToY(int timelineIndex) const;
    int yToTimelineIndex(float y) const;
    
    // Layout constants
    float headerHeight = 30.0f;
    float trackHeaderWidth = 150.0f;
    float trackHeight = 60.0f;
    float timelineSpacing = 20.0f;
    float timelineHeaderHeight = 40.0f;
    float clipHeight = 40.0f;
    float clipCornerSize = 4.0f;
    float resizeHandleWidth = 6.0f;
    
    // View state
    ms_t visibleStartTime = 0;
    ms_t visibleEndTime = 10000;
    float pixelsPerMillisecond = 0.1f;
    
    juce::Array<ClipBounds> visibleClips;
    DragState dragState;
    
    // Update methods
    void updateScrollBars();
    void updateVisibleClips();
    ClipBounds findClipAtPosition(const juce::Point<int>& position);
    void showClipContextMenu(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const juce::Point<int>& position);
    void timelineSelectionChanged();
    
    // Rendering helpers
    juce::Colour getClipColour(const Clip& clip) const;
    juce::String getClipDisplayName(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const;
    juce::String getClipTimeInfo(const Clip& clip) const;
    juce::Path getClipIcon(int layerIndex, bool isMovementClip) const;
    juce::Colour getTimelineColour(int timelineIndex) const;
    
    // Clip access helpers
    const Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip) const;
    Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip);
    bool removeClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    TimelineModel* getCurrentTimeline() const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
