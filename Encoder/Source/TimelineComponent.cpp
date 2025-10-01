#include "TimelineComponent.h"

TimelineComponent::TimelineComponent()
{
    loadIcons(); // Load the SVG icons
    
    horizontalScrollBar = std::make_unique<juce::ScrollBar>(true);
    timelineSelector = std::make_unique<juce::ComboBox>();
    tempButton = std::make_unique<juce::TextButton>();
    
    horizontalScrollBar->addListener(this);
    
    timelineSelector->addListener(this);
    timelineSelector->setTextWhenNoChoicesAvailable("No Timelines");
    
    addAndMakeVisible(horizontalScrollBar.get());
    addAndMakeVisible(timelineSelector.get());
    //addAndMakeVisible(tempButton.get());
    
    timelineSelector->setVisible(false);
    horizontalScrollBar->setVisible(true);
    
    auto setScrollbarColors = [](juce::ScrollBar* scrollbar) {
        scrollbar->setColour(juce::ScrollBar::backgroundColourId, juce::Colour(0xffff0000));
        scrollbar->setColour(juce::ScrollBar::thumbColourId, juce::Colour(0xff00ff00));
        scrollbar->setColour(juce::ScrollBar::trackColourId, juce::Colour(0xff0000ff));
        
        // Force the scrollbar to be opaque
        scrollbar->setOpaque(true);
    };
    
    setScrollbarColors(horizontalScrollBar.get());
    
    // Make scrollbars always visible for testing
    horizontalScrollBar->setAlwaysOnTop(true);
    
    setWantsKeyboardFocus(true);
    setFocusContainerType(FocusContainerType::keyboardFocusContainer);
}

TimelineComponent::~TimelineComponent()
{
    horizontalScrollBar->removeListener(this);
    timelineSelector->removeListener(this);
}

void TimelineComponent::setTimelines(juce::OwnedArray<TimelineModel>* newTimelines)
{
    timelines = newTimelines;
    
    // Update timeline selector
    timelineSelector->clear();
    if (timelines != nullptr)
    {
        for (int i = 0; i < timelines->size(); ++i)
        {
            timelineSelector->addItem("Group " + juce::String(i + 1), i + 1);
        }
        if (timelines->size() > 0)
        {
            timelineSelector->setSelectedId(1, juce::sendNotificationSync);
        }
    }
    
    // Auto-resize based on content
    const float totalHeight = calculateTotalContentHeight();
    setSize(getWidth(), static_cast<int>(totalHeight));
    
    updateScrollBars();
    repaint();
}

void TimelineComponent::setCurrentTimeline(int index)
{
    if (timelines != nullptr && index >= 0 && index < timelines->size())
    {
        currentTimelineIndex = index;
        timelineSelector->setSelectedId(index + 1, juce::sendNotificationSync);
        repaint();
    }
}

void TimelineComponent::setPlayheadPosition(ms_t timeMs)
{
    playheadPosition = timeMs;
    
    if (autoFollow && playheadPosition > visibleEndTime - 2000)
    {
        visibleStartTime = playheadPosition - 5000;
        visibleEndTime = (ms_t)(visibleStartTime + (getWidth() - trackHeaderWidth) / pixelsPerMillisecond);
        updateScrollBars();
    }
    
    repaint();
}

void TimelineComponent::setAutoFollow(bool shouldAutoFollow)
{
    autoFollow = shouldAutoFollow;
}

void TimelineComponent::scrollBarMoved(juce::ScrollBar* scrollBar, double newRangeStart)
{
    if (scrollBar == horizontalScrollBar.get())
    {
        visibleStartTime = static_cast<ms_t>(newRangeStart);
        visibleEndTime = (ms_t)(visibleStartTime + (getWidth() - trackHeaderWidth) / pixelsPerMillisecond);
    }
    
    repaint();
}

juce::Rectangle<float> TimelineComponent::getIconBoundsWithinClip(const Rectangle<float>& clipBounds)
{
    const float iconSize = (clipHeight - 10.0f) * 0.5f;
    const float iconX = clipBounds.getX() + 7.0f;
    const float iconY = clipBounds.getY() + (clipBounds.getHeight() - 2.0f * iconSize) * 0.25f;
    
    return Rectangle<float>(iconX, iconY, iconSize, iconSize);
}

juce::Rectangle<float> TimelineComponent::getButtonBoundsWithinClip(const Rectangle<float>& clipBounds)
{
    const float btnSize = (clipHeight - 10.0f) * 0.5f;
    const float btnX = clipBounds.getX() + 7.0f;
    const float btnY = clipBounds.getBottom() - btnSize - (clipBounds.getHeight() - 2.0f * btnSize) * 0.25f;
    
    return Rectangle<float>(btnX, btnY, btnSize, btnSize);
}

void TimelineComponent::paint(juce::Graphics& g)
{
    // Calculate the content area (excluding scrollbars)
    auto contentArea = getLocalBounds();
    if (horizontalScrollBar->isVisible())
        contentArea.removeFromBottom(horizontalScrollBar->getHeight());
    
    // Only fill the content area, not the scrollbar area
    g.setColour(juce::Colour(0xff2d2d30));
    g.fillRect(contentArea);
    
    if (timelines == nullptr || timelines->size() == 0)
        return;
    
    // Make sure all your timeline painting stays within contentArea
    const auto totalWidth = contentArea.getWidth();
    const auto totalHeight = contentArea.getHeight();

    const auto timelineWidth = totalWidth - trackHeaderWidth;
    const auto timelineX = trackHeaderWidth;
    
    // Draw header background
    g.setColour(juce::Colour(0xff252526));
    g.fillRect(timelineX, 0.0f, timelineWidth, headerHeight);
    
    // Draw track headers background
    g.setColour(juce::Colour(0xff333337));
    g.fillRect(0.0f, headerHeight, trackHeaderWidth, totalHeight - headerHeight);
    
    // Draw time markers
    g.setColour(juce::Colours::lightgrey.withAlpha(0.3f));
    g.setFont(juce::FontOptions(12.0f));
    
    const ms_t timeStep = 1000;
    ms_t currentTime = (visibleStartTime / timeStep) * timeStep;
    
    while (currentTime <= visibleEndTime)
    {
        const float x = timeToX(currentTime);
        if (x >= timelineX && x <= totalWidth)
        {
            g.drawVerticalLine(static_cast<int>(x), headerHeight, totalHeight);
            
            juce::String timeText;
            if (currentTime < 60000)
                timeText = juce::String(currentTime) + "ms";
            else
                timeText = juce::String(currentTime / 1000.0, 1) + "s";
            
            g.drawText(timeText,
                      static_cast<int>(x) + 2, 2, 60, (int)(headerHeight - 4),
                      juce::Justification::left);
        }
        currentTime += timeStep;
    }
    
    // Draw timelines
    updateVisibleClips();
    
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        // Apply vertical scroll offset to Y position
        const float timelineY = timelineIndexToY(timelineIndex);
        
        // Only draw if this timeline is visible (within viewport)
        if (timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight < 0)
            continue; // Above viewport
        if (timelineY > getHeight())
            continue; // Below viewport
        
        const bool isCurrentTimeline = (timelineIndex == currentTimelineIndex);
        
        // Draw timeline header
        g.setColour(getTimelineColour(timelineIndex).withAlpha(isCurrentTimeline ? 0.8f : 0.5f));
        g.fillRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight);
        
        // Draw timeline name
        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(16.0f, juce::Font::bold));
        g.drawText("Group " + juce::String(timelineIndex + 1),
                   10, (int)(timelineY + 5), (int)(trackHeaderWidth - 20), (int)(timelineHeaderHeight - 10),
                   juce::Justification::centredLeft);
        
        // Draw tracks for this timeline
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
            
            // Draw track background
            g.setColour(juce::Colour(0xff3e3e42).withAlpha(isCurrentTimeline ? 1.0f : 0.6f));
            g.fillRect(timelineX, trackY, timelineWidth, trackHeight);
            
            // Draw track name
            g.setColour(juce::Colours::white.withAlpha(isCurrentTimeline ? 1.0f : 0.7f));
            g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
            g.drawText(timeline->getLayerName(layerIndex),
                      10, (int)trackY, (int)(trackHeaderWidth - 20), (int)trackHeight,
                      juce::Justification::centredLeft);
            
            // Draw track separator
            g.setColour(juce::Colours::black.withAlpha(0.5f));
            g.drawLine(0, (int)(trackY + trackHeight), (int)totalWidth, (int)(trackY + trackHeight), 1.0f);
        }
        
        // Draw timeline separator
        if (timelineIndex < timelines->size() - 1)
        {
            g.setColour(juce::Colours::white.withAlpha(0.3f));
            g.drawLine(0, timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing / 2,
                      totalWidth, timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing / 2, 2.0f);
        }
    }
    
    // Draw visible clips
    for (const auto& clipBounds : visibleClips)
    {
        bool isMovementClip = clipBounds.isMovementClip;
        const auto* clip = getClip(clipBounds.timelineIndex, clipBounds.layerIndex, clipBounds.clipIndex, isMovementClip);
        if (!clip) continue;
        
        const bool isCurrentTimeline = (clipBounds.timelineIndex == currentTimelineIndex);
        const auto bounds = clipBounds.bounds;
        
        // Check if this clip is selected
        const bool isSelected = isClipSelected(clipBounds.timelineIndex, clipBounds.layerIndex,
                                              clipBounds.clipIndex, clipBounds.isMovementClip);
        
        // Clip background with alpha based on current timeline
        g.setColour(getClipColour(*clip).withAlpha(isCurrentTimeline ? 0.8f : 0.4f));
        g.fillRoundedRectangle(bounds, clipCornerSize);
        
        // Clip border - thicker and white if selected
        if (isSelected)
        {
            g.setColour(juce::Colours::white);
            g.drawRoundedRectangle(bounds, clipCornerSize, 3.0f); // Thicker border for selection
        }
        else
        {
            g.setColour(juce::Colours::white.withAlpha(isCurrentTimeline ? 0.3f : 0.1f));
            g.drawRoundedRectangle(bounds, clipCornerSize, 1.0f);
        }
        
        // Only show resize handles for current timeline and selected clips
        if (isCurrentTimeline && isSelected && (clipBounds.isResizeLeft || clipBounds.isResizeRight))
        {
            g.setColour(juce::Colours::white.withAlpha(0.6f));
            if (clipBounds.isResizeLeft)
                g.fillRect(bounds.getX(), bounds.getY(), resizeHandleWidth, bounds.getHeight());
            if (clipBounds.isResizeRight)
                g.fillRect(bounds.getRight() - resizeHandleWidth, bounds.getY(),
                          resizeHandleWidth, bounds.getHeight());
        }
        
        // Clip content (faded for non-current timelines)
        Rectangle<float> iconBounds = getIconBoundsWithinClip(bounds);
        
        // Update scaled icons if needed
        updateScaledIcons(iconBounds.getWidth());

        // Draw icon using pre-scaled SVG
        g.setColour(juce::Colours::white.withAlpha(isCurrentTimeline ? 1.0f : 0.5f));

        juce::Drawable* iconDrawable = nullptr;
        if (clipBounds.isMovementClip && scaledMovementIcon != nullptr)
        {
            iconDrawable = scaledMovementIcon.get();
        }
        else if (!clipBounds.isMovementClip && scaledActionIcon != nullptr)
        {
            iconDrawable = scaledActionIcon.get();
        }

        if (iconDrawable != nullptr)
        {
            auto transform = juce::AffineTransform::translation(iconBounds.getX(), iconBounds.getY());
            iconDrawable->draw(g, 1.0f, transform);
        }
        
        // Draw text
        const float textX = iconBounds.getRight() + 5.0f;
        const float textWidth = jmax(10.0f, bounds.getWidth() - (iconBounds.getWidth() + 10.0f));
        
        g.setFont(juce::FontOptions(12.0f, juce::Font::bold));
        g.drawText(getClipDisplayName(clipBounds.timelineIndex, clipBounds.layerIndex, clipBounds.clipIndex, clipBounds.isMovementClip),
                   (int)textX, (int)(bounds.getY() + 5.0f), (int)textWidth, 16,
                   juce::Justification::left);
        
        g.setFont(juce::FontOptions(10.0f));
        g.drawText(getClipTimeInfo(*clip),
                   (int)textX, (int)(bounds.getY() + 22.0f), (int)textWidth, 14,
                   juce::Justification::left);
        
        // Menu button only for current timeline and selected clips
        if (isCurrentTimeline && isSelected)
        {
            Rectangle<float> buttonBounds = getButtonBoundsWithinClip(bounds);
            
            g.setColour(juce::Colours::white.withAlpha(0.7f));
            g.fillEllipse(buttonBounds);
            
            g.setColour(juce::Colours::black);
            g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
            g.drawText("...", buttonBounds,
                       juce::Justification::centred);
        }
    }
    
    // Draw selection rectangle
    if (dragState.isRectangleSelecting && !dragState.selectionRectangle.isEmpty())
    {
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.fillRect(dragState.selectionRectangle);
        
        g.setColour(juce::Colours::white);
        g.drawRect(dragState.selectionRectangle, 1.0f);
    }
    
    // Draw playhead
    if (playheadPosition >= visibleStartTime && playheadPosition <= visibleEndTime)
    {
        const float playheadX = timeToX(playheadPosition);
        g.setColour(juce::Colours::red);
        g.drawLine(playheadX, headerHeight, playheadX, totalHeight, 2.0f);
    }
}

void TimelineComponent::resized()
{
    const auto totalWidth = getWidth();
    const auto totalHeight = getHeight();
    
    timelineSelector->setBounds(10, 5, 150, 24);
    
    horizontalScrollBar->setBounds(0, totalHeight - scrollBarSize, totalWidth, scrollBarSize);
    
    tempButton->setBounds(2, 2, 200, 20);
    updateScrollBars();
}

void TimelineComponent::mouseDown(const juce::MouseEvent& event)
{
    if (timelines == nullptr || timelines->size() == 0) return;
    
    const auto pos = event.getPosition();
    
    // Check if we're starting a rectangle selection (Ctrl/Cmd + drag)
    if ((event.mods.isCommandDown() || event.mods.isCtrlDown()) && !event.mods.isShiftDown())
    {
        dragState.isRectangleSelecting = true;
        dragState.dragStartPosition = pos;
        dragState.selectionRectangle.setPosition(pos.x, pos.y);
        dragState.selectionRectangle.setSize(0, 0);
        return;
    }
    
    // Check if click is on a clip
    auto clipBounds = findClipAtPosition(pos);
    
    if (clipBounds.timelineIndex != -1)
    {
        dragState.isDragging = true;
        dragState.timelineIndex = clipBounds.timelineIndex;
        dragState.layerIndex = clipBounds.layerIndex;
        dragState.clipIndex = clipBounds.clipIndex;
        dragState.isMovementClip = clipBounds.isMovementClip;
        dragState.dragStartPosition = pos;
        
        // Set resize flags directly from the detected bounds
        dragState.isResizingLeft = clipBounds.isResizeLeft;
        dragState.isResizingRight = clipBounds.isResizeRight;
        dragState.isResizing = clipBounds.isResizeLeft || clipBounds.isResizeRight;
        
        // Handle selection based on modifier keys
        bool addToSelection = event.mods.isShiftDown();
        bool toggleSelection = (event.mods.isCommandDown() || event.mods.isCtrlDown()) && !dragState.isResizing;
        
        if (toggleSelection)
        {
            toggleClipSelection(clipBounds.timelineIndex, clipBounds.layerIndex,
                               clipBounds.clipIndex, clipBounds.isMovementClip);
        }
        else if (addToSelection)
        {
            selectClip(clipBounds.timelineIndex, clipBounds.layerIndex,
                      clipBounds.clipIndex, clipBounds.isMovementClip, true);
        }
        else if (!dragState.isResizing) // Only clear selection if we're not resizing
        {
            // Single selection - clear others
            selectClip(clipBounds.timelineIndex, clipBounds.layerIndex,
                      clipBounds.clipIndex, clipBounds.isMovementClip, false);
        }
        
        bool isMovementClip = clipBounds.isMovementClip;
        auto* clip = getClip(clipBounds.timelineIndex, clipBounds.layerIndex,
                            clipBounds.clipIndex, isMovementClip);
        if (clip)
        {
            dragState.dragStartTime = xToTime(static_cast<float>(pos.x));
            dragState.originalStart = clip->start;
            dragState.originalLength = clip->length;
        }
        
        // Check if menu button was clicked (only for current timeline)
        if (clipBounds.timelineIndex == currentTimelineIndex)
        {
            const auto bounds = clipBounds.bounds;
            auto buttonArea = getButtonBoundsWithinClip(bounds);
            
            if (buttonArea.contains(pos.toFloat()))
            {
                showClipContextMenu(clipBounds.timelineIndex, clipBounds.layerIndex,
                                   clipBounds.clipIndex, clipBounds.isMovementClip, pos);
                dragState.isDragging = false;
            }
        }
        
        // Switch to clicked timeline
        setCurrentTimeline(clipBounds.timelineIndex);
        
        repaint();
    }
    else
    {
        // Clicked on empty space - clear selection unless shift/ctrl is held
        if (!event.mods.isAnyModifierKeyDown())
        {
            clearSelection();
            repaint();
        }
    }
}

void TimelineComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (dragState.isRectangleSelecting)
    {
        // Update selection rectangle
        auto currentPos = event.getPosition();
        dragState.selectionRectangle = juce::Rectangle<int>::leftTopRightBottom(
            juce::jmin(dragState.dragStartPosition.x, currentPos.x),
            juce::jmin(dragState.dragStartPosition.y, currentPos.y),
            juce::jmax(dragState.dragStartPosition.x, currentPos.x),
            juce::jmax(dragState.dragStartPosition.y, currentPos.y));
        repaint();
        return;
    }
    
    if (!dragState.isDragging || timelines == nullptr) return;
    
    const auto currentPos = event.getPosition();
    const ms_t currentTime = xToTime(static_cast<float>(currentPos.x));
    const ms_t timeDelta = currentTime - dragState.dragStartTime;
    
    // Handle resizing (single clip only)
    if (dragState.isResizing)
    {
        bool isMovementClip = dragState.isMovementClip;
        auto* clip = getClip(dragState.timelineIndex, dragState.layerIndex, dragState.clipIndex, isMovementClip);
        if (!clip) return;
        
        if (dragState.isResizingLeft)
        {
            const ms_t newStart = juce::jmax<ms_t>(0, dragState.originalStart + timeDelta);
            const ms_t newLength = juce::jmax<ms_t>(10, dragState.originalLength - (newStart - dragState.originalStart));
            
            if (newLength > 10)
            {
                clip->start = newStart;
                clip->length = newLength;
            }
        }
        else if (dragState.isResizingRight)
        {
            const ms_t newLength = juce::jmax<ms_t>(10, dragState.originalLength + timeDelta);
            clip->length = newLength;
        }
    }
    // Handle moving clips (single or multiple)
    else
    {
        // If multiple clips are selected, move all of them
        if (selectedClips.size() > 1)
        {
            for (const auto& selected : selectedClips)
            {
                bool isMovementClip = selected.isMovementClip;
                auto* clip = getClip(selected.timelineIndex, selected.layerIndex, selected.clipIndex, isMovementClip);
                if (clip)
                {
                    clip->start = juce::jmax<ms_t>(0, clip->start + timeDelta);
                }
            }
            
            // Update drag start time for next movement
            dragState.dragStartTime = currentTime;
        }
        else
        {
            // Single clip movement
            bool isMovementClip = dragState.isMovementClip;
            auto* clip = getClip(dragState.timelineIndex, dragState.layerIndex, dragState.clipIndex, isMovementClip);
            if (clip)
            {
                clip->start = juce::jmax<ms_t>(0, dragState.originalStart + timeDelta);
            }
        }
    }
    
    repaint();
}

void TimelineComponent::mouseUp(const juce::MouseEvent& event)
{
    if (dragState.isRectangleSelecting)
    {
        // Finalize rectangle selection
        bool addToSelection = event.mods.isShiftDown();
        selectClipsInRectangle(dragState.selectionRectangle, addToSelection);
    }
    
    // Reset all drag state
    dragState = DragState();
    repaint();
}

void TimelineComponent::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    if (event.mods.isAltDown())
    {
        const float zoomFactor = wheel.deltaY > 0 ? 1.1f : 0.9f;
        pixelsPerMillisecond = juce::jlimit(0.01f, 1.0f, pixelsPerMillisecond * zoomFactor);
        updateScrollBars();
        repaint();
    }
    else
    {
        horizontalScrollBar->mouseWheelMove(event, wheel);
    }
}

void TimelineComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == timelineSelector.get())
    {
        timelineSelectionChanged();
    }
}

void TimelineComponent::timelineSelectionChanged()
{
    currentTimelineIndex = timelineSelector->getSelectedId() - 1;
    repaint();
}

bool TimelineComponent::keyPressed(const juce::KeyPress& key)
{
    // Nudge selected clips left/right with arrow keys
    if (selectedClips.size() > 0)
    {
        ms_t nudgeAmount = 0;
        bool isNudge = false;
        
        // Determine nudge amount based on modifier keys
        if (key.getModifiers().isShiftDown())
        {
            // Shift + arrow = large nudge (1 second)
            nudgeAmount = NUDGE_LARGE_MS;
        }
        else if (key.getModifiers().isAltDown() || key.getModifiers().isCommandDown() || key.getModifiers().isCtrlDown())
        {
            // Alt/Cmd/Ctrl + arrow = medium nudge (100ms)
            nudgeAmount = NUDGE_MEDIUM_MS;
        }
        else
        {
            // Arrow only = small nudge (10ms)
            nudgeAmount = NUDGE_SMALL_MS;
        }
        
        // Apply nudge based on arrow key direction
        if (key.getKeyCode() == juce::KeyPress::leftKey)
        {
            nudgeSelectedClips(-nudgeAmount);
            isNudge = true;
        }
        else if (key.getKeyCode() == juce::KeyPress::rightKey)
        {
            nudgeSelectedClips(nudgeAmount);
            isNudge = true;
        }
        
        if (isNudge)
        {
            repaint();
            return true;
        }
    }
    
    // Existing keyboard shortcuts
    if (key.getKeyCode() == juce::KeyPress::escapeKey)
    {
        clearSelection();
        repaint();
        return true;
    }
    else if (key.getKeyCode() == juce::KeyPress::deleteKey || key.getKeyCode() == juce::KeyPress::backspaceKey)
    {
        // Delete all selected clips
        for (int i = selectedClips.size() - 1; i >= 0; --i)
        {
            const auto& selected = selectedClips.getReference(i);
            removeClip(selected.timelineIndex, selected.layerIndex, selected.clipIndex, selected.isMovementClip);
        }
        selectedClips.clear();
        repaint();
        return true;
    }
    else if (key.getKeyCode() == 'A' && (key.getModifiers().isCommandDown() || key.getModifiers().isCtrlDown()))
    {
        // Select all clips in current timeline
        if (auto* timeline = getCurrentTimeline())
        {
            selectedClips.clear();
            
            // Add movement clips
            for (int i = 0; i < timeline->movement.clips.size(); ++i)
            {
                SelectedClip clip;
                clip.timelineIndex = currentTimelineIndex;
                clip.layerIndex = 0;
                clip.clipIndex = i;
                clip.isMovementClip = true;
                selectedClips.add(clip);
            }
            
            // Add action clips
            for (int i = 0; i < timeline->actions.clips.size(); ++i)
            {
                SelectedClip clip;
                clip.timelineIndex = currentTimelineIndex;
                clip.layerIndex = 1;
                clip.clipIndex = i;
                clip.isMovementClip = false;
                selectedClips.add(clip);
            }
            
            repaint();
            return true;
        }
    }
    
    return false;
}

void TimelineComponent::nudgeSelectedClips(ms_t nudgeAmount)
{
    if (selectedClips.isEmpty() || nudgeAmount == 0)
        return;
    
    // Nudge all selected clips
    for (const auto& selected : selectedClips)
    {
        bool isMovementClip = selected.isMovementClip;
        auto* clip = getClip(selected.timelineIndex, selected.layerIndex, selected.clipIndex, isMovementClip);
        if (clip)
        {
            ms_t newStart = clip->start + nudgeAmount;
            
            // Ensure clip doesn't go below 0
            if (newStart < 0)
                newStart = 0;
            
            clip->start = newStart;
        }
    }
}

float TimelineComponent::timeToX(ms_t time) const
{
    return trackHeaderWidth + (time - visibleStartTime) * pixelsPerMillisecond;
}

ms_t TimelineComponent::xToTime(float x) const
{
    return visibleStartTime + static_cast<ms_t>((x - trackHeaderWidth) / pixelsPerMillisecond);
}

float TimelineComponent::layerIndexToY(int layerIndex) const
{
    return layerIndex * trackHeight;
}

int TimelineComponent::yToLayerIndex(float y) const
{
    return static_cast<int>(y / trackHeight);
}

float TimelineComponent::timelineIndexToY(int timelineIndex) const
{
    if (timelines == nullptr) return headerHeight;
    
    float y = headerHeight;
    for (int i = 0; i < timelineIndex; ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            y += timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing;
        }
    }
    return y;
}

int TimelineComponent::yToTimelineIndex(float y) const
{
    if (timelines == nullptr) return -1;
    
    float currentY = headerHeight;
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            float timelineHeight = timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing;
            if (y >= currentY && y < currentY + timelineHeight)
                return i;
            currentY += timelineHeight;
        }
    }
    return -1;
}

void TimelineComponent::updateScrollBars()
{
    if (timelines == nullptr || timelines->size() == 0)
    {
        horizontalScrollBar->setVisible(false);
        return;
    }
    
    // HORIZONTAL SCROLLBAR (keep your existing logic)
    ms_t maxDuration = 60000;
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            ms_t latestTime = 0;
            for (int layerIdx = 0; layerIdx < timeline->getNumLayers(); ++layerIdx)
            {
                for (int clipIdx = 0; clipIdx < timeline->getNumClips(layerIdx); ++clipIdx)
                {
                    bool isMovementClip = false;
                    if (auto* clip = getClip(i, layerIdx, clipIdx, isMovementClip))
                    {
                        latestTime = juce::jmax(latestTime, clip->end());
                    }
                }
            }
            maxDuration = juce::jmax(maxDuration, latestTime + 5000);
        }
    }
    
    float visibleWidthMs = (getWidth() - trackHeaderWidth) / pixelsPerMillisecond;
    bool needsHorizontalScroll = maxDuration > visibleWidthMs;
    horizontalScrollBar->setVisible(needsHorizontalScroll);
    
    if (needsHorizontalScroll)
    {
        horizontalScrollBar->setRangeLimits(0, maxDuration);
        horizontalScrollBar->setCurrentRange(visibleStartTime, visibleWidthMs);
        horizontalScrollBar->setSingleStepSize(1000);
    }
}

void TimelineComponent::updateVisibleClips()
{
    visibleClips.clear();
    if (timelines == nullptr) return;
    
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            const int numClips = timeline->getNumClips(layerIndex);
            for (int clipIndex = 0; clipIndex < numClips; ++clipIndex)
            {
                bool isMovementClip = false;
                const auto* clip = getClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
                if (!clip) continue;
                
                // Check if clip is visible in current view
                if (clip->end() < visibleStartTime || clip->start > visibleEndTime)
                    continue;
                
                ClipBounds bounds;
                bounds.timelineIndex = timelineIndex;
                bounds.layerIndex = layerIndex;
                bounds.clipIndex = clipIndex;
                bounds.isMovementClip = isMovementClip;
                
                const float x = timeToX(clip->start);
                const float width = clip->length * pixelsPerMillisecond;
                const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                bounds.bounds = juce::Rectangle<float>(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                // Check if resize handles would be visible
                bounds.isResizeLeft = (clip->start >= visibleStartTime);
                bounds.isResizeRight = (clip->end() <= visibleEndTime);
                
                // Only add if clip is vertically visible
                if (bounds.bounds.getBottom() >= 0 && bounds.bounds.getY() <= getHeight())
                    visibleClips.add(bounds);
            }
        }
    }
}

// Selection methods implementation
void TimelineComponent::selectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, bool addToSelection)
{
    if (!addToSelection)
    {
        selectedClips.clear();
    }
    
    SelectedClip newClip;
    newClip.timelineIndex = timelineIndex;
    newClip.layerIndex = layerIndex;
    newClip.clipIndex = clipIndex;
    newClip.isMovementClip = isMovementClip;
    
    // Check if not already selected
    if (!isClipSelected(timelineIndex, layerIndex, clipIndex, isMovementClip))
    {
        selectedClips.add(newClip);
    }
}

void TimelineComponent::deselectClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip)
{
    for (int i = selectedClips.size() - 1; i >= 0; --i)
    {
        if (selectedClips.getReference(i).equals(timelineIndex, layerIndex, clipIndex, isMovementClip))
        {
            selectedClips.remove(i);
            break;
        }
    }
}

void TimelineComponent::toggleClipSelection(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip)
{
    if (isClipSelected(timelineIndex, layerIndex, clipIndex, isMovementClip))
    {
        deselectClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
    }
    else
    {
        selectClip(timelineIndex, layerIndex, clipIndex, isMovementClip, true);
    }
}

juce::Array<TimelineComponent::ClipBounds> TimelineComponent::findAllClipsAtPosition(const juce::Point<int>& position) const
{
    juce::Array<ClipBounds> clipsAtPos;
    
    for (const auto& bounds : visibleClips)
    {
        if (bounds.bounds.contains(position.toFloat()))
        {
            clipsAtPos.add(bounds);
        }
    }
    
    return clipsAtPos;
}

TimelineComponent::ClipBounds TimelineComponent::findMostHiddenClip(const juce::Array<ClipBounds>& clips, const juce::Point<int>& /*position*/) const
{
    if (clips.size() == 1)
        return clips.getFirst();
    
    // Sort clips by how many other clips they're covered by
    struct ClipCoverage
    {
        ClipBounds bounds;
        int coverageCount = 0;
    };
    
    juce::Array<ClipCoverage> coverage;
    
    for (const auto& clip : clips)
    {
        ClipCoverage item;
        item.bounds = clip;
        
        // Count how many other clips cover this one
        for (const auto& other : clips)
        {
            if (&clip != &other && other.bounds.contains(clip.bounds.getCentre()))
            {
                item.coverageCount++;
            }
        }
        
        coverage.add(item);
    }
    
    // Find the clip with the highest coverage count (most hidden)
    int maxCoverage = -1;
    ClipBounds mostHidden = clips.getFirst();
    
    for (const auto& item : coverage)
    {
        if (item.coverageCount > maxCoverage)
        {
            maxCoverage = item.coverageCount;
            mostHidden = item.bounds;
        }
    }
    
    return mostHidden;
}

void TimelineComponent::clearSelection()
{
    selectedClips.clear();
}

bool TimelineComponent::isClipSelected(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const
{
    for (const auto& selected : selectedClips)
    {
        if (selected.equals(timelineIndex, layerIndex, clipIndex, isMovementClip))
        {
            return true;
        }
    }
    return false;
}

void TimelineComponent::selectClipsInRectangle(const juce::Rectangle<int>& rect, bool addToSelection)
{
    if (!addToSelection)
    {
        clearSelection();
    }
    
    for (const auto& clipBounds : visibleClips)
    {
        if (rect.intersects(clipBounds.bounds.toNearestInt()))
        {
            selectClip(clipBounds.timelineIndex, clipBounds.layerIndex,
                      clipBounds.clipIndex, clipBounds.isMovementClip, true);
        }
    }
}

TimelineComponent::ClipBounds TimelineComponent::findClipAtPosition(const juce::Point<int>& position)
{
    ClipBounds result;
    result.timelineIndex = -1;
    result.layerIndex = -1;
    result.clipIndex = -1;
    result.isMovementClip = false;
    result.isResizeLeft = false;
    result.isResizeRight = false;

    for (const auto& bounds : visibleClips)
    {
        if (bounds.bounds.contains(position.toFloat()))
        {
            result = bounds;
            
            // Check if click is on resize handles (only for current timeline and selected clips)
            if (bounds.timelineIndex == currentTimelineIndex &&
                isClipSelected(bounds.timelineIndex, bounds.layerIndex, bounds.clipIndex, bounds.isMovementClip))
            {
                const auto posX = static_cast<float>(position.x);
                const auto clipBounds = bounds.bounds;
                
                // Check left resize handle
                if (posX >= clipBounds.getX() && posX <= clipBounds.getX() + resizeHandleWidth)
                {
                    result.isResizeLeft = true;
                    result.isResizeRight = false;
                }
                // Check right resize handle
                else if (posX >= clipBounds.getRight() - resizeHandleWidth && posX <= clipBounds.getRight())
                {
                    result.isResizeLeft = false;
                    result.isResizeRight = true;
                }
                else
                {
                    result.isResizeLeft = false;
                    result.isResizeRight = false;
                }
            }
            else
            {
                result.isResizeLeft = false;
                result.isResizeRight = false;
            }
            
            break;
        }
    }
    
    return result;
}

void TimelineComponent::showClipContextMenu(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const juce::Point<int>& /*position*/)
{
    juce::PopupMenu menu;
    
    menu.addItem(1, "Edit Clip Properties");
    menu.addItem(2, "Delete Clip");
    menu.addSeparator();
    menu.addItem(3, "Duplicate Clip");
    
    menu.showMenuAsync(juce::PopupMenu::Options()
                       .withParentComponent(this),
                       [this, timelineIndex, layerIndex, clipIndex, isMovementClip](int result)
                       {
                           if (result == 1)
                           {
                               // Edit clip - implement based on your needs
                           }
                           else if (result == 2)
                           {
                               removeClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
                               repaint();
                           }
                           else if (result == 3)
                           {
                               // Duplicate clip - implement based on your needs
                           }
                       });
}

juce::Colour TimelineComponent::getClipColour(const Clip& clip) const
{
    return clip.colour;
}

juce::String TimelineComponent::getClipDisplayName(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip) const
{
    if (timelines == nullptr) return "Clip";
    
    bool tempIsMovementClip = isMovementClip;
    const auto* clip = getClip(timelineIndex, layerIndex, clipIndex, tempIsMovementClip);
    if (!clip) return "Clip";
    
    if (clip->id.isNotEmpty())
        return clip->id;
    
    return isMovementClip ? "Movement" : "Action";
}

juce::String TimelineComponent::getClipTimeInfo(const Clip& clip) const
{
    return juce::String(clip.start) + " - " + juce::String(clip.end()) + "ms";
}

juce::Colour TimelineComponent::getTimelineColour(int timelineIndex) const
{
    const juce::Colour colours[] = {
        juce::Colours::cornflowerblue,
        juce::Colours::mediumseagreen,
        juce::Colours::coral,
        juce::Colours::goldenrod,
        juce::Colours::plum,
        juce::Colours::lightsteelblue
    };
    
    return colours[timelineIndex % juce::numElementsInArray(colours)];
}

const Clip* TimelineComponent::getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip) const
{
    if (timelines == nullptr || timelineIndex < 0 || timelineIndex >= timelines->size())
        return nullptr;
    
    auto* timeline = timelines->getUnchecked(timelineIndex);
    if (timeline == nullptr) return nullptr;
    
    if (layerIndex == 0)
    {
        isMovementClip = true;
        if (clipIndex >= 0 && clipIndex < timeline->movement.clips.size())
            return static_cast<const Clip*>(&timeline->movement.clips.getReference(clipIndex));
    }
    else if (layerIndex == 1)
    {
        isMovementClip = false;
        if (clipIndex >= 0 && clipIndex < timeline->actions.clips.size())
            return static_cast<const Clip*>(&timeline->actions.clips.getReference(clipIndex));
    }
    
    return nullptr;
}

Clip* TimelineComponent::getClip(int timelineIndex, int layerIndex, int clipIndex, bool& isMovementClip)
{
    return const_cast<Clip*>(static_cast<const TimelineComponent*>(this)->getClip(timelineIndex, layerIndex, clipIndex, isMovementClip));
}

bool TimelineComponent::removeClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip)
{
    if (timelines == nullptr || timelineIndex < 0 || timelineIndex >= timelines->size())
        return false;
    
    auto* timeline = timelines->getUnchecked(timelineIndex);
    if (timeline == nullptr) return false;
    
    bool success = false;
    
    if (isMovementClip && layerIndex == 0)
    {
        if (clipIndex >= 0 && clipIndex < timeline->movement.clips.size())
        {
            timeline->movement.clips.remove(clipIndex);
            success = true;
        }
    }
    else if (!isMovementClip && layerIndex == 1)
    {
        if (clipIndex >= 0 && clipIndex < timeline->actions.clips.size())
        {
            timeline->actions.clips.remove(clipIndex);
            success = true;
        }
    }
    
    // Remove from selection if it was selected
    if (success)
    {
        deselectClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
    }
    
    return success;
}

TimelineModel* TimelineComponent::getCurrentTimeline() const
{
    if (timelines == nullptr || currentTimelineIndex < 0 || currentTimelineIndex >= timelines->size())
        return nullptr;
    
    return timelines->getUnchecked(currentTimelineIndex);
}

// In TimelineComponent.cpp
void TimelineComponent::loadIcons()
{
    // Load movement icon from embedded SVG
    if (BinaryData::movement_icon_svg != nullptr && BinaryData::movement_icon_svgSize > 0)
    {
        movementIcon = juce::Drawable::createFromImageData(BinaryData::movement_icon_svg, BinaryData::movement_icon_svgSize);
        if (movementIcon != nullptr)
        {
            movementIcon->replaceColour(juce::Colours::black, juce::Colours::white);
        }
    }
    
    // Load action icon from embedded SVG
    if (BinaryData::action_icon_svg != nullptr && BinaryData::action_icon_svgSize > 0)
    {
        actionIcon = juce::Drawable::createFromImageData(BinaryData::action_icon_svg, BinaryData::action_icon_svgSize);
        if (actionIcon != nullptr)
        {
            actionIcon->replaceColour(juce::Colours::black, juce::Colours::white);
        }
    }
}

void TimelineComponent::updateScaledIcons(float iconSize)
{
    if (lastIconSize == iconSize) return; // Already scaled to this size
    
    lastIconSize = iconSize;
    
    // Scale movement icon
    if (movementIcon != nullptr)
    {
        scaledMovementIcon = movementIcon->createCopy();
        scaledMovementIcon->setTransformToFit(juce::Rectangle<float>(0, 0, iconSize, iconSize),
                                            juce::RectanglePlacement::centred);
    }
    
    // Scale action icon
    if (actionIcon != nullptr)
    {
        scaledActionIcon = actionIcon->createCopy();
        scaledActionIcon->setTransformToFit(juce::Rectangle<float>(0, 0, iconSize, iconSize),
                                          juce::RectanglePlacement::centred);
    }
}

int TimelineComponent::calculateTotalContentHeight() const
{
    if (timelines == nullptr || timelines->size() == 0)
        return static_cast<int>(headerHeight);
    
    float totalHeight = headerHeight;
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            totalHeight += timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing;
        }
    }
    
    // Subtract the last timeline spacing
    if (timelines->size() > 0)
        totalHeight -= timelineSpacing;
    
    if(horizontalScrollBar->isVisible())
    {
        totalHeight += ( scrollBarSize + 4 );
    }
    
    return static_cast<int>(totalHeight);
}
