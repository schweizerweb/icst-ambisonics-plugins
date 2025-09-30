#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "TimelineTypes.h"

class TimelineComponent : public juce::Component,
                          public juce::ScrollBar::Listener,
                          public juce::ComboBox::Listener,
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

    // Keyboard
    bool keyPressed(const juce::KeyPress& key) override;
    void nudgeSelectedClips(ms_t nudgeAmount);
    
    ms_t getPlayheadPosition() const { return playheadPosition; }
    void setPixelsPerMillisecond(float newPixelsPerMs)
    {
        pixelsPerMillisecond = newPixelsPerMs;
        updateScrollBars();
        repaint();
    }
    
    float getPixelsPerMillisecond() const { return pixelsPerMillisecond; }
    TimelineModel* getCurrentTimeline() const;
    
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
        
        // For selection comparison
        bool equals(int t, int l, int c, bool m) const {
            return timelineIndex == t && layerIndex == l && clipIndex == c && isMovementClip == m;
        }
    };

    struct DragState
    {
        bool isDragging = false;
        bool isResizing = false;
        bool isResizingLeft = false;
        bool isResizingRight = false;
        bool isRectangleSelecting = false;
        int timelineIndex = -1;
        int layerIndex = -1;
        int clipIndex = -1;
        bool isMovementClip = false;
        ms_t dragStartTime = 0;
        ms_t originalStart = 0;
        ms_t originalLength = 0;
        juce::Point<int> dragStartPosition;
        juce::Rectangle<int> selectionRectangle;
    };

    juce::OwnedArray<TimelineModel>* timelines = nullptr;
    int currentTimelineIndex = 0;
    
    std::unique_ptr<juce::ScrollBar> horizontalScrollBar;
    std::unique_ptr<juce::ScrollBar> verticalScrollBar;
    std::unique_ptr<juce::ComboBox> timelineSelector;
    std::unique_ptr<juce::TextButton> tempButton;
    
    ms_t playheadPosition = 0;
    bool autoFollow = true;
    
    // Selection state
    juce::Array<SelectedClip> selectedClips;
    DragState dragState;
    
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
    
    // constants for nudge amounts
    static constexpr ms_t NUDGE_SMALL_MS = 10;    // 10ms nudge
    static constexpr ms_t NUDGE_MEDIUM_MS = 100;  // 100ms nudge
    static constexpr ms_t NUDGE_LARGE_MS = 1000;  // 1 second nudge
    
    // View state
    ms_t visibleStartTime = 0;
    ms_t visibleEndTime = 10000;
    float pixelsPerMillisecond = 0.1f;
    
    juce::Array<ClipBounds> visibleClips;
    
    // Update methods
    void updateScrollBars();
    void updateVisibleClips();
    ClipBounds findClipAtPosition(const juce::Point<int>& position);
    juce::Array<ClipBounds> findAllClipsAtPosition(const juce::Point<int>& position) const;
    ClipBounds findMostHiddenClip(const juce::Array<ClipBounds>& clips, const juce::Point<int>& position) const;
    void showClipContextMenu(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const juce::Point<int>& position);
    void timelineSelectionChanged();
    
    // Selection methods
    void selectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, bool addToSelection = false);
    void deselectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    void toggleClipSelection(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    void clearSelection();
    bool isClipSelected(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const;
    void selectClipsInRectangle(const juce::Rectangle<int>& rect, bool addToSelection = false);
    
    // Rendering helpers
    juce::Colour getClipColour(const Clip& clip) const;
    juce::String getClipDisplayName(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const;
    juce::String getClipTimeInfo(const Clip& clip) const;
    juce::Path getClipIcon(bool isMovementClip) const;
    juce::Colour getTimelineColour(int timelineIndex) const;
    juce::Rectangle<float> getIconBoundsWithinClip(const juce::Rectangle<float>& clipBounds);
    juce::Rectangle<float> getButtonBoundsWithinClip(const juce::Rectangle<float>& clipBounds);
    
    // Clip access helpers
    const Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip) const;
    Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip);
    bool removeClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
