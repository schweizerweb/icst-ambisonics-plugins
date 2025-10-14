#include "AnimatorMainView.h"
#include "../../Common/UTF8Helpers.h"
#include "../../Common/SvgHelper.h"

AnimatorMainView::AnimatorMainView(AnimatorEngine* pEngine)
{
    pAnimatorEngine = pEngine;
    
    // Create menu bar model with pointer
    menuBarModel = std::make_unique<MainMenuBarModel>(this);
    menuBar = std::make_unique<juce::MenuBarComponent>(menuBarModel.get());
    
    // Create toolbar
    toolbar = std::make_unique<ToolbarComponent>(*this);
    
    // Create timeline component
    timelineViewport = std::make_unique<TimelineViewport>();
    timelineViewport->getTimelineComponent()->setAutoFollow(pAnimatorEngine->getAutoFollow());
    
    // Create status bar
    statusBar = std::make_unique<StatusBarComponent>(*this);
    
    // Create command manager
    commandManager = std::make_unique<juce::ApplicationCommandManager>();
    commandManager->registerAllCommandsForTarget(this);
    
    // Add as key listener to handle shortcuts globally
    addKeyListener(commandManager->getKeyMappings());
    
    // Add controls
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(toolbar.get());
    addAndMakeVisible(timelineViewport.get());
    addAndMakeVisible(statusBar.get());
    
    // Show initial status
    juce::AttributedString welcomeMessage;
    welcomeMessage.append("Timeline Animator Ready",
                         juce::FontOptions(12.0f, juce::Font::bold),
                         juce::Colours::lightgreen);
    
    setStatusMessage(welcomeMessage);
    
    commandManager->commandStatusChanged();
    
    // Start validation timer (1Hz default)
    startTimerHz(1);
}

AnimatorMainView::~AnimatorMainView()
{
    stopTimer();
    
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
        
    if (commandManager != nullptr)
    {
        commandManager->commandStatusChanged();
    }
}

void AnimatorMainView::setSelectionControl(PointSelection* pPointSelection)
{
    timelineViewport->getTimelineComponent()->setSelectionControl(pPointSelection);
}

void AnimatorMainView::setSourceSet(AmbiSourceSet *pSources)
{
    pSourceSet = pSources;
    timelineViewport->getTimelineComponent()->setSourceSet(pSources);
}

void AnimatorMainView::setPlayheadPosition(ms_t timeMs)
{
    timelineViewport->getTimelineComponent()->setPlayheadPosition(timeMs);
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
        case 0: // File - Keep custom handling for import/export
            {
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_addTimeline);
                // Remove Timeline submenu
                juce::PopupMenu removeSubMenu;
                
                // Add entries for each existing timeline
                if (owner->timelines != nullptr && !owner->timelines->isEmpty())
                {
                    for (int i = 0; i < owner->timelines->size(); ++i)
                    {
                        juce::String timelineName = "Group " + juce::String(i + 1);
                        removeSubMenu.addItem(300 + i, "Remove " + timelineName);
                    }
                    removeSubMenu.addSeparator();
                }
                
                // Add "Remove all invalid timelines" entry
                removeSubMenu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_removeAllInvalid);
                
                menu.addSubMenu("Remove Timeline...", removeSubMenu);
                
                menu.addSeparator();
                menu.addItem(6, "Preferences", false); // not implemented yet
        
                menu.addSeparator();
                
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
            
        case 1: // Edit - Use command items for standard operations
            {
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_undo);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_redo);
                menu.addSeparator();
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_cut);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_copy);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_paste);
                menu.addSeparator();
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_deleteSelected);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_duplicate);
                menu.addSeparator();
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_selectAll);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_deselectAll);
                menu.addSeparator();
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_addMovementClip);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_addActionClip);
            }
            break;
            
        case 2: // View - Use command items
            {
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_zoomIn);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_zoomOut);
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_resetZoom);
                menu.addSeparator();
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_toggleAutoFollow);
            }
            break;
            
        case 3: // Playback - Use command items
            {
                menu.addCommandItem(owner->commandManager.get(), AnimatorMainView::CMD_toggleOnOff);
            }
            break;
    }
    
    return menu;
}

void AnimatorMainView::MainMenuBarModel::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/)
{
    if (owner != nullptr)
    {
        // Only handle custom menu items (import/export, undo/redo, preferences)
        // Command items are handled automatically by ApplicationCommandManager
        if (menuItemID == 10 || menuItemID == 11 || menuItemID == 6 ||
            menuItemID == 100 ||
            (menuItemID >= 101 && menuItemID < 400))
        {
            owner->handleMenuAction(menuItemID);
        }
        // Command items (CMD_*) are handled automatically
    }
}

void AnimatorMainView::handleMenuAction(int menuItemID)
{
    // Only handle custom menu items (import/export, undo/redo, preferences)
    // Command items are handled automatically by ApplicationCommandManager
    
    switch (menuItemID)
    {
        case 100: // Import - Append as new timeline
            importScene(-1); // -1 means append
            break;
            
        case 6: // Preferences
            // Handle preferences
            break;
            
        case 10: // Undo
            // Handle undo (not implemented yet)
            break;
            
        case 11: // Redo
            // Handle redo (not implemented yet)
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
            // Handle remove individual timeline
            else if (menuItemID >= 300 && menuItemID < 400)
            {
                int timelineIndex = menuItemID - 300;
                removeTimeline(timelineIndex);
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

void AnimatorMainView::toggleAutoFollow()
{
    bool newState = !pAnimatorEngine->getAutoFollow();
    pAnimatorEngine->setAutoFollow(newState);
    timelineViewport->getTimelineComponent()->setAutoFollow(newState);
    
    // Force toolbar to re-read the state from the engine
    if (toolbar != nullptr)
    {
        toolbar->refreshButtonStates();
    }
    
    // Update command manager to refresh menu checkmarks
    if (commandManager != nullptr)
    {
        commandManager->commandStatusChanged();
    }
    
    toolbar->repaint();
}

void AnimatorMainView::toggleOnOff()
{
    bool newState = !pAnimatorEngine->getAnimatorState();
    pAnimatorEngine->setAnimatorState(newState);
    
    // Update toolbar button states
    if (toolbar != nullptr)
    {
        toolbar->refreshButtonStates();
    }
    
    // Update command manager to refresh menu states
    if (commandManager != nullptr)
    {
        commandManager->commandStatusChanged();
    }
    
    toolbar->repaint();
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
    autoFollowButton = std::make_unique<ColorDrawableToggleButton>("Auto Follow");
    animatorOnOff = std::make_unique<ColorDrawableToggleButton>("Animator OnOff");

    SvgHelper::loadSVGIcon(addMovementButton.get(), BinaryData::movement_icon_svg, BinaryData::movement_icon_svgSize, "Add Movement Clip");
    SvgHelper::loadSVGIcon(addActionButton.get(), BinaryData::action_icon_svg, BinaryData::action_icon_svgSize, "Add Action Clip");
    SvgHelper::loadSVGIcon(deleteButton.get(), BinaryData::delete_icon_svg, BinaryData::delete_icon_svgSize, "Delete Selected Clips");
    SvgHelper::loadSVGIcon(zoomInButton.get(), BinaryData::zoom_in_icon_svg, BinaryData::zoom_in_icon_svgSize, "Zoom In");
    SvgHelper::loadSVGIcon(zoomOutButton.get(), BinaryData::zoom_out_icon_svg, BinaryData::zoom_out_icon_svgSize, "Zoom Out");
    SvgHelper::loadSVGIcon(resetZoomButton.get(), BinaryData::reset_zoom_icon_svg, BinaryData::reset_zoom_icon_svgSize, "Reset Zoom");
    SvgHelper::loadSVGIcon(autoFollowButton.get(), BinaryData::auto_follow_icon_svg, BinaryData::auto_follow_icon_svgSize, "Toggle Auto-follow");
    SvgHelper::loadSVGIcon(animatorOnOff.get(), BinaryData::play_icon_svg, BinaryData::play_icon_svgSize, "Turn Animator ON/OFF");

    autoFollowButton->setClickingTogglesState(true);
    autoFollowButton->setToggleState(owner.pAnimatorEngine->getAutoFollow(), juce::dontSendNotification);

    animatorOnOff->setClickingTogglesState(true);
    animatorOnOff->setToggleState(owner.pAnimatorEngine->getAnimatorState(), juce::dontSendNotification);

    // Connect buttons to actions
    addMovementButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_addMovementClip, true);
    };
    addActionButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_addActionClip, true);
    };
    deleteButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_deleteSelected, true);
    };
    zoomInButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_zoomIn, true);
    };
    zoomOutButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_zoomOut, true);
    };
    resetZoomButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_resetZoom, true);
    };
    autoFollowButton->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_toggleAutoFollow, true);
    };
    animatorOnOff->onClick = [this] {
        owner.commandManager->invokeDirectly(AnimatorMainView::CMD_toggleOnOff, true);
    };
    
    
    // Add buttons to component
    addAndMakeVisible(addMovementButton.get());
    addAndMakeVisible(addActionButton.get());
    addAndMakeVisible(deleteButton.get());
    addAndMakeVisible(zoomInButton.get());
    addAndMakeVisible(zoomOutButton.get());
    addAndMakeVisible(resetZoomButton.get());
    addAndMakeVisible(autoFollowButton.get());
    addAndMakeVisible(animatorOnOff.get());
}

void AnimatorMainView::ToolbarComponent::paint(juce::Graphics& g)
{
    // Draw toolbar background only
    g.fillAll(juce::Colour(0xff2d2d30));
    
    // Draw subtle border at bottom
    g.setColour(juce::Colours::grey.withAlpha(0.3f));
    g.drawLine(0.0f, (float)getHeight(), (float)getWidth(), (float)getHeight(), 1.0f);
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
    
    animatorOnOff->setBounds(area.removeFromRight(buttonSize));
}

void AnimatorMainView::ToolbarComponent::refreshButtonStates()
{
    if (autoFollowButton != nullptr)
    {
        autoFollowButton->setToggleState(owner.pAnimatorEngine->getAutoFollow(), juce::dontSendNotification);
    }
    if (animatorOnOff != nullptr)
    {
        animatorOnOff->setToggleState(owner.pAnimatorEngine->getAnimatorState(), juce::dontSendNotification);
    }
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

void AnimatorMainView::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    const juce::CommandID commandList[] = {
        CMD_cut, CMD_copy, CMD_paste, CMD_deleteSelected, CMD_duplicate,
        CMD_selectAll, CMD_deselectAll, CMD_zoomIn, CMD_zoomOut, CMD_resetZoom,
        CMD_addMovementClip, CMD_addActionClip, CMD_toggleAutoFollow, CMD_undo, CMD_redo, CMD_toggleOnOff,
        CMD_addTimeline, CMD_removeAllInvalid
    };
    
    commands.addArray(commandList, numElementsInArray(commandList));
}

void AnimatorMainView::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    bool isMac = false;
#if JUCE_MAC
    isMac = true;
#endif

    switch (commandID)
    {
        case CMD_cut:
            result.setInfo("Cut", "Cut selected clips", "Edit", 0);
            result.addDefaultKeypress('X', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(timelineViewport->getTimelineComponent()->hasSelectedClips());
            break;
            
        case CMD_copy:
            result.setInfo("Copy", "Copy selected clips", "Edit", 0);
            result.addDefaultKeypress('C', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(timelineViewport->getTimelineComponent()->hasSelectedClips());
            break;
            
        case CMD_paste:
            result.setInfo("Paste", "Paste clips", "Edit", 0);
            result.addDefaultKeypress('V', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(timelineViewport->getTimelineComponent()->hasClipboardData());
            break;
            
        case CMD_deleteSelected:
            result.setInfo("Delete", "Delete selected clips", "Edit", 0);
            result.addDefaultKeypress(juce::KeyPress::deleteKey, 0);
            result.addDefaultKeypress(juce::KeyPress::backspaceKey, 0);
            result.setActive(timelineViewport->getTimelineComponent()->hasSelectedClips());
            break;
            
        case CMD_duplicate:
            result.setInfo("Duplicate", "Duplicate selected clips", "Edit", 0);
            result.addDefaultKeypress('D', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(timelineViewport->getTimelineComponent()->hasSelectedClips());
            break;
            
        case CMD_selectAll:
            result.setInfo("Select All", "Select all clips", "Edit", 0);
            result.addDefaultKeypress('A', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            break;
            
        case CMD_deselectAll:
            result.setInfo("Deselect All", "Deselect all clips", "Edit", 0);
            result.addDefaultKeypress(juce::KeyPress::escapeKey, 0);
            result.setActive(timelineViewport->getTimelineComponent()->hasSelectedClips());
            break;
            
        case CMD_zoomIn:
            result.setInfo("Zoom In", "Zoom in timeline", "View", 0);
            result.addDefaultKeypress('=', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(true);
            break;
            
        case CMD_zoomOut:
            result.setInfo("Zoom Out", "Zoom out timeline", "View", 0);
            result.addDefaultKeypress('-', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(true);
            break;
            
        case CMD_resetZoom:
            result.setInfo("Reset Zoom", "Reset zoom level", "View", 0);
            result.setActive(true);
            break;
            
        case CMD_addMovementClip:
            result.setInfo("Add Movement Clip", "Add a new movement clip", "Edit", 0);
            break;
            
        case CMD_addActionClip:
            result.setInfo("Add Action Clip", "Add a new action clip", "Edit", 0);
            break;
            
        case CMD_toggleAutoFollow:
            result.setInfo("Auto-follow", "Toggle auto-follow mode", "View", 0);
            result.setTicked(pAnimatorEngine->getAutoFollow());
            result.setActive(true);
            break;
        
        case CMD_undo:
            result.setInfo("Undo", "Undo last manipulation", "Edit", 0);
            result.addDefaultKeypress('Z', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(false);
            break;
            
        case CMD_redo:
            result.setInfo("Redo", "Redo last undone manipulation", "Edit", 0);
            result.addDefaultKeypress('Y', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(false);
            break;
            
        case CMD_toggleOnOff:
            result.setInfo("Toggle ON/OFF", "Toggles the Animator Engine", "Playback", 0);
            result.setTicked(pAnimatorEngine->getAnimatorState());
            result.setActive(true);
            break;
        
        case CMD_addTimeline:
            result.setInfo("Add Timeline", "Add a new empty timeline", "File", 0);
            result.addDefaultKeypress('N', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(true);
            break;
            
        case CMD_removeAllInvalid:
            result.setInfo("Remove all invalid timelines", "Remove timelines exceeding group count", "File", 0);
            result.setActive(hasInvalidTimelines()); // This will be grayed out if no invalid timelines
            break;
    }
}

bool AnimatorMainView::perform(const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    auto* timelineComp = timelineViewport->getTimelineComponent();
    
    switch (info.commandID)
    {
        case CMD_cut:
            timelineComp->cutSelectedClips();
            return true;
        case CMD_copy:
            timelineComp->copySelectedClips();
            return true;
        case CMD_paste:
            timelineComp->pasteClips();
            return true;
        case CMD_deleteSelected:
            timelineComp->deleteSelectedClips();
            return true;
        case CMD_duplicate:
            timelineComp->duplicateSelectedClips();
            return true;
        case CMD_selectAll:
            timelineComp->selectAllClips();
            return true;
        case CMD_deselectAll:
            timelineComp->deselectAllClips();
            return true;
        case CMD_zoomIn:
            timelineComp->zoom(ZOOM_STEP);
            return true;
        case CMD_zoomOut:
            timelineComp->zoom(1.0f/ZOOM_STEP);
            return true;
        case CMD_resetZoom:
            timelineComp->zoom(TimelineComponent::ZOOM_RESET);
            return true;
        case CMD_addMovementClip:
            timelineComp->addMovementClip();
            return true;
        case CMD_addActionClip:
            timelineComp->addActionClip();
            return true;
        case CMD_toggleAutoFollow:
            toggleAutoFollow();
            return true;
        case CMD_undo:
            {
                juce::AttributedString msg;
                msg.append("Undo is not implemented yet!",
                           juce::FontOptions(12.0f, juce::Font::bold),
                           juce::Colours::orangered);
                setStatusMessage(msg);
            }
            return true;
        case CMD_redo:
            { juce::AttributedString msg;
                msg.append("Redo is not implemented yet!",
                           juce::FontOptions(12.0f, juce::Font::bold),
                           juce::Colours::orangered);
                setStatusMessage(msg);
            }
            return true;
        case CMD_toggleOnOff:
            toggleOnOff();
            return true;
            
        case CMD_addTimeline:
            addNewTimeline();
            return true;
            
        case CMD_removeAllInvalid:
            removeAllInvalidTimelines();
            return true;
    }
    return false;
}

bool AnimatorMainView::hasInvalidTimelines() const
{
    if (timelines == nullptr || pAnimatorEngine == nullptr || pSourceSet == nullptr)
        return false;
        
    int groupCount = pSourceSet->activeGroupCount();
    return timelines->size() > groupCount;
}

juce::Array<int> AnimatorMainView::getInvalidTimelineIndices() const
{
    juce::Array<int> invalidIndices;
    
    if (timelines == nullptr || pAnimatorEngine == nullptr || pSourceSet == nullptr)
        return invalidIndices;
        
    int groupCount = pSourceSet->activeGroupCount();
    
    for (int i = groupCount; i < timelines->size(); ++i)
    {
        invalidIndices.add(i);
    }
    
    return invalidIndices;
}

void AnimatorMainView::addNewTimeline()
{
    if (timelines == nullptr)
        timelines = new juce::OwnedArray<TimelineModel>();
    
    auto* newTimeline = new TimelineModel();
    timelines->add(newTimeline);
    
    // Update the timeline component
    timelineViewport->setTimelines(timelines);
    timelineViewport->repaint();
    
    // Refresh menus
    if (menuBarModel != nullptr)
        menuBarModel->menuItemsChanged();
    if (commandManager != nullptr)
        commandManager->commandStatusChanged();
    
    // Show status message
    juce::AttributedString msg;
    msg.append("Added new empty timeline",
               juce::FontOptions(12.0f, juce::Font::bold),
               juce::Colours::lightgreen);
    setStatusMessage(msg);
}
    
void AnimatorMainView::removeAllInvalidTimelines()
{
    auto invalidIndices = getInvalidTimelineIndices();
    if (invalidIndices.isEmpty())
        return;
    
    // Create confirmation message
    juce::String message = "This will permanently delete the following timelines and all their clips:\n\n";
    
    for (auto index : invalidIndices)
    {
        message += "- Group " + juce::String(index + 1) + "\n";
    }
    
    message += "\nThis action cannot be undone. Continue?";
    
    // Show confirmation dialog
    juce::AlertWindow::showOkCancelBox(
                                       juce::AlertWindow::WarningIcon,
                                       "Confirm Deletion",
                                       message,
                                       "Delete",
                                       "Cancel",
                                       this,
                                       juce::ModalCallbackFunction::create([this, invalidIndices](int result) {
                                           if (result != 0) // User clicked "Delete"
                                           {
                                               // Remove timelines from highest index to lowest to avoid index issues
                                               for (int i = invalidIndices.size() - 1; i >= 0; --i)
                                               {
                                                   int indexToRemove = invalidIndices[i];
                                                   if (indexToRemove < timelines->size())
                                                   {
                                                       timelines->remove(indexToRemove);
                                                   }
                                               }
                                               
                                               // Update the timeline component
                                               timelineViewport->setTimelines(timelines);
                                               timelineViewport->repaint();
                                               
                                               // Refresh menus
                                               if (menuBarModel != nullptr)
                                                   menuBarModel->menuItemsChanged();
                                               if (commandManager != nullptr)
                                                   commandManager->commandStatusChanged();
                                               
                                               // Show status message
                                               juce::AttributedString msg;
                                               msg.append("Removed " + juce::String(invalidIndices.size()) + " invalid timelines",
                                                          juce::FontOptions(12.0f, juce::Font::bold),
                                                          juce::Colours::lightgreen);
                                               setStatusMessage(msg);
                                           }
                                       })
                                       );
}

void AnimatorMainView::removeTimeline(int timelineIndex)
{
    if (timelines == nullptr || timelineIndex < 0 || timelineIndex >= timelines->size())
        return;
    
    juce::String timelineName = "Group " + juce::String(timelineIndex + 1);
    
    juce::AlertWindow::showOkCancelBox(
                                       juce::AlertWindow::WarningIcon,
                                       "Confirm Deletion",
                                       "This will permanently delete " + timelineName + " and all its clips.\nThis action cannot be undone. Continue?",
                                       "Delete",
                                       "Cancel",
                                       this,
                                       juce::ModalCallbackFunction::create([this, timelineIndex, timelineName](int result) {
                                           if (result != 0) // User clicked "Delete"
                                           {
                                               timelines->remove(timelineIndex);
                                               
                                               // Update the timeline component
                                               timelineViewport->setTimelines(timelines);
                                               timelineViewport->repaint();
                                               
                                               // Refresh menus
                                               if (menuBarModel != nullptr)
                                                   menuBarModel->menuItemsChanged();
                                               if (commandManager != nullptr)
                                                   commandManager->commandStatusChanged();
                                               
                                               // Show status message
                                               juce::AttributedString msg;
                                               msg.append("Removed " + timelineName,
                                                          juce::FontOptions(12.0f, juce::Font::bold),
                                                          juce::Colours::lightgreen);
                                               setStatusMessage(msg);
                                           }
                                       })
                                       );
}
