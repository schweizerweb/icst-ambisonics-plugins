#include "AnimatorMainView.h"

AnimatorMainView::AnimatorMainView()
{
    // Create menu bar model with pointer
    menuBarModel = std::make_unique<MainMenuBarModel>(this);
    menuBar = std::make_unique<juce::MenuBarComponent>(menuBarModel.get());
    
    // Create toolbar
    toolbar = std::make_unique<ToolbarComponent>(*this);
    
    // Create timeline component
    timelineComponent = std::make_unique<TimelineComponent>();
    
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(toolbar.get());
    addAndMakeVisible(timelineComponent.get());
}

AnimatorMainView::~AnimatorMainView()
{
    // Remove menu bar before destruction to prevent JUCE assertion
    if (menuBar != nullptr)
    {
        removeChildComponent(menuBar.get());
        menuBar.reset();
    }
    menuBarModel.reset();
}

void AnimatorMainView::setTimelines(juce::OwnedArray<TimelineModel>* newTimelines)
{
    timelines = newTimelines;
    timelineComponent->setTimelines(timelines);
    
    // Refresh menu bar to update the import/export submenus
    if (menuBarModel != nullptr)
    {
        menuBarModel->menuItemsChanged();
    }
}

void AnimatorMainView::setPlayheadPosition(ms_t timeMs)
{
    timelineComponent->setPlayheadPosition(timeMs);
}

void AnimatorMainView::setAutoFollow(bool shouldAutoFollow)
{
    autoFollowEnabled = shouldAutoFollow;
    timelineComponent->setAutoFollow(shouldAutoFollow);
}

void AnimatorMainView::setPlayheadProvider(std::function<PlayheadSnapshot()> provider)
{
    // Store if needed
}

void AnimatorMainView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void AnimatorMainView::resized()
{
    auto area = getLocalBounds();
    
    // Menu bar at top (24 pixels)
    menuBar->setBounds(area.removeFromTop(24));
    
    // Toolbar below menu (40 pixels)
    toolbar->setBounds(area.removeFromTop(40));
    
    // Rest goes to timeline component
    timelineComponent->setBounds(area);
}

// Menu implementation
juce::PopupMenu AnimatorMainView::MainMenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    
    switch (topLevelMenuIndex)
    {
        case 0: // File
            {
                // Import submenu
                juce::PopupMenu importSubMenu;
                importSubMenu.addItem(100, "Append as new timeline");
                
                // Add overwrite options for existing timelines
                if (owner->timelines != nullptr && !owner->timelines->isEmpty())
                {
                    importSubMenu.addSeparator();
                    for (int i = 0; i < owner->timelines->size(); ++i)
                    {
                        auto* timeline = owner->timelines->getUnchecked(i);
                        juce::String timelineName = "Timeline " + juce::String(i + 1);
                            
                        importSubMenu.addItem(101 + i, "Overwrite " + timelineName);
                    }
                }
                menu.addSubMenu("Import Scene...", importSubMenu);
                
                // Export submenu
                juce::PopupMenu exportSubMenu;
                if (owner->timelines != nullptr && !owner->timelines->isEmpty())
                {
                    for (int i = 0; i < owner->timelines->size(); ++i)
                    {
                        auto* timeline = owner->timelines->getUnchecked(i);
                        juce::String timelineName = "Group " + juce::String(i + 1);
                            
                        exportSubMenu.addItem(200 + i, timelineName);
                    }
                }
                else
                {
                    exportSubMenu.addItem(1, "No timelines available", false);
                }
                menu.addSubMenu("Export Scene...", exportSubMenu);
                
                menu.addSeparator();
                menu.addItem(6, "Preferences");
            }
            break;
            
        case 1: // Edit
            menu.addItem(10, "Undo");
            menu.addItem(11, "Redo");
            menu.addSeparator();
            menu.addItem(12, "Cut");
            menu.addItem(13, "Copy");
            menu.addItem(14, "Paste");
            menu.addItem(15, "Delete Selected Clips");
            menu.addSeparator();
            menu.addItem(16, "Select All");
            menu.addItem(17, "Deselect All");
            break;
            
        case 2: // View
            menu.addItem(20, "Zoom In");
            menu.addItem(21, "Zoom Out");
            menu.addItem(22, "Reset Zoom");
            menu.addSeparator();
            menu.addItem(23, "Toggle Auto-follow");
            menu.addItem(24, "Show/Hide Rulers");
            break;
            
        case 3: // Insert
            menu.addItem(30, "Add Movement Clip");
            menu.addItem(31, "Add Action Clip");
            break;
    }
    
    return menu;
}

void AnimatorMainView::MainMenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    if (owner != nullptr)
        owner->handleMenuAction(menuItemID);
}

void AnimatorMainView::handleMenuAction(int menuItemID)
{
    switch (menuItemID)
    {
        case 100: // Import - Append as new timeline
            importScene(-1); // -1 means append
            break;
            
        case 6: // Preferences
            // Handle preferences
            break;
            
        case 12: // Cut
            cutSelectedClips();
            break;
        case 13: // Copy
            copySelectedClips();
            break;
        case 14: // Paste
            pasteClips();
            break;
        case 15: // Delete Selected Clips
            deleteSelectedClips();
            break;
        case 16: // Select All
            selectAllClips();
            break;
        case 17: // Deselect All
            deselectAllClips();
            break;
        case 20: // Zoom In
            zoomIn();
            break;
        case 21: // Zoom Out
            zoomOut();
            break;
        case 22: // Reset Zoom
            resetZoom();
            break;
        case 23: // Toggle Auto-follow
            toggleAutoFollow();
            break;
        case 30: // Add Movement Clip
            addMovementClip();
            break;
        case 31: // Add Action Clip
            addActionClip();
            break;
            
        default:
            // Handle import overwrite and export menu items
            if (menuItemID >= 101 && menuItemID < 200)
            {
                // Import overwrite - menuItemID 101+ corresponds to timeline index 0+
                int timelineIndex = menuItemID - 101;
                importScene(timelineIndex);
            }
            else if (menuItemID >= 200 && menuItemID < 300)
            {
                // Export - menuItemID 200+ corresponds to timeline index 0+
                int timelineIndex = menuItemID - 200;
                exportScene(timelineIndex);
            }
            break;
    }
}

void AnimatorMainView::importScene(int targetTimelineIndex)
{
    juce::FileChooser chooser("Import Scene...",
                             juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.xml");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto xml = juce::XmlDocument::parse(file);
        
        if (xml != nullptr && xml->hasTagName("Timeline"))
        {
            auto* newTimeline = new TimelineModel();
            if (newTimeline->fromXml(*xml))
            {
                if (targetTimelineIndex == -1)
                {
                    // Append as new timeline
                    if (timelines == nullptr)
                        timelines = new juce::OwnedArray<TimelineModel>();
                        
                    timelines->add(newTimeline);
                    
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                          "Import Successful",
                                                          "Timeline imported and appended successfully.");
                }
                else
                {
                    // Overwrite existing timeline
                    if (timelines != nullptr && targetTimelineIndex < timelines->size())
                    {
                        timelines->set(targetTimelineIndex, newTimeline);
                        
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                              "Import Successful",
                                                              "Timeline imported and overwritten successfully.");
                    }
                    else
                    {
                        delete newTimeline;
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                              "Import Error",
                                                              "Invalid timeline index for overwrite.");
                        return;
                    }
                }
                
                // Update the timeline component
                timelineComponent->setTimelines(timelines);
                timelineComponent->repaint();
            }
            else
            {
                delete newTimeline;
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                      "Import Error",
                                                      "Failed to import timeline file.");
            }
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Import Error",
                                                  "Invalid timeline file format.");
        }
    }
}

void AnimatorMainView::exportScene(int timelineIndex)
{
    if (timelines == nullptr || timelines->isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Export Error",
                                              "No timeline data to export.");
        return;
    }
    
    if (timelineIndex < 0 || timelineIndex >= timelines->size())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Export Error",
                                              "Invalid timeline index.");
        return;
    }
    
    auto* timelineToExport = (*timelines)[timelineIndex];
    juce::String timelineName = "Group " + juce::String(timelineIndex + 1);
    
    juce::FileChooser chooser("Export Scene: " + timelineName,
                             juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.xml");
    
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension("xml");
        auto xml = timelineToExport->toXml();
        
        if (xml != nullptr && xml->writeTo(file))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                  "Export Successful",
                                                  "Timeline exported successfully to: " + file.getFullPathName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Export Error",
                                                  "Failed to export timeline file.");
        }
    }
}

void AnimatorMainView::cutSelectedClips()
{
    copySelectedClips();
    deleteSelectedClips();
}

void AnimatorMainView::copySelectedClips()
{
    // Clear previous clipboard data
    clipboard.clips.clear();
    clipboard.timelineData.clear();
    clipboard.hasData = false;
    
    // Get selected clips from timeline component using its existing selection system
    auto selectedClips = timelineComponent->getSelectedClips();
    
    if (selectedClips.isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Copy",
                                              "No clips selected to copy.");
        return;
    }
    
    // Create a temporary timeline to store only the selected clips
    auto* timelineCopy = new TimelineModel();
    
    // Group clips by their original timeline to maintain structure
    juce::HashMap<int, TimelineModel*> timelineCopies;
    timelineCopies.set(0, timelineCopy); // Use main timeline copy
    
    // Copy selected clips
    for (const auto& selected : selectedClips)
    {
        bool isMovementClip = selected.isMovementClip;
        auto* originalClip = timelineComponent->getClip(selected.timelineIndex, selected.layerIndex,
                                                       selected.clipIndex, isMovementClip);
        if (!originalClip) continue;
        
        // Use the main timeline copy for all clips
        auto* targetTimeline = timelineCopies[0];
        
        if (isMovementClip)
        {
            MovementClip newClip = *static_cast<const MovementClip*>(originalClip);
            targetTimeline->movement.clips.add(newClip);
        }
        else
        {
            ActionClip newClip = *static_cast<const ActionClip*>(originalClip);
            targetTimeline->actions.clips.add(newClip);
        }
    }
    
    clipboard.timelineData.add(timelineCopy);
    clipboard.hasData = true;
}

void AnimatorMainView::pasteClips()
{
    if (!clipboard.hasData || clipboard.timelineData.isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Paste",
                                              "Clipboard is empty.");
        return;
    }
        
    if (auto* currentTimeline = timelineComponent->getCurrentTimeline())
    {
        if (auto* sourceTimeline = clipboard.timelineData.getFirst())
        {
            // Clear current selection
            timelineComponent->clearSelection();
            
            // Calculate time offset based on playhead position
            ms_t timeOffset = timelineComponent->getPlayheadPosition();
            
            // Find the earliest clip time in the clipboard to maintain relative timing
            ms_t earliestTime = std::numeric_limits<ms_t>::max();
            for (const auto& clip : sourceTimeline->movement.clips)
                earliestTime = juce::jmin(earliestTime, clip.start);
            for (const auto& clip : sourceTimeline->actions.clips)
                earliestTime = juce::jmin(earliestTime, clip.start);
            
            if (earliestTime == std::numeric_limits<ms_t>::max())
                earliestTime = 0;
            
            // Paste movement clips with time offset
            for (const auto& clip : sourceTimeline->movement.clips)
            {
                MovementClip newClip = clip;
                newClip.start = timeOffset + (clip.start - earliestTime);
                
                // Generate unique ID to avoid conflicts
                newClip.id = generateUniqueClipId(currentTimeline->movement.clips, clip.id);
                
                int newClipIndex = currentTimeline->movement.clips.size();
                currentTimeline->movement.clips.add(newClip);
                
                // Select the newly pasted clip
                timelineComponent->selectClip(timelineComponent->getCurrentTimelineIndex(),
                                             0, newClipIndex, true, true);
            }
            
            // Paste action clips with time offset
            for (const auto& clip : sourceTimeline->actions.clips)
            {
                ActionClip newClip = clip;
                newClip.start = timeOffset + (clip.start - earliestTime);
                
                // Generate unique ID to avoid conflicts
                newClip.id = generateUniqueClipId(currentTimeline->actions.clips, clip.id);
                
                int newClipIndex = currentTimeline->actions.clips.size();
                currentTimeline->actions.clips.add(newClip);
                
                // Select the newly pasted clip
                timelineComponent->selectClip(timelineComponent->getCurrentTimelineIndex(),
                                             1, newClipIndex, false, true);
            }
            
            timelineComponent->repaint();
            
            // Optional: Provide feedback
            int totalClips = sourceTimeline->movement.clips.size() + sourceTimeline->actions.clips.size();
        }
    }
    else
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Paste Error",
                                              "No active timeline to paste into.");
    }
}

void AnimatorMainView::selectAllClips()
{
    timelineComponent->keyPressed(juce::KeyPress('a', juce::ModifierKeys::commandModifier, 0));
}

void AnimatorMainView::deselectAllClips()
{
    timelineComponent->keyPressed(juce::KeyPress(juce::KeyPress::escapeKey));
}

void AnimatorMainView::addMovementClip()
{
    if (auto* currentTimeline = timelineComponent->getCurrentTimeline())
    {
        MovementClip newClip;
        newClip.id = "Movement " + juce::String(currentTimeline->movement.clips.size() + 1);
        newClip.start = timelineComponent->getPlayheadPosition();
        newClip.length = 2000; // 2 second default
        newClip.colour = juce::Colours::cornflowerblue;
        
        currentTimeline->movement.clips.add(newClip);
        timelineComponent->repaint();
    }
}

void AnimatorMainView::addActionClip()
{
    if (auto* currentTimeline = timelineComponent->getCurrentTimeline())
    {
        ActionClip newClip;
        newClip.id = "Action " + juce::String(currentTimeline->actions.clips.size() + 1);
        newClip.start = timelineComponent->getPlayheadPosition();
        newClip.length = 2000; // 2 second default
        newClip.colour = juce::Colours::orange;
        
        // Add a default action
        ActionDefinition defaultAction;
        defaultAction.action = ActionType::RotationX;
        defaultAction.timing = TimingType::AbsoluteTarget;
        defaultAction.value = 45.0;
        newClip.actions.add(defaultAction);
        
        currentTimeline->actions.clips.add(newClip);
        timelineComponent->repaint();
    }
}

void AnimatorMainView::deleteSelectedClips()
{
    timelineComponent->keyPressed(juce::KeyPress(juce::KeyPress::deleteKey));
}

void AnimatorMainView::zoomIn()
{
    zoomLevel = juce::jmin(MAX_ZOOM, zoomLevel * ZOOM_STEP);
    updateTimelineZoom();
}

void AnimatorMainView::zoomOut()
{
    zoomLevel = juce::jmax(MIN_ZOOM, zoomLevel / ZOOM_STEP);
    updateTimelineZoom();
}

void AnimatorMainView::resetZoom()
{
    zoomLevel = DEFAULT_ZOOM;
    updateTimelineZoom();
}

void AnimatorMainView::toggleAutoFollow()
{
    autoFollowEnabled = !autoFollowEnabled;
    timelineComponent->setAutoFollow(autoFollowEnabled);
    
    // Update toolbar button state
    if (toolbar != nullptr && toolbar->autoFollowButton != nullptr)
    {
        toolbar->autoFollowButton->setToggleState(autoFollowEnabled, juce::dontSendNotification);
    }
    
    toolbar->repaint();
}

void AnimatorMainView::updateTimelineZoom()
{
    // Convert zoom level to pixels per millisecond
    float basePixelsPerMs = 0.1f;
    float pixelsPerMs = basePixelsPerMs * zoomLevel;
    
    timelineComponent->setPixelsPerMillisecond(pixelsPerMs);
}

// ToolbarComponent implementation
AnimatorMainView::ToolbarComponent::ToolbarComponent(AnimatorMainView& ownerRef)
    : owner(ownerRef)
{
    // Create transparent buttons for custom drawing
    addMovementButton = std::make_unique<juce::TextButton>();
    addActionButton = std::make_unique<juce::TextButton>();
    deleteButton = std::make_unique<juce::TextButton>();
    zoomInButton = std::make_unique<juce::TextButton>();
    zoomOutButton = std::make_unique<juce::TextButton>();
    resetZoomButton = std::make_unique<juce::TextButton>();
    autoFollowButton = std::make_unique<juce::TextButton>();

    // Make buttons transparent and remove borders
    auto setupIconButton = [](juce::TextButton* button) {
        button->setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        button->setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
        button->setClickingTogglesState(false);
    };

    setupIconButton(addMovementButton.get());
    setupIconButton(addActionButton.get());
    setupIconButton(deleteButton.get());
    setupIconButton(zoomInButton.get());
    setupIconButton(zoomOutButton.get());
    setupIconButton(resetZoomButton.get());
    
    // Auto-follow button should toggle state but still be transparent
    autoFollowButton->setClickingTogglesState(true);
    autoFollowButton->setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    autoFollowButton->setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);

    // Set tooltips (same as before)
    addMovementButton->setTooltip("Add Movement Clip");
    addActionButton->setTooltip("Add Action Clip");
    deleteButton->setTooltip("Delete Selected Clips");
    zoomInButton->setTooltip("Zoom In");
    zoomOutButton->setTooltip("Zoom Out");
    resetZoomButton->setTooltip("Reset Zoom");
    autoFollowButton->setTooltip("Toggle Auto-follow");

    // Connect buttons to actions (same as before)
    addMovementButton->onClick = [this] { owner.addMovementClip(); };
    addActionButton->onClick = [this] { owner.addActionClip(); };
    deleteButton->onClick = [this] { owner.deleteSelectedClips(); };
    zoomInButton->onClick = [this] { owner.zoomIn(); };
    zoomOutButton->onClick = [this] { owner.zoomOut(); };
    resetZoomButton->onClick = [this] { owner.resetZoom(); };
    autoFollowButton->onClick = [this] {
        owner.toggleAutoFollow();
        repaint(); // Repaint to update icon color
    };

    // Add buttons to component
    addAndMakeVisible(addMovementButton.get());
    addAndMakeVisible(addActionButton.get());
    addAndMakeVisible(deleteButton.get());
    addAndMakeVisible(zoomInButton.get());
    addAndMakeVisible(zoomOutButton.get());
    addAndMakeVisible(resetZoomButton.get());
    addAndMakeVisible(autoFollowButton.get());
}

void AnimatorMainView::ToolbarComponent::paint(juce::Graphics& g)
{
    // Draw toolbar background
    g.fillAll(juce::Colour(0xff2d2d30));
    
    // Draw subtle border at bottom
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawLine(0.0f, (float)getHeight(), (float)getWidth(), (float)getHeight(), 1.0f);
    
    // Draw each icon on its respective button
    drawButtonIcon(g, addMovementButton.get(), createMovementIcon());
    drawButtonIcon(g, addActionButton.get(), createActionIcon());
    drawButtonIcon(g, deleteButton.get(), createDeleteIcon());
    drawButtonIcon(g, zoomInButton.get(), createZoomInIcon());
    drawButtonIcon(g, zoomOutButton.get(), createZoomOutIcon());
    drawButtonIcon(g, resetZoomButton.get(), createResetZoomIcon());
    drawButtonIcon(g, autoFollowButton.get(), createAutoFollowIcon(), owner.autoFollowEnabled);
}

void AnimatorMainView::ToolbarComponent::drawButtonIcon(juce::Graphics& g, juce::Button* button, const juce::Path& icon, bool isToggled)
{
    auto bounds = button->getLocalBounds().toFloat().reduced(6.0f);
    
    // Set color based on button state
    if (isToggled || button->getToggleState()) {
        g.setColour(juce::Colour(0xff0078d7));
    } else if (button->isOver()) {
        g.setColour(juce::Colours::white.withAlpha(0.9f));
    } else {
        g.setColour(juce::Colours::white.withAlpha(0.7f));
    }
    
    g.fillPath(icon, icon.getTransformToScaleToFit(bounds, true));
}

void AnimatorMainView::ToolbarComponent::resized()
{
    auto area = getLocalBounds().reduced(5, 5);
    int buttonSize = 30;
    int spacing = 5;
    
    addMovementButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing);
    addActionButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing * 2);
    
    deleteButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing * 2);
    
    zoomOutButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing);
    zoomInButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing);
    resetZoomButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing * 2);
    
    autoFollowButton->setBounds(area.removeFromLeft(buttonSize));
}

juce::Path AnimatorMainView::ToolbarComponent::createMovementIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0) so the path scales properly
    path.addTriangle({0.0f, 1.0f}, {1.0f, 0.5f}, {0.0f, 0.0f});
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createActionIcon()
{
    juce::Path path;
    path.addEllipse(0.2f, 0.2f, 0.6f, 0.6f);
    
    for (int i = 0; i < 8; ++i)
    {
        float angle = i * juce::MathConstants<float>::pi / 4.0f;
        juce::Path tooth;
        tooth.addRectangle(0.45f, 0.1f, 0.1f, 0.3f);
        tooth.applyTransform(juce::AffineTransform::rotation(angle, 0.5f, 0.5f));
        path.addPath(tooth);
    }
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createDeleteIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0)
    path.addRectangle(0.2f, 0.3f, 0.6f, 0.1f);  // Horizontal line
    path.addRectangle(0.3f, 0.1f, 0.4f, 0.1f);  // Top part
    path.addRectangle(0.35f, 0.5f, 0.1f, 0.4f); // Left tine
    path.addRectangle(0.45f, 0.5f, 0.1f, 0.4f); // Middle tine
    path.addRectangle(0.55f, 0.5f, 0.1f, 0.4f); // Right tine
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createZoomInIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0)
    path.addEllipse(0.1f, 0.1f, 0.5f, 0.5f); // Magnifying glass
    path.addLineSegment({0.5f, 0.35f, 0.8f, 0.65f}, 0.1f); // Handle
    path.addLineSegment({0.35f, 0.35f, 0.65f, 0.35f}, 0.1f); // Horizontal plus
    path.addLineSegment({0.5f, 0.2f, 0.5f, 0.5f}, 0.1f); // Vertical plus
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createZoomOutIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0)
    path.addEllipse(0.1f, 0.1f, 0.5f, 0.5f); // Magnifying glass
    path.addLineSegment({0.5f, 0.35f, 0.8f, 0.65f}, 0.1f); // Handle
    path.addLineSegment({0.35f, 0.35f, 0.65f, 0.35f}, 0.1f); // Horizontal minus
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createResetZoomIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0)
    path.addRectangle(0.3f, 0.6f, 0.4f, 0.4f); // Base
    path.addTriangle({0.1f, 0.6f}, {0.5f, 0.2f}, {0.9f, 0.6f}); // Arrow
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createAutoFollowIcon()
{
    juce::Path path;
    // Use relative coordinates (0.0 to 1.0)
    path.addRectangle(0.45f, 0.1f, 0.1f, 0.8f); // Playhead
    path.addTriangle({0.2f, 0.3f}, {0.4f, 0.5f}, {0.2f, 0.7f}); // Left follow indicator
    path.addTriangle({0.6f, 0.3f}, {0.8f, 0.5f}, {0.6f, 0.7f}); // Right follow indicator
    return path;
}
