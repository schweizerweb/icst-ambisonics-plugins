#include "TimelineComponent.h"
#include "MovementClipEditor.h"
#include "ActionClipEditor.h"
#include "ClipEditorDialogManager.h"

TimelineComponent::TimelineComponent()
{
    loadIcons(); // Load the SVG icons
    
    horizontalScrollBar = std::make_unique<juce::ScrollBar>(false);
    horizontalScrollBar->addListener(this);
    addAndMakeVisible(horizontalScrollBar.get());
    
    clipEditorManager = std::make_unique<ClipEditorDialogManager>();
    
    setWantsKeyboardFocus(true);
    setFocusContainerType(FocusContainerType::keyboardFocusContainer);
}

TimelineComponent::~TimelineComponent()
{
    horizontalScrollBar->removeListener(this);
    
    // Remove as ChangeListener from PointSelection and AmbiSourceSet
    if (pPointSelectionControl != nullptr)
    {
        pPointSelectionControl->removeChangeListener(this);
    }
}

void TimelineComponent::setTimelines(juce::OwnedArray<TimelineModel>* newTimelines)
{
    timelines = newTimelines;
    
    maxDuration = 60000;
    // Find the latest clip end time across all timelines
    if (timelines != nullptr)
    {
        for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
        {
            auto* timeline = timelines->getUnchecked(timelineIndex);
            if (timeline == nullptr) continue;
            
            // Check movement clips (layer 0)
            for (int clipIndex = 0; clipIndex < timeline->movement.clips.size(); ++clipIndex)
            {
                const auto& clip = timeline->movement.clips.getReference(clipIndex);
                maxDuration = juce::jmax(maxDuration, clip.end());
            }
            
            // Check action clips (layer 1)
            for (int clipIndex = 0; clipIndex < timeline->actions.clips.size(); ++clipIndex)
            {
                const auto& clip = timeline->actions.clips.getReference(clipIndex);
                maxDuration = juce::jmax(maxDuration, clip.end());
            }
        }
    }

    autoResizeBasedOnContent();
    
    repaint();
}

void TimelineComponent::autoResizeBasedOnContent()
{
    const float totalHeight = calculateTotalContentHeight();
    setSize(getWidth(), static_cast<int>(totalHeight));
    
    updateScrollBars();
}

void TimelineComponent::setCurrentTimeline(int index)
{
    if (timelines != nullptr && index >= 0 && index < timelines->size())
    {
        currentTimelineIndex = index;
        
        // Sync to PointSelection
        syncTimelineSelectionToPointSelection();
        
        repaint();
    }
}

void TimelineComponent::setSelectionControl(PointSelection* pPointSelection)
{
    if(pPointSelectionControl != nullptr)
    {
        pPointSelectionControl->removeChangeListener(this);
    }
    
    pPointSelectionControl = pPointSelection;
    
    if (pPointSelectionControl != nullptr)
    {
        pPointSelectionControl->addChangeListener(this);
    }
}

void TimelineComponent::setSourceSet(AmbiSourceSet* pSources)
{
    pSourceSet = pSources;
}

void TimelineComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == pPointSelectionControl && pPointSelectionControl != nullptr)
    {
        // PointSelection changed - sync timeline selection
        syncPointSelectionToTimelineSelection();
    
        // check if group count changed
        int previousGroupCount = timelineValidationStates.size();
        int currentGroupCount = pSourceSet->activeGroupCount();
        
        if (currentGroupCount != previousGroupCount)
        {
            syncTimelinesToGroupCount();
        }
    }
}

void TimelineComponent::syncTimelinesToGroupCount()
{
    if (pSourceSet == nullptr) return;
    
    int currentGroupCount = pSourceSet->activeGroupCount();
    int currentTimelineCount = timelines != nullptr ? timelines->size() : 0;
    
    // Update validation states array size to match group count
    timelineValidationStates.resize(currentGroupCount);
    
    // If we have more groups than timelines, add new timelines
    if (currentGroupCount > currentTimelineCount && timelines != nullptr)
    {
        for (int i = currentTimelineCount; i < currentGroupCount; ++i)
        {
            timelines->add(new TimelineModel());
        }
    }
    // If we have more timelines than groups, remove empty ones
    else if (currentGroupCount < currentTimelineCount && timelines != nullptr)
    {
        // Remove timelines from the end that have no clips
        for (int i = currentTimelineCount - 1; i >= currentGroupCount; --i)
        {
            bool hasClips = false;
            
            if (auto* timeline = timelines->getUnchecked(i))
            {
                hasClips = (timeline->movement.clips.size() > 0) ||
                          (timeline->actions.clips.size() > 0);
            }
            
            // Only remove if timeline has no clips
            if (!hasClips)
            {
                timelines->remove(i);
                
                // If we're removing the current timeline, update selection
                if (i == currentTimelineIndex)
                {
                    currentTimelineIndex = juce::jmin(currentGroupCount - 1, 0);
                }
                // Adjust current timeline index if needed
                else if (i < currentTimelineIndex)
                {
                    currentTimelineIndex--;
                }
            }
        }
    }
    
    // Update validation states
    updateTimelineValidationStates();
    
    // Validate current selection
    validateTimelineSelection();
    
    autoResizeBasedOnContent();
    
    repaint();
}

void TimelineComponent::updateTimelineValidationStates()
{
    if (timelines == nullptr || pSourceSet == nullptr) return;
    
    int groupCount = pSourceSet->activeGroupCount();
    int timelineCount = timelines->size();
    
    // Ensure validation states array is the correct size
    timelineValidationStates.resize(groupCount);
    
    for (int i = 0; i < groupCount; ++i)
    {
        bool isValid = (i < timelineCount);
        bool hasClips = false;
        
        // Check if timeline exists and has clips
        if (isValid)
        {
            auto* timeline = timelines->getUnchecked(i);
            if (timeline != nullptr)
            {
                hasClips = (timeline->movement.clips.size() > 0) ||
                          (timeline->actions.clips.size() > 0);
            }
        }
        
        // Update validation state
        if (i < timelineValidationStates.size())
        {
            timelineValidationStates.getReference(i).isValid = isValid;
            timelineValidationStates.getReference(i).hasClips = hasClips;
        }
    }
    
    // Also check timelines beyond group count for invalidation
    for (int i = groupCount; i < timelineCount; ++i)
    {
        bool hasClips = false;
        
        if (auto* timeline = timelines->getUnchecked(i))
        {
            hasClips = (timeline->movement.clips.size() > 0) ||
                      (timeline->actions.clips.size() > 0);
        }
        
        // These timelines are always invalid (beyond group count)
        // We track them in a separate part of the array if needed
        // For now, we'll handle them in the painting logic
    }
}

void TimelineComponent::validateTimelineSelection()
{
    if (pSourceSet == nullptr) return;
    
    int groupCount = pSourceSet->activeGroupCount();
    
    // If current selection is beyond group count, change to first timeline
    if (currentTimelineIndex >= groupCount)
    {
        // Check if the invalid timeline has clips
        bool hasClips = false;
        if (currentTimelineIndex < timelines->size())
        {
            auto* timeline = timelines->getUnchecked(currentTimelineIndex);
            if (timeline != nullptr)
            {
                hasClips = (timeline->movement.clips.size() > 0) ||
                          (timeline->actions.clips.size() > 0);
            }
        }
        
        // Only change selection if the timeline doesn't have clips
        // If it has clips, we keep it selected but mark as invalid
        if (!hasClips)
        {
            setCurrentTimeline(juce::jmin(groupCount - 1, 0));
        }
    }
}

void TimelineComponent::setPlayheadPosition(ms_t timeMs)
{
    playheadPosition = timeMs;
    if (playheadPosition > maxDuration)
    {
        // Round up to the next multiple of 10000
        maxDuration = ((playheadPosition + 9999) / 10000) * 10000;
    }
    
    auto visibleTime = visibleEndTime - visibleStartTime;
    if (autoFollow)
    {
        if(playheadPosition > (visibleEndTime - 0.2 * visibleTime))
        {
            visibleStartTime = playheadPosition - (ms_t)(visibleTime * 0.2);
            visibleEndTime = (ms_t)(visibleStartTime + visibleTime);
            updateScrollBars();
        }
        else if(playheadPosition < visibleStartTime)
        {
            visibleStartTime = playheadPosition - (ms_t)(visibleTime * 0.5);
            visibleEndTime = (ms_t)(visibleStartTime + visibleTime);
            updateScrollBars();
        }
    
        setCursorTime(timeMs);
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
    
    // Draw time markers (using bitmap cache)
    drawHeader(g);
    
    // =========================================================================
    // TIMELINE-BY-TIMELINE PAINTING
    // =========================================================================
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        
        // Only draw if this timeline is visible (within viewport)
        if (timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight < 0)
            continue;
        if (timelineY > getHeight())
            continue;
        
        // Check timeline validation state
        bool isValid = true;
        bool hasClips = false;
        bool isDisabled = false;
        
        if (pSourceSet != nullptr)
        {
            int groupCount = pSourceSet->activeGroupCount();
            isValid = (timelineIndex < groupCount);
            
            // Check if timeline has clips
            hasClips = (timeline->movement.clips.size() > 0) ||
                      (timeline->actions.clips.size() > 0);
            
            isDisabled = !isValid && hasClips;
        }
        
        const bool isCurrentTimeline = (timelineIndex == currentTimelineIndex);
        
        // Draw timeline header
        juce::Colour headerColour = getTimelineColour(timelineIndex);
        if (isDisabled)
        {
            headerColour = headerColour.withMultipliedBrightness(0.5f).withMultipliedSaturation(0.7f);
        }
        else if (!isValid)
        {
            headerColour = headerColour.withMultipliedBrightness(0.3f).withMultipliedSaturation(0.5f);
        }
        
        if (isCurrentTimeline)
        {
            g.setColour(headerColour.withAlpha(isDisabled ? 0.6f : 0.9f));
            g.fillRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight);
            
            if (!isDisabled)
            {
                g.setColour(juce::Colours::white.withAlpha(0.8f));
                g.drawRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight, 2.0f);
            }
        }
        else
        {
            g.setColour(headerColour.withAlpha(isDisabled ? 0.3f : 0.4f));
            g.fillRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight);
            
            if (!isDisabled && isValid)
            {
                g.setColour(juce::Colours::white.withAlpha(0.2f));
                g.drawRect(0.0f, timelineY, (float)totalWidth, timelineHeaderHeight, 1.0f);
            }
        }
        
        // Draw timeline name
        juce::Colour textColour;
        if (isDisabled)
        {
            textColour = juce::Colours::lightgrey.withAlpha(0.5f);
        }
        else if (isCurrentTimeline)
        {
            textColour = juce::Colours::white;
        }
        else
        {
            textColour = juce::Colours::white.withAlpha(0.7f);
        }
        
        g.setColour(textColour);
        g.setFont(juce::FontOptions(isCurrentTimeline ? 16.0f : 14.0f,
                                   isCurrentTimeline ? juce::Font::bold : juce::Font::plain));
        
        juce::String timelineName = "Group " + juce::String(timelineIndex + 1);
        if (isDisabled)
        {
            timelineName += " (Invalid)";
        }
        else if (!isValid)
        {
            timelineName += " (No Source)";
        }
        
        g.drawText(timelineName,
                   10, (int)(timelineY + 5), (int)(trackHeaderWidth - 20), (int)(timelineHeaderHeight - 10),
                   juce::Justification::centredLeft);
        
        // Draw tracks and clips for this timeline
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
            
            // Draw track background
            juce::Colour trackColour;
            if (isDisabled)
            {
                trackColour = juce::Colour(0xff2a2a2a);
            }
            else if (isCurrentTimeline)
            {
                trackColour = juce::Colour(0xff4a4a4f);
            }
            else
            {
                trackColour = juce::Colour(0xff3e3e42).withAlpha(0.6f);
            }
            
            g.setColour(trackColour);
            g.fillRect(timelineX, trackY, timelineWidth, trackHeight);
            
            // Draw track name
            juce::Colour trackTextColour;
            if (isDisabled)
            {
                trackTextColour = juce::Colours::lightgrey.withAlpha(0.3f);
            }
            else if (isCurrentTimeline)
            {
                trackTextColour = juce::Colours::white;
            }
            else
            {
                trackTextColour = juce::Colours::white.withAlpha(0.6f);
            }
            
            g.setColour(trackTextColour);
            g.setFont(juce::FontOptions(isCurrentTimeline ? 14.0f : 13.0f,
                                       isCurrentTimeline ? juce::Font::bold : juce::Font::plain));
            
            juce::String trackName = layerIndex == 0 ? "Movement" : "Action";
            g.drawText(trackName,
                       10, (int)(trackY + 5), (int)(trackHeaderWidth - 20), (int)(trackHeight - 10),
                       juce::Justification::centredLeft);
            
            // Draw clips for this track
            bool isMovementClip = (layerIndex == 0);
            int numClips = isMovementClip ? timeline->movement.clips.size() : timeline->actions.clips.size();
            
            for (int clipIndex = 0; clipIndex < numClips; ++clipIndex)
            {
                const Clip* clip = isMovementClip ?
                    static_cast<const Clip*>(&timeline->movement.clips.getReference(clipIndex)) :
                    static_cast<const Clip*>(&timeline->actions.clips.getReference(clipIndex));
                
                if (!clip) continue;
                
                // Check if clip is visible in current view
                if (clip->end() < visibleStartTime || clip->start > visibleEndTime)
                    continue;
                
                // Calculate the visible portion of the clip
                ms_t visibleClipStart = juce::jmax(clip->start, visibleStartTime);
                ms_t visibleClipEnd = juce::jmin(clip->end(), visibleEndTime);
                ms_t visibleClipLength = visibleClipEnd - visibleClipStart;
                
                // Ensure we have at least some visible portion
                if (visibleClipLength <= 0)
                    continue;
                
                const float x = timeToX(visibleClipStart);
                const float width = visibleClipLength * pixelsPerMillisecond;
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                // Ensure minimum width for visibility and interaction
                juce::Rectangle<float> bounds(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                // Only draw if clip is vertically visible
                if (bounds.getBottom() >= 0 && bounds.getY() <= getHeight())
                {
                    // Check if this clip is selected
                    const bool isSelected = isClipSelected(timelineIndex, layerIndex, clipIndex, isMovementClip);
                    
                    // Clip background with disabled state
                    juce::Colour clipColour = getClipColour(*clip);
                    if (isDisabled)
                    {
                        clipColour = clipColour.withMultipliedBrightness(0.6f).withMultipliedSaturation(0.5f);
                    }
                    else if (isCurrentTimeline)
                    {
                        clipColour = clipColour.withAlpha(isSelected ? 0.9f : 0.8f);
                    }
                    else
                    {
                        clipColour = clipColour.withAlpha(isSelected ? 0.6f : 0.3f);
                    }
                    
                    g.setColour(clipColour);
                    g.fillRoundedRectangle(bounds, clipCornerSize);
                    
                    // Clip border
                    if (isSelected)
                    {
                        g.setColour(juce::Colours::white.withAlpha(isDisabled ? 0.4f : 1.0f));
                        g.drawRoundedRectangle(bounds, clipCornerSize, isCurrentTimeline ? 3.0f : 2.0f);
                    }
                    else if (!isDisabled)
                    {
                        if (isCurrentTimeline)
                        {
                            g.setColour(juce::Colours::white.withAlpha(0.4f));
                            g.drawRoundedRectangle(bounds, clipCornerSize, 1.5f);
                        }
                        else
                        {
                            g.setColour(juce::Colours::white.withAlpha(0.2f));
                            g.drawRoundedRectangle(bounds, clipCornerSize, 1.0f);
                        }
                    }
                    
                    // Clip content (faded for disabled timelines)
                    Rectangle<float> iconBounds = getIconBoundsWithinClip(bounds);
                    updateScaledIcons(iconBounds.getWidth());

                    float iconAlpha = isDisabled ? 0.3f : (isCurrentTimeline ? 1.0f : 0.5f);
                    g.setColour(juce::Colours::white.withAlpha(iconAlpha));

                    juce::Drawable* iconDrawable = nullptr;
                    if (isMovementClip && scaledMovementIcon != nullptr)
                    {
                        iconDrawable = scaledMovementIcon.get();
                    }
                    else if (!isMovementClip && scaledActionIcon != nullptr)
                    {
                        iconDrawable = scaledActionIcon.get();
                    }

                    if (iconDrawable != nullptr)
                    {
                        auto transform = juce::AffineTransform::translation(iconBounds.getX(), iconBounds.getY());
                        iconDrawable->draw(g, iconAlpha, transform);
                    }
                    
                    // Draw text only if there's enough space and timeline is not disabled
                    if (!isDisabled && shouldShowClipText(bounds, iconBounds.getWidth()))
                    {
                        const float textX = iconBounds.getRight() + 5.0f;
                        const float availableTextWidth = bounds.getWidth() - (iconBounds.getWidth() + 10.0f);
                        
                        juce::String clipName = getClipDisplayName(timelineIndex, layerIndex, clipIndex, isMovementClip);
                        juce::String timeInfo = getClipTimeInfo(*clip);
                        
                        g.setFont(juce::FontOptions(12.0f, juce::Font::bold));
                        g.drawText(clipName,
                                   (int)textX, (int)(bounds.getY() + 5.0f), (int)availableTextWidth, 16,
                                   juce::Justification::left);
                        
                        g.setFont(juce::FontOptions(10.0f));
                        g.drawText(timeInfo,
                                   (int)textX, (int)(bounds.getY() + 22.0f), (int)availableTextWidth, 14,
                                   juce::Justification::left);
                    }
                    
                    // Menu button only for current timeline, selected clips, and valid timelines
                    if (isCurrentTimeline && isSelected && !isDisabled)
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
            }
            
            // Draw track separator
            juce::Colour separatorColour = juce::Colours::black.withAlpha(isDisabled ? 0.1f : 0.5f);
            g.setColour(separatorColour);
            g.drawLine(0, (int)(trackY + trackHeight), (int)totalWidth, (int)(trackY + trackHeight), 1.0f);
        }
        
        // Draw timeline separator
        if (timelineIndex < timelines->size() - 1)
        {
            juce::Colour separatorColour = juce::Colours::white.withAlpha(isDisabled ? 0.1f : 0.3f);
            g.setColour(separatorColour);
            g.drawLine(0, timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing / 2,
                      totalWidth, timelineY + timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing / 2, 2.0f);
        }
        
        // =====================================================================
        // Draw diagonal pattern for invalid timelines (after clips)
        // =====================================================================
        if (isDisabled)
        {
            // Calculate the full height of this timeline
            float timelineTotalHeight = timelineHeaderHeight;
            for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
            {
                timelineTotalHeight += trackHeight;
            }
            
            // Add spacing if this isn't the last timeline
            if (timelineIndex < timelines->size() - 1)
            {
                timelineTotalHeight += timelineSpacing;
            }
            
            // Draw diagonal pattern over the entire timeline area
            g.setColour(juce::Colour(0xaa800000)); // Dark red with transparency
            
            const float patternSpacing = 8.0f; // Space between diagonal lines
            const float patternWidth = patternSpacing * 2.0f;
            
            // Create a clipping region for this specific timeline
            g.saveState();
            g.reduceClipRegion(juce::Rectangle<int>(0, (int)timelineY, (int)totalWidth, (int)timelineTotalHeight));
            
            // Draw diagonal lines across the entire width
            float startX = -patternWidth; // Start slightly offscreen to ensure full coverage
            while (startX < totalWidth + patternWidth)
            {
                // Draw diagonal line from top-left to bottom-right
                juce::Line<float> diagonalLine(
                    startX, timelineY,
                    startX + patternWidth, timelineY + timelineTotalHeight
                );
                g.drawLine(diagonalLine, 1.5f);
                
                startX += patternSpacing;
            }
            
            g.restoreState();
        }
    }
    
    // =========================================================================
    // GLOBAL ELEMENTS (cursors, playhead, selection rectangle)
    // =========================================================================
    
    // Draw selection rectangle
    if (dragState.isRectangleSelecting && !dragState.selectionRectangle.isEmpty())
    {
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.fillRect(dragState.selectionRectangle);
        
        g.setColour(juce::Colours::white);
        g.drawRect(dragState.selectionRectangle, 1.0f);
    }
    
    // Draw placement cursor (fixed, after clicking)
    if (cursorVisible && cursorPosition >= visibleStartTime && cursorPosition <= visibleEndTime)
    {
        const float cursorX = timeToX(cursorPosition);
        g.setColour(juce::Colours::cyan);
        g.drawLine(cursorX, headerHeight, cursorX, totalHeight, 2.0f);
        
        juce::Path cursorHead;
        cursorHead.addTriangle(cursorX - 6, headerHeight,
                              cursorX + 6, headerHeight,
                              cursorX, headerHeight + 12);
        g.fillPath(cursorHead);
    }
    
    // Draw preview cursor (greyish, follows mouse)
    if (!autoFollow && previewCursorVisible && previewCursorPosition >= visibleStartTime && previewCursorPosition <= visibleEndTime)
    {
        const float previewX = timeToX(previewCursorPosition);
        g.setColour(juce::Colours::lightgrey.withAlpha(0.7f));
        g.drawLine(previewX, headerHeight, previewX, totalHeight, 1.0f);
        
        juce::Path previewHead;
        previewHead.addTriangle(previewX - 4, headerHeight,
                               previewX + 4, headerHeight,
                               previewX, headerHeight + 8);
        g.fillPath(previewHead);
    }
    
    // Draw playhead
    if (!autoFollow && playheadPosition >= visibleStartTime && playheadPosition <= visibleEndTime)
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
    
    horizontalScrollBar->setBounds(0, totalHeight - scrollBarSize, totalWidth, scrollBarSize);
    
    updateScrollBars();
}

void TimelineComponent::mouseDown(const juce::MouseEvent& event)
{
    // Reset cursor to normal when starting interaction
    setMouseCursor(juce::MouseCursor::NormalCursor);
    
    if (timelines == nullptr || timelines->size() == 0) return;

    const auto pos = event.getPosition();
    
    // Check if we're starting a rectangle selection (Ctrl/Cmd + drag)
    if ((event.mods.isCommandDown() || event.mods.isCtrlDown()) && !event.mods.isShiftDown())
    {
        dragState.isRectangleSelecting = true;
        dragState.dragStartPosition = pos;
        dragState.selectionRectangle.setPosition(pos.x, pos.y);
        dragState.selectionRectangle.setSize(0, 0);
        // Hide preview during drag operations
        hidePreviewCursor();
        return;
    }
    
    // Check if click is on a timeline header
    int clickedTimelineIndex = -1;
    float currentY = headerHeight;
    
    for (int i = 0; i < timelines->size(); ++i)
    {
        if (auto* timeline = timelines->getUnchecked(i))
        {
            float headerBottom = currentY + timelineHeaderHeight;
            
            // Check if click is within this timeline's header area
            if (pos.y >= currentY && pos.y < headerBottom)
            {
                clickedTimelineIndex = i;
                break;
            }
            
            currentY += timelineHeaderHeight + timeline->getNumLayers() * trackHeight + timelineSpacing;
        }
    }
    
    // If header was clicked, check if it's valid before switching
    if (clickedTimelineIndex != -1)
    {
        // Check if timeline is invalid but has clips (disabled)
        bool isDisabled = false;
        if (pSourceSet != nullptr)
        {
            int groupCount = pSourceSet->activeGroupCount();
            bool isValid = (clickedTimelineIndex < groupCount);
            bool hasClips = false;
            
            if (clickedTimelineIndex < timelines->size())
            {
                auto* timeline = timelines->getUnchecked(clickedTimelineIndex);
                if (timeline != nullptr)
                {
                    hasClips = (timeline->movement.clips.size() > 0) ||
                              (timeline->actions.clips.size() > 0);
                }
            }
            
            isDisabled = !isValid && hasClips;
        }
        
        if (!isDisabled)
        {
            setCurrentTimeline(clickedTimelineIndex);
            
            // Set cursor at click position in the timeline
            ms_t clickTime = xToTime(static_cast<float>(pos.x));
            setCursorTime(clickTime);
            
            repaint();
        }
        // If disabled, don't allow selection but still set cursor time
        else
        {
            ms_t clickTime = xToTime(static_cast<float>(pos.x));
            setCursorTime(clickTime);
            repaint();
        }
        return;
    }
    
    // Check if click is on a clip
    auto clipBounds = findClipAtPosition(pos);
    
    if (clipBounds.timelineIndex != -1)
    {
        // Check if the timeline containing this clip is disabled
        bool isTimelineDisabled = false;
        if (pSourceSet != nullptr)
        {
            int groupCount = pSourceSet->activeGroupCount();
            bool isValid = (clipBounds.timelineIndex < groupCount);
            bool hasClips = true; // We're clicking on a clip, so it has clips
            
            isTimelineDisabled = !isValid && hasClips;
        }
        
        // Prevent interaction with clips on disabled timelines
        if (isTimelineDisabled)
        {
            // Just set cursor time but don't allow selection or dragging
            ms_t clickTime = xToTime(static_cast<float>(pos.x));
            setCursorTime(clickTime);
            repaint();
            return;
        }
        
        // Normal clip interaction for valid timelines
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
            // Check if the clicked clip is already selected
            bool clipAlreadySelected = isClipSelected(clipBounds.timelineIndex, clipBounds.layerIndex,
                                                     clipBounds.clipIndex, clipBounds.isMovementClip);
            
            if (!clipAlreadySelected)
            {
                // Single selection - clear others only if this clip isn't already selected
                selectClip(clipBounds.timelineIndex, clipBounds.layerIndex,
                          clipBounds.clipIndex, clipBounds.isMovementClip, false);
            }
            // If clip is already selected, keep the multi-selection and drag all
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
            auto iconArea = getIconBoundsWithinClip(bounds);
            
            if (iconArea.contains(pos.toFloat()))
            {
                showClipEditor(clipBounds.timelineIndex,
                              clipBounds.clipIndex, clipBounds.isMovementClip);
                dragState.isDragging = false;
                return;
            }
            
            if (buttonArea.contains(pos.toFloat()))
            {
                showClipContextMenu(clipBounds.timelineIndex, clipBounds.layerIndex,
                                   clipBounds.clipIndex, clipBounds.isMovementClip, pos);
                dragState.isDragging = false;
            }
        }

        // Switch to clicked timeline (if it's valid)
        if (!isTimelineDisabled)
        {
            setCurrentTimeline(clipBounds.timelineIndex);
        }
        
        repaint();
    }
    else
    {
        // Set fixed cursor at click position and hide preview
        ms_t clickTime = xToTime(static_cast<float>(event.getPosition().x));
        setCursorTime(clickTime);
        
        // Clicked on empty space - clear selection unless shift/ctrl is held
        if (!event.mods.isAnyModifierKeyDown())
        {
            clearSelection();
            repaint();
        }
    }
}

void TimelineComponent::mouseEnter(const juce::MouseEvent& event)
{
    // Show preview cursor when mouse enters component (if not over clip)
    if (findClipAtPosition(event.getPosition()).timelineIndex == -1)
    {
        updatePreviewCursor(event.getPosition());
    }
}

void TimelineComponent::mouseExit(const juce::MouseEvent& event)
{
    // Hide preview cursor when mouse leaves component
    hidePreviewCursor();
    setMouseCursor(juce::MouseCursor::NormalCursor);
    
    // Clear status message
    if (statusMessageFunction)
    {
        juce::AttributedString message;
        message.append("Ready",
                      juce::FontOptions(12.0f),
                      juce::Colours::lightgrey);
        statusMessageFunction(message);
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
        
    // Optionally update cursor during resize operations for visual feedback
    if (dragState.isResizing)
    {
        updatePreviewCursor(currentPos);
    }
    
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
    
    showPreviewCursor(true);
    
    repaint();
}

void TimelineComponent::mouseMove(const juce::MouseEvent& event)
{
    if (timelines == nullptr || timelines->size() == 0)
    {
        hidePreviewCursor();
        return;
    }
    
    const auto pos = event.getPosition();
    
    // Check if mouse is in the row header section (left side with trackHeaderWidth)
    bool isInRowHeaderSection = pos.x < trackHeaderWidth;
    
    if (isInRowHeaderSection)
    {
        // Mouse is in row header section - hide preview cursor and don't show status messages
        hidePreviewCursor();
        setMouseCursor(juce::MouseCursor::NormalCursor);
        
        // Clear status message when in header area
        if (statusMessageFunction)
        {
            juce::AttributedString message;
            message.append("Ready",
                          juce::FontOptions(12.0f),
                          juce::Colours::lightgrey);
            statusMessageFunction(message);
        }
        return;
    }
    
    // Check if we're over a clip
    auto clipBounds = findClipAtPosition(pos);
    
    if (clipBounds.timelineIndex != -1)
    {
        // Check if timeline is disabled
        bool isTimelineDisabled = false;
        if (pSourceSet != nullptr)
        {
            int groupCount = pSourceSet->activeGroupCount();
            bool isValid = (clipBounds.timelineIndex < groupCount);
            bool hasClips = true; // We're over a clip
            
            isTimelineDisabled = !isValid && hasClips;
        }
        
        // Mouse is over a clip - hide preview cursor and change cursor
        hidePreviewCursor();
        
        if (isTimelineDisabled)
        {
            setMouseCursor(juce::MouseCursor::NormalCursor);
        }
        else if (clipBounds.isResizeLeft || clipBounds.isResizeRight)
        {
            setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
        }
        else
        {
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
        }
        
        // Send clip information to status bar
        if (statusMessageFunction)
        {
            juce::AttributedString message;
            
            // Clip name in bold
            message.append(clipBounds.displayName,
                          juce::FontOptions(12.0f, juce::Font::bold),
                          getClipColourFromTimeline(clipBounds.timelineIndex));
            
            // Time info
            message.append(" - " + clipBounds.timeInfo,
                          juce::FontOptions(12.0f),
                          juce::Colours::lightgrey);
            
            if (isTimelineDisabled)
            {
                message.append(" (Timeline Invalid - No Interaction)",
                              juce::FontOptions(11.0f),
                              juce::Colours::red);
            }
            else if (clipBounds.isResizeLeft)
            {
                message.append(" (Drag left edge to resize)",
                              juce::FontOptions(11.0f),
                              juce::Colours::yellow);
            }
            else if (clipBounds.isResizeRight)
            {
                message.append(" (Drag right edge to resize)",
                              juce::FontOptions(11.0f),
                              juce::Colours::yellow);
            }
            else
            {
                message.append(" (Drag to move)",
                              juce::FontOptions(11.0f),
                              juce::Colours::lightblue);
            }
            
            statusMessageFunction(message);
        }
    }
    else
    {
        // Mouse is not over a clip - show preview cursor and time position
        setMouseCursor(juce::MouseCursor::NormalCursor);
        updatePreviewCursor(pos);
        
        // Send time position to status bar
        if (statusMessageFunction)
        {
            ms_t currentTime = xToTime(static_cast<float>(pos.x));
            
            juce::AttributedString message;
            message.append("Position: ",
                          juce::FontOptions(12.0f),
                          juce::Colours::lightgrey);
            
            message.append(formatTimeWithSeparators(currentTime) + " ms",
                          juce::FontOptions(12.0f, juce::Font::bold),
                          juce::Colours::white);
            
            statusMessageFunction(message);
        }
    }
}

void TimelineComponent::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)
{
    if (event.mods.isAltDown())
    {
        zoom(wheel.deltaY > 0 ? 1.05f : (1.0f/1.05f));
    }
    else
    {
        horizontalScrollBar->mouseWheelMove(event, wheel);
    }
    
    if(!approximatelyEqual(wheel.deltaY, 0.0f) && !event.mods.isAnyModifierKeyDown())
    {
        // Vertical scrolling - forward to parent viewport
        if (auto* viewport = findParentComponentOfClass<juce::Viewport>())
        {
            viewport->mouseWheelMove(event.getEventRelativeTo(viewport), wheel);
        }
    }
}

void TimelineComponent::timelineSelectionChanged()
{
//    currentTimelineIndex = timelineSelector->getSelectedId() - 1;
    
    // Sync to PointSelection
    syncTimelineSelectionToPointSelection();
        
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
    // Duplicate selected clips with Ctrl/Cmd + D
    if (key.getKeyCode() == 'D' && (key.getModifiers().isCommandDown() || key.getModifiers().isCtrlDown()))
    {
        if (!selectedClips.isEmpty())
        {
            // Store current selection since it will change during duplication
            auto currentSelection = selectedClips;
            
            // Clear selection first
            clearSelection();
            
            // Duplicate all selected clips
            for (const auto& selected : currentSelection)
            {
                duplicateClip(selected.timelineIndex, selected.layerIndex,
                             selected.clipIndex, selected.isMovementClip);
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
        horizontalScrollBar->setSingleStepSize((visibleEndTime - visibleStartTime)*0.01);
    }
}

juce::String TimelineComponent::generateClipFullInfo(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip, const Clip& clip) const
{
    juce::String info;
    
    // Basic information
    info << "Group " << (timelineIndex + 1) << " - ";
    info << (layerIndex == 0 ? "Movement" : "Action") << " Clip\n";
    info << "Name: " << (clip.id.isNotEmpty() ? clip.id : "Unnamed") << "\n";
    
    // Time information with formatted numbers
    info << "Start: " << formatTimeWithSeparators(clip.start) << " ms\n";
    info << "End: " << formatTimeWithSeparators(clip.end()) << " ms\n";
    info << "Duration: " << formatTimeWithSeparators(clip.length) << " ms\n";
    
    // Additional clip-specific information
    if (isMovementClip)
    {
        const auto& movementClip = static_cast<const MovementClip&>(clip);
        // Add movement-specific info if needed
    }
    else
    {
        const auto& actionClip = static_cast<const ActionClip&>(clip);
        info << "Actions: " << actionClip.actions.size() << "\n";
    }
    
    return info;
}

juce::String TimelineComponent::formatTimeWithSeparators(ms_t timeMs) const
{
    juce::String timeStr = juce::String(timeMs);
    juce::String result;
    
    // Add thousands separators for readability
    int length = timeStr.length();
    for (int i = 0; i < length; ++i)
    {
        if (i > 0 && (length - i) % 3 == 0)
            result << ",";
        result << timeStr[i];
    }
    
    return result;
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

    if (timelines == nullptr) return clipsAtPos;

    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            bool isMovementClip = (layerIndex == 0);
            int numClips = isMovementClip ? timeline->movement.clips.size() : timeline->actions.clips.size();
            
            for (int clipIndex = 0; clipIndex < numClips; ++clipIndex)
            {
                const Clip* clip = isMovementClip ?
                    static_cast<const Clip*>(&timeline->movement.clips.getReference(clipIndex)) :
                    static_cast<const Clip*>(&timeline->actions.clips.getReference(clipIndex));
                
                if (!clip) continue;
                
                // Check if clip is visible
                if (clip->end() < visibleStartTime || clip->start > visibleEndTime)
                    continue;
                
                // Calculate clip bounds
                ms_t visibleClipStart = juce::jmax(clip->start, visibleStartTime);
                ms_t visibleClipEnd = juce::jmin(clip->end(), visibleEndTime);
                ms_t visibleClipLength = visibleClipEnd - visibleClipStart;
                
                if (visibleClipLength <= 0) continue;
                
                const float x = timeToX(visibleClipStart);
                const float width = visibleClipLength * pixelsPerMillisecond;
                const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                juce::Rectangle<float> bounds(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                // Only add if clip is vertically visible
                if (bounds.getBottom() >= 0 && bounds.getY() <= getHeight() &&
                    bounds.contains(position.toFloat()))
                {
                    ClipBounds clipBounds;
                    clipBounds.timelineIndex = timelineIndex;
                    clipBounds.layerIndex = layerIndex;
                    clipBounds.clipIndex = clipIndex;
                    clipBounds.isMovementClip = isMovementClip;
                    clipBounds.bounds = bounds;
                    
                    // Populate clip information
                    clipBounds.displayName = getClipDisplayName(timelineIndex, layerIndex, clipIndex, isMovementClip);
                    clipBounds.timeInfo = getClipTimeInfo(*clip);
                    
                    clipsAtPos.add(clipBounds);
                }
            }
        }
    }
    
    return clipsAtPos;
}

TimelineComponent::ClipBounds TimelineComponent::findMostHiddenClip(const juce::Array<ClipBounds>& clips, const juce::Point<int>& /*position*/) const
{
    if (clips.size() == 1)
        return clips.getFirst();
    
    // Sort clips by selection status and coverage
    struct ClipCoverage
    {
        ClipBounds bounds;
        int coverageCount = 0;
        bool isSelected = false;
        int priorityScore = 0; // Higher score = higher priority
    };
    
    juce::Array<ClipCoverage> coverage;
    
    for (const auto& clip : clips)
    {
        ClipCoverage item;
        item.bounds = clip;
        item.isSelected = isClipSelected(clip.timelineIndex, clip.layerIndex,
                                       clip.clipIndex, clip.isMovementClip);
        
        // Count how many other clips cover this one
        for (const auto& other : clips)
        {
            if (&clip != &other && other.bounds.contains(clip.bounds.getCentre()))
            {
                item.coverageCount++;
            }
        }
        
        // Calculate priority score: selected clips get high priority
        // Selected clips: 1000 + coverage count (so most hidden selected clips are prioritized)
        // Non-selected clips: coverage count only
        item.priorityScore = item.isSelected ? (1000 + item.coverageCount) : item.coverageCount;
        
        coverage.add(item);
    }
    
    // Find the clip with the highest priority score
    int maxPriorityScore = -1;
    ClipBounds mostHidden = clips.getFirst();
    
    for (const auto& item : coverage)
    {
        if (item.priorityScore > maxPriorityScore)
        {
            maxPriorityScore = item.priorityScore;
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
    
    if (timelines == nullptr) return;

    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            bool isMovementClip = (layerIndex == 0);
            int numClips = isMovementClip ? timeline->movement.clips.size() : timeline->actions.clips.size();
            
            for (int clipIndex = 0; clipIndex < numClips; ++clipIndex)
            {
                const Clip* clip = isMovementClip ?
                    static_cast<const Clip*>(&timeline->movement.clips.getReference(clipIndex)) :
                    static_cast<const Clip*>(&timeline->actions.clips.getReference(clipIndex));
                
                if (!clip) continue;
                
                // Check if clip is visible
                if (clip->end() < visibleStartTime || clip->start > visibleEndTime)
                    continue;
                
                // Calculate clip bounds
                ms_t visibleClipStart = juce::jmax(clip->start, visibleStartTime);
                ms_t visibleClipEnd = juce::jmin(clip->end(), visibleEndTime);
                ms_t visibleClipLength = visibleClipEnd - visibleClipStart;
                
                if (visibleClipLength <= 0) continue;
                
                const float x = timeToX(visibleClipStart);
                const float width = visibleClipLength * pixelsPerMillisecond;
                const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                juce::Rectangle<float> bounds(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                // Only select if clip is vertically visible and intersects rectangle
                if (bounds.getBottom() >= 0 && bounds.getY() <= getHeight() &&
                    rect.intersects(bounds.toNearestInt()))
                {
                    selectClip(timelineIndex, layerIndex, clipIndex, isMovementClip, true);
                }
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
    result.isResizeLeft = false;
    result.isResizeRight = false;

    if (timelines == nullptr) return result;

    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = timelines->getUnchecked(timelineIndex);
        if (timeline == nullptr) continue;
        
        const float timelineY = timelineIndexToY(timelineIndex);
        
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            bool isMovementClip = (layerIndex == 0);
            int numClips = isMovementClip ? timeline->movement.clips.size() : timeline->actions.clips.size();
            
            for (int clipIndex = 0; clipIndex < numClips; ++clipIndex)
            {
                const Clip* clip = isMovementClip ?
                    static_cast<const Clip*>(&timeline->movement.clips.getReference(clipIndex)) :
                    static_cast<const Clip*>(&timeline->actions.clips.getReference(clipIndex));
                
                if (!clip) continue;
                
                // Check if clip is visible
                if (clip->end() < visibleStartTime || clip->start > visibleEndTime)
                    continue;
                
                // Calculate clip bounds
                ms_t visibleClipStart = juce::jmax(clip->start, visibleStartTime);
                ms_t visibleClipEnd = juce::jmin(clip->end(), visibleEndTime);
                ms_t visibleClipLength = visibleClipEnd - visibleClipStart;
                
                if (visibleClipLength <= 0) continue;
                
                const float x = timeToX(visibleClipStart);
                const float width = visibleClipLength * pixelsPerMillisecond;
                const float trackY = timelineY + timelineHeaderHeight + layerIndexToY(layerIndex);
                const float clipY = trackY + (trackHeight - clipHeight) * 0.5f;
                
                juce::Rectangle<float> bounds(x, clipY, juce::jmax(10.0f, width), clipHeight);
                
                if (bounds.contains(position.toFloat()))
                {
                    result.timelineIndex = timelineIndex;
                    result.layerIndex = layerIndex;
                    result.clipIndex = clipIndex;
                    result.isMovementClip = isMovementClip;
                    result.bounds = bounds;
                    
                    // Populate clip information
                    result.displayName = getClipDisplayName(timelineIndex, layerIndex, clipIndex, isMovementClip);
                    result.timeInfo = getClipTimeInfo(*clip);
                    
                    // Check resize handles (only for current timeline and selected clips)
                    if (timelineIndex == currentTimelineIndex &&
                        isClipSelected(timelineIndex, layerIndex, clipIndex, isMovementClip))
                    {
                        const auto posX = static_cast<float>(position.x);
                        
                        // Check left resize handle
                        if (posX >= bounds.getX() && posX <= bounds.getX() + resizeHandleWidth)
                        {
                            result.isResizeLeft = true;
                        }
                        // Check right resize handle
                        else if (posX >= bounds.getRight() - resizeHandleWidth && posX <= bounds.getRight())
                        {
                            result.isResizeRight = true;
                        }
                    }
                    
                    return result; // Return first found clip
                }
            }
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
                       .withParentComponent(this),
                       [this, timelineIndex, layerIndex, clipIndex, isMovementClip, position](int result)
                       {
                           if (result == 1)
                           {
                               showClipEditor(timelineIndex, clipIndex, isMovementClip);
                           }
                           else if (result == 2)
                           {
                               removeClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
                               repaint();
                           }
                           else if (result == 3)
                           {
                               duplicateClip(timelineIndex, layerIndex, clipIndex, isMovementClip);
                               repaint();
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
    
    clipEditorManager->closeAllWindows();
    
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

void TimelineComponent::renderHeaderToCache()
{
    const int headerAreaHeight = static_cast<int>(headerHeight);
    
    // Check if cache is still valid
    if (headerCache &&
        cachedHeaderVisibleStartTime == visibleStartTime &&
        cachedHeaderVisibleEndTime == visibleEndTime &&
        cachedHeaderPixelsPerMillisecond == pixelsPerMillisecond &&
        cachedHeaderWidth == getWidth() &&
        cachedHeaderHeight == headerAreaHeight)
    {
        return; // Cache is still valid
    }
    
    // Update cache dimensions
    cachedHeaderVisibleStartTime = visibleStartTime;
    cachedHeaderVisibleEndTime = visibleEndTime;
    cachedHeaderPixelsPerMillisecond = pixelsPerMillisecond;
    cachedHeaderWidth = getWidth();
    cachedHeaderHeight = headerAreaHeight;
    
    // Create or update the cache image
    if (!headerCache || headerCache->getWidth() != cachedHeaderWidth || headerCache->getHeight() != cachedHeaderHeight)
    {
        headerCache = std::make_unique<juce::Image>(juce::Image::ARGB, cachedHeaderWidth, cachedHeaderHeight, true);
    }
    
    // Clear the cache with header background color
    juce::Graphics g(*headerCache);
    g.fillAll(juce::Colour(0xff252526)); // Header background color
    
    const float totalWidth = static_cast<float>(cachedHeaderWidth);
    const float timelineX = trackHeaderWidth;
    
    // Calculate visible duration
    const float visibleDuration = visibleEndTime - visibleStartTime;
    
    // Determine if we should use milliseconds or seconds
    const bool useMilliseconds = visibleDuration < 2000; // Switch to ms when full span < 2s
    
    // Calculate optimal time step to get approximately 7-8 labels
    const float targetNumberOfLabels = 7.5f; // Increased for more labels
    float baseTimeStep = visibleDuration / targetNumberOfLabels;
    
    // Round to a nice human-readable time step
    ms_t timeStep;
    if (useMilliseconds) {
        // Millisecond steps for short durations
        if (baseTimeStep <= 50) {
            timeStep = 50; // 50ms steps
        } else if (baseTimeStep <= 100) {
            timeStep = 100; // 100ms steps
        } else if (baseTimeStep <= 200) {
            timeStep = 200; // 200ms steps
        } else if (baseTimeStep <= 250) {
            timeStep = 250; // 250ms steps
        } else if (baseTimeStep <= 500) {
            timeStep = 500; // 500ms steps
        } else {
            timeStep = 1000; // 1 second steps
        }
    } else {
        // Second steps for longer durations
        if (baseTimeStep <= 500) {
            timeStep = 500; // 0.5 second steps
        } else if (baseTimeStep <= 1000) {
            timeStep = 1000; // 1 second steps
        } else if (baseTimeStep <= 2000) {
            timeStep = 2000; // 2 second steps
        } else if (baseTimeStep <= 5000) {
            timeStep = 5000; // 5 second steps
        } else if (baseTimeStep <= 10000) {
            timeStep = 10000; // 10 second steps
        } else {
            timeStep = 15000; // 15 second steps (instead of 30s)
        }
    }
    
    // Draw minor ticks in header (smaller marks between the main marks)
    ms_t minorTimeStep = timeStep / (useMilliseconds ? 2 : 4);
    ms_t currentMinorTime = (visibleStartTime / minorTimeStep) * minorTimeStep;
    
    g.setColour(juce::Colours::lightgrey.withAlpha(0.2f));
    while (currentMinorTime <= visibleEndTime) {
        // Skip positions that would have major ticks
        if (currentMinorTime % timeStep != 0) {
            const float x = timeToX(currentMinorTime);
            if (x >= timelineX && x <= totalWidth) {
                // Draw minor tick in header only (shorter line)
                g.drawVerticalLine(static_cast<int>(x), headerHeight - 8, headerHeight);
            }
        }
        currentMinorTime += minorTimeStep;
    }
    
    // Draw major ticks with labels
    ms_t currentTime = (visibleStartTime / timeStep) * timeStep;
    
    g.setColour(juce::Colours::lightgrey.withAlpha(0.3f));
    g.setFont(juce::FontOptions(12.0f));
    
    while (currentTime <= visibleEndTime) {
        const float x = timeToX(currentTime);
        if (x >= timelineX && x <= totalWidth) {
            // Draw major tick line in header only
            g.drawVerticalLine(static_cast<int>(x), 0, headerHeight);
            
            // Format time text
            juce::String timeText;
            if (useMilliseconds) {
                // Always show milliseconds when full span < 2s
                timeText = juce::String(currentTime) + "ms";
            } else {
                // Show seconds for longer durations
                double seconds = currentTime / 1000.0;
                
                if (timeStep >= 2000 || seconds >= 5) {
                    // For 2s+ steps or times >= 5s, show integer seconds
                    timeText = juce::String(static_cast<int>(seconds)) + "s";
                } else {
                    // For smaller steps and times < 5s, show one decimal
                    timeText = juce::String(seconds, 1) + "s";
                    // Remove trailing .0 but keep .1, .2, etc.
                    if (timeText.endsWith(".0s")) {
                        timeText = juce::String(static_cast<int>(seconds)) + "s";
                    }
                }
            }
            
            // Draw time text
            g.setColour(juce::Colours::lightgrey);
            g.drawText(timeText,
                      static_cast<int>(x) + 2, 2, 60, (int)(headerHeight - 4),
                      juce::Justification::left);
        }
        currentTime += timeStep;
    }
}

void TimelineComponent::drawHeader(juce::Graphics& g)
{
    // Update cache if needed
    renderHeaderToCache();
    
    // Draw the cached header
    if (headerCache)
    {
        g.drawImageAt(*headerCache, 0, 0);
    }
    
    // Draw major lines through entire timeline (always painted fresh)
    const float totalWidth = static_cast<float>(getWidth());
    const float totalHeight = static_cast<float>(getHeight());
    const float timelineX = trackHeaderWidth;
    
    // Calculate visible duration
    const float visibleDuration = visibleEndTime - visibleStartTime;
    
    // Use the same logic as header cache to determine time step
    const bool useMilliseconds = visibleDuration < 2000;
    const float targetNumberOfLabels = 7.5f; // Match the cache
    float baseTimeStep = visibleDuration / targetNumberOfLabels;
    
    // Round to the same human-readable time step
    ms_t timeStep;
    if (useMilliseconds) {
        if (baseTimeStep <= 50) {
            timeStep = 50;
        } else if (baseTimeStep <= 100) {
            timeStep = 100;
        } else if (baseTimeStep <= 200) {
            timeStep = 200;
        } else if (baseTimeStep <= 250) {
            timeStep = 250;
        } else if (baseTimeStep <= 500) {
            timeStep = 500;
        } else {
            timeStep = 1000;
        }
    } else {
        if (baseTimeStep <= 500) {
            timeStep = 500;
        } else if (baseTimeStep <= 1000) {
            timeStep = 1000;
        } else if (baseTimeStep <= 2000) {
            timeStep = 2000;
        } else if (baseTimeStep <= 5000) {
            timeStep = 5000;
        } else if (baseTimeStep <= 10000) {
            timeStep = 10000;
        } else {
            timeStep = 15000;
        }
    }
    
    // Draw major lines through entire timeline
    g.setColour(juce::Colours::lightgrey.withAlpha(0.3f));
    ms_t currentTime = (visibleStartTime / timeStep) * timeStep;
    
    while (currentTime <= visibleEndTime) {
        const float x = timeToX(currentTime);
        if (x >= timelineX && x <= totalWidth) {
            // Draw major line through entire height (below header)
            g.drawVerticalLine(static_cast<int>(x), headerHeight, totalHeight);
        }
        currentTime += timeStep;
    }
}

void TimelineComponent::setCursorPosition(ms_t time)
{
    cursorPosition = juce::jmax<ms_t>(0, time);
    repaint();
}

void TimelineComponent::showCursor(bool shouldShow)
{
    if (cursorVisible != shouldShow)
    {
        cursorVisible = shouldShow;
        repaint();
    }
}

void TimelineComponent::setPreviewCursorPosition(ms_t time)
{
    previewCursorPosition = juce::jmax<ms_t>(0, time);
    repaint();
}

void TimelineComponent::showPreviewCursor(bool shouldShow)
{
    if (previewCursorVisible != shouldShow)
    {
        previewCursorVisible = shouldShow;
        repaint();
    }
}

void TimelineComponent::setCursorTime(ms_t time)
{
    setCursorPosition(time);
    showCursor(true);
    // Hide preview when setting fixed cursor
    showPreviewCursor(false);
}

ms_t TimelineComponent::getCursorTime() const
{
    return cursorPosition;
}

void TimelineComponent::updatePreviewCursor(const juce::Point<int>& mousePos)
{
    ms_t time = xToTime(static_cast<float>(mousePos.x));
    setPreviewCursorPosition(time);
    showPreviewCursor(true);
}

void TimelineComponent::hidePreviewCursor()
{
    showPreviewCursor(false);
}


bool TimelineComponent::shouldShowClipText(const juce::Rectangle<float>& clipBounds, float iconSize) const
{
    const float minWidthForText = iconSize * 2.5f; // 2.5x icon size as threshold
    return clipBounds.getWidth() >= minWidthForText;
}

bool TimelineComponent::duplicateClip(int timelineIndex, int layerIndex, int clipIndex, bool isMovementClip)
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
            const auto& originalClip = timeline->movement.clips.getReference(clipIndex);
            
            // Create a duplicate clip
            MovementClip newClip = originalClip;
            
            // Place at cursor position if available, otherwise after original
            if (cursorVisible)
            {
                newClip.start = cursorPosition;
            }
            else
            {
                newClip.start = originalClip.start + originalClip.length + 100; // 100ms gap after original
            }
            
            newClip.id = generateDuplicateClipId(originalClip.id);
            
            timeline->movement.clips.add(newClip);
            success = true;
            
            // Select the new duplicated clip
            selectClip(timelineIndex, layerIndex, timeline->movement.clips.size() - 1, true, false);
        }
    }
    else if (!isMovementClip && layerIndex == 1)
    {
        if (clipIndex >= 0 && clipIndex < timeline->actions.clips.size())
        {
            const auto& originalClip = timeline->actions.clips.getReference(clipIndex);
            
            // Create a duplicate clip
            ActionClip newClip = originalClip;
            
            // Place at cursor position if available, otherwise after original
            if (cursorVisible)
            {
                newClip.start = cursorPosition;
            }
            else
            {
                newClip.start = originalClip.start + originalClip.length + 100; // 100ms gap after original
            }
            
            newClip.id = generateDuplicateClipId(originalClip.id);
            
            timeline->actions.clips.add(newClip);
            success = true;
            
            // Select the new duplicated clip
            selectClip(timelineIndex, layerIndex, timeline->actions.clips.size() - 1, false, false);
        }
    }
    
    if (success)
    {
        // Update max duration if needed
        if (auto* clip = getClip(timelineIndex, layerIndex,
                                isMovementClip ? timeline->movement.clips.size() - 1 : timeline->actions.clips.size() - 1,
                                isMovementClip))
        {
            maxDuration = juce::jmax(maxDuration, clip->end() + 5000);
        }
        
        repaint();
    }
    
    return success;
}

juce::String TimelineComponent::generateDuplicateClipId(const juce::String& originalId)
{
    if (originalId.isEmpty())
        return "Copy";
    
    // Check if the ID already ends with "Copy" or "Copy (n)"
    if (originalId.endsWith("Copy"))
    {
        // Check for pattern "Copy (n)"
        int openParen = originalId.lastIndexOfChar('(');
        int closeParen = originalId.lastIndexOfChar(')');
        
        if (openParen != -1 && closeParen != -1 && closeParen > openParen + 1)
        {
            juce::String numberStr = originalId.substring(openParen + 1, closeParen);
            int copyNumber = numberStr.getIntValue();
            
            if (copyNumber > 0)
            {
                // Increment the copy number
                juce::String baseName = originalId.substring(0, openParen - 1); // Remove " (n)"
                return baseName + " (" + juce::String(copyNumber + 1) + ")";
            }
        }
        
        // Just "Copy" -> make it "Copy 2"
        return originalId + " 2";
    }
    
    // Add "Copy" to the original ID
    return originalId + " Copy";
}

void TimelineComponent::showClipEditor(int timelineIndex, int clipIndex, bool isMovementClip)
{
    if (isMovementClip)
    {
        clipEditorManager->showMovementEditor(this, timelineIndex, clipIndex, this);
    }
    else
    {
        clipEditorManager->showActionEditor(this, timelineIndex, clipIndex, this);
    }
}

AmbiSourceSet* TimelineComponent::getSources() const
{
    return pSourceSet;
}

PointSelection* TimelineComponent::getPointSelection() const
{
    return pPointSelectionControl;
}

TimelineModel* TimelineComponent::getTimelineModel(int timelineIndex) const
{
    if (timelines == nullptr || timelineIndex < 0 || timelineIndex >= timelines->size())
        return nullptr;
    
    return timelines->getUnchecked(timelineIndex);
}

void TimelineComponent::syncTimelineSelectionToPointSelection()
{
    if (pPointSelectionControl == nullptr) return;
    
    // When timeline selection changes, update PointSelection
    if (timelines != nullptr && currentTimelineIndex >= 0 && currentTimelineIndex < timelines->size())
    {
        // Select the group corresponding to the current timeline
        // Assuming timeline index maps directly to group index
        pPointSelectionControl->selectGroup(currentTimelineIndex, false);
    }
    else
    {
        // No timeline selected, clear group selection
        pPointSelectionControl->unselectPoint();
    }
}

void TimelineComponent::syncPointSelectionToTimelineSelection()
{
    if (pPointSelectionControl == nullptr) return;
    
    // When PointSelection changes, update timeline selection
    if (pPointSelectionControl->getSelectionMode() == PointSelection::Group)
    {
        Array<int> selectedGroups = pPointSelectionControl->getSelectedIndices();
        if (!selectedGroups.isEmpty())
        {
            // Use the first selected group as the current timeline
            int groupIndex = selectedGroups.getFirst();
            if (groupIndex >= 0 && groupIndex < getNumTimelines())
            {
                setCurrentTimeline(groupIndex);
            }
        }
    }
}

void TimelineComponent::setStatusMessageFunction(std::function<void(const juce::AttributedString&)> function)
{
    statusMessageFunction = function;
}

juce::Colour TimelineComponent::getClipColourFromTimeline(int timelineIndex) const
{
    // Use the same colors as timeline headers for consistency
    return getTimelineColour(timelineIndex).brighter(0.3f);
}
