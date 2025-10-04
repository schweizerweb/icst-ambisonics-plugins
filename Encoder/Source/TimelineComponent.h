#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "TimelineTypes.h"
#include "../../Common/PointSelection.h"
#include "../../Common/AmbiSourceSet.h"
#include <cmath>

class ClipEditorDialogManager;

class TimelineComponent : public juce::Component,
                          public juce::ScrollBar::Listener,
                          public juce::ChangeListener
{
public:
    TimelineComponent();
    ~TimelineComponent() override;

    void setTimelines(juce::OwnedArray<TimelineModel>* timelines);
    void setSelectionControl(PointSelection* pPointSelection);
    void setSourceSet(AmbiSourceSet* pSources);
    void setStatusMessageFunction(std::function<void(const juce::AttributedString&)> function);
    void setPlayheadPosition(ms_t timeMs);
    void setAutoFollow(bool shouldAutoFollow);
    void autoResizeBasedOnContent();
    void setCurrentTimeline(int index);
    int getCurrentTimelineIndex() const { return currentTimelineIndex; }
    int getNumTimelines() const { return timelines != nullptr ? timelines->size() : 0; }

    // ScrollBar::Listener
    void scrollBarMoved(juce::ScrollBar* scrollBar, double newRangeStart) override;
    
    int calculateTotalContentHeight() const;
    
    // Component
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // MouseListener
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;

    // Keyboard
    bool keyPressed(const juce::KeyPress& key) override;
    void nudgeSelectedClips(ms_t nudgeAmount);
    
    // ChangeListener method
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    ms_t getPlayheadPosition() const { return playheadPosition; }
    ms_t maxDuration = 60000;
    
    // zoom
    static constexpr float ZOOM_RESET = 0.0f;
    static constexpr float ZOOM_MAX = 10.0f; // pixel per ms
    
    void zoom(float zoomFactor)
    {
        if(approximatelyEqual(zoomFactor, ZOOM_RESET))
        {
            pixelsPerMillisecond = DEFAULT_PIXELS_PER_MS;
        }
        else
        {
            float zoomMin = getWidth() / maxDuration;
            pixelsPerMillisecond = juce::jlimit(zoomMin, ZOOM_MAX, pixelsPerMillisecond * zoomFactor);
        }
        
        updateScrollBars();
        repaint();
    }
    
    AmbiSourceSet* getSources() const;
    PointSelection* getPointSelection() const;
    TimelineModel* getTimelineModel(int timelineIndex) const;
    TimelineModel* getCurrentTimeline() const;
    const juce::Array<SelectedClip>& getSelectedClips() const { return selectedClips; }
    
    // Selection methods
    void selectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, bool addToSelection = false);
    void deselectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    void toggleClipSelection(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    bool isClipSelected(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const;
    void selectClipsInRectangle(const juce::Rectangle<int>& rect, bool addToSelection = false);
    void clearSelection();
    
    // Clip access helpers
    const Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip) const;
    Clip* getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip);
    bool removeClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    bool duplicateClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip);
    void showClipEditor(int timelineIndex, int clipIndex, bool isMovementClip);
    
    // Cursor control methods
    void setCursorTime(ms_t time);
    ms_t getCursorTime() const;
    void updatePreviewCursor(const juce::Point<int>& mousePos);
    void hidePreviewCursor();
    
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
        
        // Clip information for tooltips
        juce::String displayName;
        juce::String timeInfo;
        juce::String fullInfo;
    
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
    float trackHeight = 50.0f;
    float timelineSpacing = 20.0f;
    float timelineHeaderHeight = 20.0f;
    float clipHeight = 40.0f;
    float clipCornerSize = 4.0f;
    float resizeHandleWidth = 6.0f;
    const int scrollBarSize = 16;
    
    // constants for nudge amounts
    static constexpr ms_t NUDGE_SMALL_MS = 10;    // 10ms nudge
    static constexpr ms_t NUDGE_MEDIUM_MS = 100;  // 100ms nudge
    static constexpr ms_t NUDGE_LARGE_MS = 1000;  // 1 second nudge
    static constexpr float DEFAULT_PIXELS_PER_MS = 0.1f;
    
    // View state
    ms_t visibleStartTime = 0;
    ms_t visibleEndTime = 10000;
    float pixelsPerMillisecond = DEFAULT_PIXELS_PER_MS;
   
    // Update methods
    void updateScrollBars();
    void repositionScrollBars();
    ClipBounds findClipAtPosition(const juce::Point<int>& position);
    juce::Array<ClipBounds> findAllClipsAtPosition(const juce::Point<int>& position) const;
    ClipBounds findMostHiddenClip(const juce::Array<ClipBounds>& clips, const juce::Point<int>& position) const;
    void showClipContextMenu(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const juce::Point<int>& position);
    void timelineSelectionChanged();
    
    // Rendering helpers
    juce::Colour getClipColour(const Clip& clip) const;
    juce::String getClipDisplayName(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const;
    juce::String getClipTimeInfo(const Clip& clip) const;
    juce::Colour getTimelineColour(int timelineIndex) const;
    juce::Rectangle<float> getIconBoundsWithinClip(const juce::Rectangle<float>& clipBounds);
    juce::Rectangle<float> getButtonBoundsWithinClip(const juce::Rectangle<float>& clipBounds);
    
    std::unique_ptr<juce::Drawable> movementIcon;
    std::unique_ptr<juce::Drawable> actionIcon;
    std::unique_ptr<juce::Drawable> scaledMovementIcon;
    std::unique_ptr<juce::Drawable> scaledActionIcon;
    float lastIconSize = 0.0f;
    
    void updateScaledIcons(float iconSize);
    void loadIcons();
    
    // Bitmap cache for header time markers
    std::unique_ptr<juce::Image> headerCache;
    ms_t cachedHeaderVisibleStartTime = 0;
    ms_t cachedHeaderVisibleEndTime = 0;
    float cachedHeaderPixelsPerMillisecond = 0.0f;
    int cachedHeaderWidth = 0;
    int cachedHeaderHeight = 0;
    
    void renderHeaderToCache();
    void drawHeader(juce::Graphics& g);
    
    /// Cursors for placing new clips
    ms_t cursorPosition = 0;
    ms_t previewCursorPosition = 0;
    bool cursorVisible = false;
    bool previewCursorVisible = false;
    
    // Methods for cursor handling
    void setCursorPosition(ms_t time);
    void showCursor(bool shouldShow);
    void setPreviewCursorPosition(ms_t time);
    void showPreviewCursor(bool shouldShow);
    ms_t getCursorPosition() const { return cursorPosition; }
    bool shouldShowClipText(const juce::Rectangle<float>& clipBounds, float iconSize) const;
    
    juce::String generateDuplicateClipId(const juce::String& originalId);
    
    std::unique_ptr<ClipEditorDialogManager> clipEditorManager;
    PointSelection* pPointSelectionControl = nullptr;
    AmbiSourceSet* pSourceSet = nullptr;
    void syncTimelineSelectionToPointSelection();
    void syncPointSelectionToTimelineSelection();
    
    std::function<void(const juce::AttributedString&)> statusMessageFunction;
    juce::String formatTimeWithSeparators(ms_t timeMs) const;
    juce::String generateClipFullInfo(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const Clip& clip) const;
    juce::Colour getClipColourFromTimeline(int timelineIndex) const;
    
    // Timeline validation state
    struct TimelineValidationState
    {
        bool isValid = true;
        bool hasClips = false;
    };
    juce::Array<TimelineValidationState> timelineValidationStates;
    
    // Methods for timeline management
    void updateTimelineValidationStates();
    void validateTimelineSelection();
    void syncTimelinesToGroupCount();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineComponent)
};
