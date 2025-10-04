#include "AnimatorMainView.h"
#include "../../Common/UTF8Helpers.h"

AnimatorMainView::AnimatorMainView()
{
    // Create menu bar model with pointer
    menuBarModel = std::make_unique<MainMenuBarModel>(this);
    menuBar = std::make_unique<juce::MenuBarComponent>(menuBarModel.get());
    
    // Create toolbar
    toolbar = std::make_unique<ToolbarComponent>(*this);
    
    // Create timeline component
    timelineViewport = std::make_unique<TimelineViewport>();
    
    // Create status bar
    statusBar = std::make_unique<StatusBarComponent>(*this);
    
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(toolbar.get());
    addAndMakeVisible(timelineViewport.get());
    addAndMakeVisible(statusBar.get());
    
    // Start validation timer (1Hz default)
    startTimerHz(1);
    
    // Show initial status
    juce::AttributedString welcomeMessage;
    welcomeMessage.append("Timeline Animator Ready",
                         juce::FontOptions(12.0f, juce::Font::bold),
                         juce::Colours::lightgreen);
    welcomeMessage.append(" - Create or import timelines to begin",
                         juce::FontOptions(12.0f),
                         juce::Colours::lightgrey);
    setStatusMessage(welcomeMessage);
}

AnimatorMainView::~AnimatorMainView()
{
    stopTimer();
    
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
    timelineViewport->setTimelines(timelines);
    timelineViewport->getTimelineComponent()->setStatusMessageFunction(getStatusMessageFunction());
    
    // Refresh menu bar to update the import/export submenus
    if (menuBarModel != nullptr)
    {
        menuBarModel->menuItemsChanged();
    }
}

void AnimatorMainView::setSelectionControl(PointSelection* pPointSelection)
{
    timelineViewport->getTimelineComponent()->setSelectionControl(pPointSelection);
}

void AnimatorMainView::setSourceSet(AmbiSourceSet *pSources)
{
    timelineViewport->getTimelineComponent()->setSourceSet(pSources);
}

void AnimatorMainView::setPlayheadPosition(ms_t timeMs)
{
    timelineViewport->getTimelineComponent()->setPlayheadPosition(timeMs);
}

void AnimatorMainView::setAutoFollow(bool shouldAutoFollow)
{
    autoFollowEnabled = shouldAutoFollow;
    timelineViewport->getTimelineComponent()->setAutoFollow(shouldAutoFollow);
}

void AnimatorMainView::setPlayheadProvider(std::function<PlayheadSnapshot()> /*provider*/)
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
    
    // Status bar at bottom (24 pixels)
    statusBar->setBounds(area.removeFromBottom(24));
    
    // Rest goes to timeline component
    timelineViewport->setBounds(area);
}

// Menu implementation
juce::PopupMenu AnimatorMainView::MainMenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& /*menuName*/)
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
                        juce::String timelineName = "Group " + juce::String(i + 1);
                            
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
                menu.addItem(6, "Preferences", false); // not implemented yet
            }
            break;
            
        case 1: // Edit
            menu.addItem(10, "Undo", false); // not implemented yet
            menu.addItem(11, "Redo", false); // not implemented yet
            menu.addSeparator();
            menu.addItem(12, "Cut");
            menu.addItem(13, "Copy");
            menu.addItem(14, "Paste");
            menu.addItem(15, "Delete");
            menu.addItem(18, "Duplicate");
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
            break;
            
        case 3: // Insert
            menu.addItem(30, "Add Movement Clip");
            menu.addItem(31, "Add Action Clip");
            break;
    }
    
    return menu;
}

void AnimatorMainView::MainMenuBarModel::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/)
{
    if (owner != nullptr)
        owner->handleMenuAction(menuItemID);
}

void AnimatorMainView::handleMenuAction(int menuItemID)
{
    auto* timelineComp = timelineViewport->getTimelineComponent();
    
    switch (menuItemID)
    {
        case 100: // Import - Append as new timeline
            importScene(-1); // -1 means append
            break;
            
        case 6: // Preferences
            // Handle preferences
            break;
            
        case 12: // Cut - Send Ctrl+X
            timelineComp->keyPressed(juce::KeyPress('X', juce::ModifierKeys::ctrlModifier, 'X'));
            break;
            
        case 13: // Copy - Send Ctrl+C
            timelineComp->keyPressed(juce::KeyPress('C', juce::ModifierKeys::ctrlModifier, 'C'));
            break;
            
        case 14: // Paste - Send Ctrl+V
            timelineComp->keyPressed(juce::KeyPress('V', juce::ModifierKeys::ctrlModifier, 'V'));
            break;
            
        case 15: // Delete Selected Clips - Send Delete key
            timelineComp->keyPressed(juce::KeyPress(juce::KeyPress::deleteKey));
            break;
            
        case 18: // Duplicate Selected Clips - Send Ctrl+D
            timelineComp->keyPressed(juce::KeyPress('D', juce::ModifierKeys::ctrlModifier, 'D'));
            break;
            
        case 16: // Select All - Send Ctrl+A
            timelineComp->keyPressed(juce::KeyPress('A', juce::ModifierKeys::ctrlModifier, 'A'));
            break;
            
        case 17: // Deselect All - Send Escape key
            timelineComp->keyPressed(juce::KeyPress(juce::KeyPress::escapeKey));
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
                timelineViewport->setTimelines(timelines);
                timelineViewport->repaint();
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

void AnimatorMainView::addMovementClip()
{
    if (auto* currentTimeline = timelineViewport->getTimelineComponent()->getCurrentTimeline())
    {
        MovementClip newClip;
        newClip.id = "Movement " + juce::String(currentTimeline->movement.clips.size() + 1);
        newClip.start = timelineViewport->getTimelineComponent()->getCursorTime();
        newClip.length = 2000; // 2 second default
        newClip.colour = juce::Colours::cornflowerblue;
        
        currentTimeline->movement.clips.add(newClip);
        timelineViewport->repaint();
    }
}

void AnimatorMainView::addActionClip()
{
    if (auto* currentTimeline = timelineViewport->getTimelineComponent()->getCurrentTimeline())
    {
        ActionClip newClip;
        newClip.id = "Action " + juce::String(currentTimeline->actions.clips.size() + 1);
        newClip.start = timelineViewport->getTimelineComponent()->getCursorTime();
        newClip.length = 2000; // 2 second default
        newClip.colour = juce::Colours::orange;
        
        // Add a default action
        ActionDefinition defaultAction;
        defaultAction.setAction(ActionType::RotationX);
        defaultAction.setTiming(TimingType::AbsoluteTarget);
        defaultAction.setValue(45.0);
        newClip.actions.add(defaultAction);
        
        currentTimeline->actions.clips.add(newClip);
        timelineViewport->repaint();
    }
}

void AnimatorMainView::deleteSelectedClips()
{
    timelineViewport->getTimelineComponent()->keyPressed(juce::KeyPress(juce::KeyPress::deleteKey));
}

void AnimatorMainView::duplicateSelectedClips()
{
    timelineViewport->getTimelineComponent()->keyPressed(juce::KeyPress('D', juce::ModifierKeys::ctrlModifier, 'D'));
}

void AnimatorMainView::zoomIn()
{
    timelineViewport->getTimelineComponent()->zoom(ZOOM_STEP);
}

void AnimatorMainView::zoomOut()
{
    timelineViewport->getTimelineComponent()->zoom(1.0f/ZOOM_STEP);
}

void AnimatorMainView::resetZoom()
{
    timelineViewport->getTimelineComponent()->zoom(TimelineComponent::ZOOM_RESET);
}

void AnimatorMainView::toggleAutoFollow()
{
    autoFollowEnabled = !autoFollowEnabled;
    timelineViewport->getTimelineComponent()->setAutoFollow(autoFollowEnabled);
    
    // Update toolbar button state
    //if (toolbar != nullptr && toolbar->autoFollowButton != nullptr)
    //{
    //    toolbar->autoFollowButton->setToggleState(autoFollowEnabled, juce::dontSendNotification);
    //}
    
    toolbar->repaint();
}

// ToolbarComponent implementation
void AnimatorMainView::ToolbarComponent::loadSVGIcon(juce::DrawableButton* button, const char* svgData, size_t svgDataSize, const juce::String& tooltip)
{
    if (svgData != nullptr && svgDataSize > 0)
    {
        auto svg = juce::Drawable::createFromImageData(svgData, svgDataSize);
        if (svg != nullptr)
        {
            svg->replaceColour(juce::Colours::black, juce::Colours::lightgrey);
            // Add some padding around the SVG
            svg->setTransformToFit(juce::Rectangle<float>(4, 4, 16, 16), juce::RectanglePlacement::centred);
            button->setImages(svg.get());
            drawables.add(std::move(svg));
        }
    }
    button->setTooltip(tooltip);
}

// In ToolbarComponent constructor
AnimatorMainView::ToolbarComponent::ToolbarComponent(AnimatorMainView& ownerRef)
    : owner(ownerRef)
{
    // Create drawable buttons
    addMovementButton = std::make_unique<juce::DrawableButton>("Add Movement", juce::DrawableButton::ImageOnButtonBackground);
    addActionButton = std::make_unique<juce::DrawableButton>("Add Action", juce::DrawableButton::ImageOnButtonBackground);
    deleteButton = std::make_unique<juce::DrawableButton>("Delete", juce::DrawableButton::ImageOnButtonBackground);
    zoomInButton = std::make_unique<juce::DrawableButton>("Zoom In", juce::DrawableButton::ImageOnButtonBackground);
    zoomOutButton = std::make_unique<juce::DrawableButton>("Zoom Out", juce::DrawableButton::ImageOnButtonBackground);
    resetZoomButton = std::make_unique<juce::DrawableButton>("Reset Zoom", juce::DrawableButton::ImageOnButtonBackground);
    autoFollowButton = std::make_unique<juce::DrawableButton>("Auto Follow", juce::DrawableButton::ImageOnButtonBackground);

    loadSVGIcon(addMovementButton.get(), BinaryData::movement_icon_svg, BinaryData::movement_icon_svgSize, "Add Movement Clip");
    loadSVGIcon(addActionButton.get(), BinaryData::action_icon_svg, BinaryData::action_icon_svgSize, "Add Action Clip");
    loadSVGIcon(deleteButton.get(), BinaryData::delete_icon_svg, BinaryData::delete_icon_svgSize, "Delete Selected Clips");
    loadSVGIcon(zoomInButton.get(), BinaryData::zoom_in_icon_svg, BinaryData::zoom_in_icon_svgSize, "Zoom In");
    loadSVGIcon(zoomOutButton.get(), BinaryData::zoom_out_icon_svg, BinaryData::zoom_out_icon_svgSize, "Zoom Out");
    loadSVGIcon(resetZoomButton.get(), BinaryData::reset_zoom_icon_svg, BinaryData::reset_zoom_icon_svgSize, "Reset Zoom");
    loadSVGIcon(autoFollowButton.get(), BinaryData::auto_follow_icon_svg, BinaryData::auto_follow_icon_svgSize, "Toggle Auto-follow");

    // Auto-follow button should toggle state
    autoFollowButton->setClickingTogglesState(true);
    autoFollowButton->setToggleState(owner.autoFollowEnabled, juce::dontSendNotification);
    // TODO: doesn't work
    autoFollowButton->setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::royalblue);
    
    // Connect buttons to actions
    addMovementButton->onClick = [this] { owner.addMovementClip(); };
    addActionButton->onClick = [this] { owner.addActionClip(); };
    deleteButton->onClick = [this] {
        owner.timelineViewport->getTimelineComponent()->keyPressed(
            juce::KeyPress(juce::KeyPress::deleteKey));
    };
    zoomInButton->onClick = [this] { owner.zoomIn(); };
    zoomOutButton->onClick = [this] { owner.zoomOut(); };
    resetZoomButton->onClick = [this] { owner.resetZoom(); };
    autoFollowButton->onClick = [this] { owner.toggleAutoFollow(); };

    // Add buttons to component
    addAndMakeVisible(addMovementButton.get());
    addAndMakeVisible(addActionButton.get());
    addAndMakeVisible(deleteButton.get());
    addAndMakeVisible(zoomInButton.get());
    addAndMakeVisible(zoomOutButton.get());
    addAndMakeVisible(resetZoomButton.get());
    addAndMakeVisible(autoFollowButton.get());
}

// Keep the paint method simple - just background
void AnimatorMainView::ToolbarComponent::paint(juce::Graphics& g)
{
    // Draw toolbar background only
    g.fillAll(juce::Colour(0xff2d2d30));
    
    // Draw subtle border at bottom
    g.setColour(juce::Colours::grey.withAlpha(0.3f));
    g.drawLine(0.0f, (float)getHeight(), (float)getWidth(), (float)getHeight(), 1.0f);
}

// Keep the resized method the same
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

// Status bar interface implementation
void AnimatorMainView::setStatusMessage(const juce::AttributedString& message)
{
    if (statusBar != nullptr)
    {
        statusBar->setMessage(message);
    }
}

void AnimatorMainView::clearStatusMessage()
{
    if (statusBar != nullptr)
    {
        statusBar->clearMessage();
    }
}

void AnimatorMainView::setValidationFrequency(double frequencyHz)
{
    stopTimer();
    if (frequencyHz > 0)
    {
        startTimerHz(static_cast<int>(frequencyHz));
    }
}

void AnimatorMainView::validateTimelines()
{
    if (timelines == nullptr || timelines->isEmpty())
    {
        validationResult = true;
        validationDetails = "No timelines to validate.";
        updateStatusBarValidation();
        return;
    }
    
    juce::StringArray issues;
    bool valid = true;
    
    // Perform validation checks
    for (int timelineIndex = 0; timelineIndex < timelines->size(); ++timelineIndex)
    {
        auto* timeline = (*timelines)[timelineIndex];
        if (timeline == nullptr) continue;
        
        juce::String timelineName = "Group " + juce::String(timelineIndex + 1);
        
        // Check for overlapping clips
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            juce::String layerName = layerIndex == 0 ? "Movement layer" : "Action layer";
            
            juce::Array<const Clip*> layerClips;
            
            // Collect all clips in this layer
            for (int clipIndex = 0; clipIndex < timeline->getNumClips(layerIndex); ++clipIndex)
            {
                bool isMovementClip = false;
                if (auto* clip = timelineViewport->getTimelineComponent()->getClip(timelineIndex, layerIndex, clipIndex, isMovementClip))
                {
                    layerClips.add(clip);
                }
            }
            
            // Check for overlaps
            for (int i = 0; i < layerClips.size(); ++i)
            {
                for (int j = i + 1; j < layerClips.size(); ++j)
                {
                    const auto* clip1 = layerClips[i];
                    const auto* clip2 = layerClips[j];
                    
                    if (clip1->start < clip2->end() && clip2->start < clip1->end())
                    {
                        valid = false;
                        issues.add(UTF8Helpers::dot() + " " + timelineName + " " + layerName +
                                  ": Overlapping clips at " + juce::String(clip1->start) + "ms and " +
                                  juce::String(clip2->start) + "ms");
                    }
                }
            }
        }
        
        // Check for clips with zero or negative duration
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            juce::String layerName = layerIndex == 0 ? "Movement layer" : "Action layer";
            
            for (int clipIndex = 0; clipIndex < timeline->getNumClips(layerIndex); ++clipIndex)
            {
                bool isMovementClip = false;
                if (auto* clip = timelineViewport->getTimelineComponent()->getClip(timelineIndex, layerIndex, clipIndex, isMovementClip))
                {
                    if (clip->length <= 0)
                    {
                        valid = false;
                        issues.add(UTF8Helpers::dot() + " " + timelineName + " " + layerName +
                                  " Clip " + juce::String(clipIndex) + ": Invalid duration " +
                                  juce::String(clip->length) + "ms");
                    }
                }
            }
        }
        
        // Check for clips with invalid start times
        for (int layerIndex = 0; layerIndex < timeline->getNumLayers(); ++layerIndex)
        {
            juce::String layerName = layerIndex == 0 ? "Movement layer" : "Action layer";
            
            for (int clipIndex = 0; clipIndex < timeline->getNumClips(layerIndex); ++clipIndex)
            {
                bool isMovementClip = false;
                if (auto* clip = timelineViewport->getTimelineComponent()->getClip(timelineIndex, layerIndex, clipIndex, isMovementClip))
                {
                    if (clip->start < 0)
                    {
                        valid = false;
                        issues.add(UTF8Helpers::dot() + " " + timelineName + " " + layerName +
                                  " Clip " + juce::String(clipIndex) + ": Negative start time " +
                                  juce::String(clip->start) + "ms");
                    }
                }
            }
        }
    }
    
    // Prepare validation results
    validationResult = valid;
    if (!issues.isEmpty())
    {
        validationDetails = "Validation issues found:\n" + issues.joinIntoString("\n");
        
        //juce::AttributedString message;
        //message.append(UTF8Helpers::xMark() + " Validation: " + juce::String(issues.size()) + " issues", juce::FontOptions(12.0f, juce::Font::bold), juce::Colours::orangered);
        //setStatusMessage(message);
    }
    
    updateStatusBarValidation();
}

void AnimatorMainView::updateStatusBarValidation()
{
    if (statusBar != nullptr)
    {
        statusBar->setValidationState(validationResult, validationDetails);
    }
}

void AnimatorMainView::timerCallback()
{
    validateTimelines();
}

std::function<void(const juce::AttributedString&)> AnimatorMainView::getStatusMessageFunction()
{
    return [this](const juce::AttributedString& message) {
        setStatusMessage(message);
    };
}
