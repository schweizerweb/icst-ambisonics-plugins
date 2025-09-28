#include "TimelineComponent.h"

TimelineComponent::TimelineComponent()
{
    horizontalScrollBar = std::make_unique<juce::ScrollBar>(true);
    verticalScrollBar = std::make_unique<juce::ScrollBar>(false);
    timelineSelector = std::make_unique<juce::ComboBox>();
    
    horizontalScrollBar->addListener(this);
    verticalScrollBar->addListener(this);
    
    timelineSelector->addListener(this);
    timelineSelector->setTextWhenNoChoicesAvailable("No Timelines");
    
    addAndMakeVisible(horizontalScrollBar.get());
    addAndMakeVisible(verticalScrollBar.get());
    addAndMakeVisible(timelineSelector.get());
    
    setWantsKeyboardFocus(true);
}

TimelineComponent::~TimelineComponent()
{
    horizontalScrollBar->removeListener(this);
    verticalScrollBar->removeListener(this);
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
            timelineSelector->addItem("Timeline " + juce::String(i + 1), i + 1);
        }
        if (timelines->size() > 0)
        {
            timelineSelector->setSelectedId(1, juce::sendNotificationSync);
        }
    }
    
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
        visibleEndTime = visibleStartTime + (getWidth() - trackHeaderWidth) / pixelsPerMillisecond;
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
        visibleEndTime = visibleStartTime + (getWidth() - trackHeaderWidth) / pixelsPerMillisecond;
    }
    
    repaint();
}

void TimelineComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2d2d30));
    
    if (timelines == nullptr || timelines->size() == 0)
        return;
    
    const auto totalWidth = getWidth();
    const auto totalHeight = getHeight();
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
    g.setFont(juce::Font(12.0f));
    
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
                      static_cast<int>(x) + 2, 2, 60, headerHeight - 4,
                      juce::Justification::left);
        }
        currentTime += timeStep;
    }
    
    // Draw playhead
    if (playheadPosition >= visibleStartTime && playheadPosition <= visibleEndTime)
    {
        const float playheadX = timeToX(playheadPosition);
        g.setColour(juce::Colours::red);
        g.drawLine(playheadX, headerHeight, playheadX, totalHeight, 2.0f);
    }
    
    // Draw timelines
    updateVisibleClips();
    
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        const bool isCurrentTimeline = (timelineIndex == currentTimelineIndex);
        
        // Draw timeline header
        g.setColour(getTimelineColour(timelineIndex).withAlpha(isCurrentTimeline ? 0.8f : 0.5f));
        g.fillRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight);
        
        // Draw timeline name
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(16.0f).boldened());
        g.drawText("Timeline " + juce::String(timelineIndex + 1),
                   10, timelineY + 5, trackHeaderWidth - 20, timelineHeaderHeight - 10,
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
            g.setFont(juce::Font(14.0f).boldened());
            g.drawText(timeline->getLayerName(layerIndex),
                      10, trackY, trackHeaderWidth - 20, trackHeight,
                      juce::Justification::centredLeft);
            
            // Draw track separator
            g.setColour(juce::Colours::black.withAlpha(0.5f));
            g.drawLine(0, trackY + trackHeight, totalWidth, trackY + trackHeight, 1.0f);
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
        
        // Clip background with alpha based on current timeline
        g.setColour(getClipColour(*clip).withAlpha(isCurrentTimeline ? 0.8f : 0.4f));
        g.fillRoundedRectangle(bounds, clipCornerSize);
        
        // Clip border
        g.setColour(juce::Colours::white.withAlpha(isCurrentTimeline ? 0.3f : 0.1f));
        g.drawRoundedRectangle(bounds, clipCornerSize, 1.0f);
        
        // Only show resize handles for current timeline
        if (isCurrentTimeline && (clipBounds.isResizeLeft || clipBounds.isResizeRight))
        {
            g.setColour(juce::Colours::white.withAlpha(0.6f));
            if (clipBounds.isResizeLeft)
                g.fillRect(bounds.getX(), bounds.getY(), resizeHandleWidth, bounds.getHeight());
            if (clipBounds.isResizeRight)
                g.fillRect(bounds.getRight() - resizeHandleWidth, bounds.getY(),
                          resizeHandleWidth, bounds.getHeight());
        }
        
        // Clip content (faded for non-current timelines)
        const float iconSize = clipHeight - 10.0f;
        const float iconX = bounds.getX() + 5.0f;
        const float iconY = bounds.getY() + (bounds.getHeight() - iconSize) * 0.5f;
        
        // Draw icon
        g.setColour(juce::Colours::white.withAlpha(isCurrentTimeline ? 1.0f : 0.5f));
        auto icon = getClipIcon(clipBounds.layerIndex, clipBounds.isMovementClip);
        if (!icon.isEmpty())
            g.fillPath(icon, icon.getTransformToScaleToFit(iconX, iconY, iconSize, iconSize, true));
        
        // Draw text
        const float textX = iconX + iconSize + 5.0f;
        const float textWidth = jmax(10.0f,bounds.getWidth() - (iconSize + 15.0f));
        
        g.setFont(juce::Font(12.0f).boldened());
        g.drawText(getClipDisplayName(clipBounds.timelineIndex, clipBounds.layerIndex, clipBounds.clipIndex, clipBounds.isMovementClip),
                   textX, bounds.getY() + 5.0f, textWidth, 16.0f,
                   juce::Justification::left);
        
        g.setFont(juce::Font(10.0f));
        g.drawText(getClipTimeInfo(*clip),
                   textX, bounds.getY() + 22.0f, textWidth, 14.0f,
                   juce::Justification::left);
        
        // Menu button only for current timeline
        if (isCurrentTimeline)
        {
            const float buttonSize = 16.0f;
            const float buttonX = bounds.getRight() - buttonSize - 3.0f;
            const float buttonY = bounds.getY() + 3.0f;
            
            g.setColour(juce::Colours::white.withAlpha(0.7f));
            g.fillEllipse(buttonX, buttonY, buttonSize, buttonSize);
            
            g.setColour(juce::Colours::black);
            g.setFont(juce::Font(10.0f).boldened());
            g.drawText("...", buttonX, buttonY, buttonSize, buttonSize,
                       juce::Justification::centred);
        }
    }
}

void TimelineComponent::resized()
{
    const int scrollBarSize = 16;
    const auto totalWidth = getWidth();
    const auto totalHeight = getHeight();
    
    // Timeline selector at top left
    timelineSelector->setBounds(10, 5, 150, 24);
    
    horizontalScrollBar->setBounds(trackHeaderWidth, totalHeight - scrollBarSize,
                                  totalWidth - trackHeaderWidth - scrollBarSize, scrollBarSize);
    verticalScrollBar->setBounds(totalWidth - scrollBarSize, headerHeight,
                                scrollBarSize, totalHeight - headerHeight - scrollBarSize);
    
    updateScrollBars();
}

void TimelineComponent::mouseDown(const juce::MouseEvent& event)
{
    if (timelines == nullptr || timelines->size() == 0) return;
    
    const auto pos = event.getPosition();
    
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
        
        bool isMovementClip = clipBounds.isMovementClip;
        auto* clip = getClip(clipBounds.timelineIndex, clipBounds.layerIndex, clipBounds.clipIndex, isMovementClip);
        if (clip)
        {
            dragState.dragStartTime = xToTime(static_cast<float>(pos.x));
            dragState.originalStart = clip->start;
            dragState.originalLength = clip->length;
            
            dragState.isResizing = clipBounds.isResizeLeft || clipBounds.isResizeRight;
        }
        
        // Check if menu button was clicked (only for current timeline)
        if (clipBounds.timelineIndex == currentTimelineIndex)
        {
            const auto bounds = clipBounds.bounds;
            const float buttonSize = 16.0f;
            const float buttonX = bounds.getRight() - buttonSize - 3.0f;
            const float buttonY = bounds.getY() + 3.0f;
            const juce::Rectangle<float> buttonArea(buttonX, buttonY, buttonSize, buttonSize);
            
            if (buttonArea.contains(pos.toFloat()))
            {
                showClipContextMenu(clipBounds.timelineIndex, clipBounds.layerIndex,
                                   clipBounds.clipIndex, clipBounds.isMovementClip, pos);
                dragState.isDragging = false;
            }
        }
        
        // Switch to clicked timeline
        setCurrentTimeline(clipBounds.timelineIndex);
    }
}

void TimelineComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (!dragState.isDragging || timelines == nullptr) return;
    
    bool isMovementClip = dragState.isMovementClip;
    auto* clip = getClip(dragState.timelineIndex, dragState.layerIndex, dragState.clipIndex, isMovementClip);
    if (!clip) return;
    
    const auto currentPos = event.getPosition();
    const ms_t currentTime = xToTime(static_cast<float>(currentPos.x));
    const ms_t timeDelta = currentTime - dragState.dragStartTime;
    
    if (dragState.isResizing)
    {
        auto clipBounds = findClipAtPosition(dragState.dragStartPosition);
        
        if (clipBounds.isResizeLeft)
        {
            const ms_t newStart = juce::jmax<ms_t>(0, dragState.originalStart + timeDelta);
            const ms_t newLength = dragState.originalLength - (newStart - dragState.originalStart);
            
            if (newLength > 10)
            {
                clip->start = newStart;
                clip->length = newLength;
            }
        }
        else if (clipBounds.isResizeRight)
        {
            const ms_t newLength = juce::jmax<ms_t>(10, dragState.originalLength + timeDelta);
            clip->length = newLength;
        }
    }
    else
    {
        clip->start = juce::jmax<ms_t>(0, dragState.originalStart + timeDelta);
    }
    
    repaint();
}

void TimelineComponent::mouseUp(const juce::MouseEvent&)
{
    dragState = DragState();
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
    if (timelines == nullptr || timelines->size() == 0) return;
    
    // Calculate total content width based on maximum timeline duration
    ms_t maxDuration = 60000; // Default 1 minute
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            // Find the latest clip end time in this timeline
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
            maxDuration = juce::jmax(maxDuration, latestTime + 5000); // Add some padding
        }
    }
    
    const float totalWidth = maxDuration * pixelsPerMillisecond;
    const float visibleWidth = getWidth() - trackHeaderWidth;
    
    horizontalScrollBar->setRangeLimits(0, maxDuration);
    horizontalScrollBar->setCurrentRange(visibleStartTime, visibleEndTime - visibleStartTime);
    horizontalScrollBar->setSingleStepSize(100);
    //horizontalScrollBar->setButtonIncSteps(1000);
    
    // Calculate total content height
    float totalHeight = headerHeight;
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            totalHeight += timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing;
        }
    }
    
    const float visibleHeight = getHeight() - headerHeight;
    verticalScrollBar->setRangeLimits(0, totalHeight);
    verticalScrollBar->setCurrentRange(0, visibleHeight);
}

void TimelineComponent::updateVisibleClips()
{
    visibleClips.clear();
    if (timelines == nullptr) return;
    
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
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
                const float timelineY = timelineIndexToY(timelineIndex);
                const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                bounds.bounds = juce::Rectangle<float>(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                // Check if resize handles would be visible
                bounds.isResizeLeft = (clip->start >= visibleStartTime);
                bounds.isResizeRight = (clip->end() <= visibleEndTime);
                
                visibleClips.add(bounds);
            }
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
    
    for (const auto& bounds : visibleClips)
    {
        if (bounds.bounds.contains(position.toFloat()))
        {
            result = bounds;
            
            // Check if click is on resize handles (only for current timeline)
            if (bounds.timelineIndex == currentTimelineIndex)
            {
                const auto posX = static_cast<float>(position.x);
                result.isResizeLeft = (posX - bounds.bounds.getX()) < resizeHandleWidth;
                result.isResizeRight = (bounds.bounds.getRight() - posX) < resizeHandleWidth;
            }
            
            break;
        }
    }
    
    return result;
}

void TimelineComponent::showClipContextMenu(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const juce::Point<int>& position)
{
    juce::PopupMenu menu;
    
    menu.addItem(1, "Edit Clip Properties");
    menu.addItem(2, "Delete Clip");
    menu.addSeparator();
    menu.addItem(3, "Duplicate Clip");
    
    menu.showMenuAsync(juce::PopupMenu::Options()
                       .withTargetScreenArea({position.x, position.y, 1, 1})
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
    if(!isMovementClip) return "ACT";
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

juce::Path TimelineComponent::getClipIcon(int layerIndex, bool isMovementClip) const
{
    juce::Path icon;
    
    if (isMovementClip)
    {
        // Movement icon (arrow)
        icon.addTriangle({0, 1}, {1, 0.5f}, {0, 0});
    }
    else
    {
        // Action icon (gear)
        icon.addEllipse(0.2f, 0.2f, 0.6f, 0.6f);
        
        for (int i = 0; i < 8; ++i)
        {
            const float angle = i * juce::MathConstants<float>::pi / 4.0f;
            juce::Path tooth;
            tooth.addRectangle(0.45f, 0.1f, 0.1f, 0.3f);
            tooth.applyTransform(juce::AffineTransform::rotation(angle, 0.5f, 0.5f));
            icon.addPath(tooth);
        }
    }
    
    return icon;
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
    else if (layerIndex - 1 < timeline->actions.size())
    {
        isMovementClip = false;
        if (clipIndex >= 0 && clipIndex < timeline->actions[layerIndex - 1].clips.size())
            return static_cast<const Clip*>(&timeline->actions[layerIndex - 1].clips.getReference(clipIndex));
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
    
    if (isMovementClip && layerIndex == 0)
    {
        if (clipIndex >= 0 && clipIndex < timeline->movement.clips.size())
        {
            timeline->movement.clips.remove(clipIndex);
            return true;
        }
    }
    else if (!isMovementClip && layerIndex - 1 < timeline->actions.size())
    {
        if (clipIndex >= 0 && clipIndex < timeline->actions[layerIndex - 1].clips.size())
        {
            timeline->actions[layerIndex - 1].clips.remove(clipIndex);
            return true;
        }
    }
    
    return false;
}

TimelineModel* TimelineComponent::getCurrentTimeline() const
{
    if (timelines == nullptr || currentTimelineIndex < 0 || currentTimelineIndex >= timelines->size())
        return nullptr;
    
    return timelines->getUnchecked(currentTimelineIndex);
}
