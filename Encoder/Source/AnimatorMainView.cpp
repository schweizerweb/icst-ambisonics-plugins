#include "AnimatorMainView.h"
#include "../../Common/UTF8Helpers.h"
#include "../../Common/SvgHelper.h"
#include "../../Common/ScalingInfo.h"
#include "../../Common/UiState.h"

namespace
{
    constexpr const char* xmlAttributeDistanceScaler = "distanceScaler";

    double getCurrentDistanceScaler(AmbiSourceSet* pSourceSet)
    {
        return (pSourceSet != nullptr) ? pSourceSet->getDistanceScaler() : ScalingInfo::Infinite;
    }

    // Scales every spatial (i.e. distance-based) quantity in each imported group's movement clips
    // by ratio. Rotation/stretch actions are deliberately untouched: rotation angles and stretch
    // factors are dimensionless, not room-relative, so they mean the same thing at any scale.
    void rescaleMovementClips(juce::OwnedArray<TimelineModel>& groups, double ratio)
    {
        for (auto* group : groups)
        {
            for (auto& clip : group->movement.clips)
            {
                clip.startPointGroup.setXYZ(clip.startPointGroup.getX() * ratio,
                                            clip.startPointGroup.getY() * ratio,
                                            clip.startPointGroup.getZ() * ratio);
                clip.targetPointGroup.setXYZ(clip.targetPointGroup.getX() * ratio,
                                             clip.targetPointGroup.getY() * ratio,
                                             clip.targetPointGroup.getZ() * ratio);
                clip.radiusChange *= ratio; // Spiral/Helix: absolute radius change per round
                clip.heightRise *= ratio;   // Helix: total Z travel over the clip

                // Spline/Polygon path points. tension/freqRatioA/freqRatioB/phaseDeg/randomSeed are
                // deliberately NOT scaled - they're dimensionless, like the action parameters above.
                for (auto& wp : clip.waypoints)
                {
                    wp.x *= ratio;
                    wp.y *= ratio;
                    wp.z *= ratio;
                }
            }
        }
    }
}

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
    timelineViewport->getTimelineComponent()->setDisplayTimeInSeconds(pAnimatorEngine->getDisplayTimeInSeconds());
    
    // Create status bar
    statusBar = std::make_unique<StatusBarComponent>(*this);
    
    // Create command manager
    commandManager = std::make_unique<juce::ApplicationCommandManager>();
    commandManager->registerAllCommandsForTarget(this);

    // The animator's enable state can also be toggled from the plugin's main UI, so follow the
    // engine rather than only updating the toolbar from this window's own button.
    pAnimatorEngine->addChangeListener(this);
    
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

    if (pAnimatorEngine != nullptr)
        pAnimatorEngine->removeChangeListener(this);
    closeImportSceneDialog();
    closePreferencesDialog();

    if (menuBar != nullptr)
    {
        removeChildComponent(menuBar.get());
        menuBar.reset();
    }
    
    menuBarModel.reset();
}

void AnimatorMainView::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source != pAnimatorEngine)
        return;

    // Covers every state the engine broadcasts, not just On/Off - refreshButtonStates() also
    // re-reads auto-follow, and the menu carries a ticked "Animator On" item of its own.
    if (toolbar != nullptr)
    {
        toolbar->refreshButtonStates();
        toolbar->repaint();
    }

    if (commandManager != nullptr)
        commandManager->commandStatusChanged();
}

AnimatorUndoManager* AnimatorMainView::getUndoManager() const
{
    if (timelineViewport == nullptr)
        return nullptr;

    if (auto* tc = timelineViewport->getTimelineComponent())
        return &tc->getUndoManager();

    return nullptr;
}

void AnimatorMainView::pushUndoStep(const juce::String& name)
{
    if (auto* undo = getUndoManager())
        if (timelines != nullptr)
            undo->pushStep(name, *timelines);
}

void AnimatorMainView::performUndo()
{
    applyUndoRedo(true);
}

void AnimatorMainView::performRedo()
{
    applyUndoRedo(false);
}

void AnimatorMainView::applyUndoRedo(bool isUndo)
{
    auto* undo = getUndoManager();
    if (undo == nullptr || timelines == nullptr)
        return;

    const auto name = isUndo ? undo->getUndoName() : undo->getRedoName();

    if (!(isUndo ? undo->undo(*timelines) : undo->redo(*timelines)))
        return;

    // The timelines array has been rebuilt, so anything addressing it by index has to be reset -
    // open clip editors, the selection, the current timeline. TimelineComponent handles its own.
    if (auto* tc = timelineViewport->getTimelineComponent())
        tc->refreshAfterUndoRedo();

    timelineViewport->setTimelines(timelines);
    timelineViewport->repaint();

    if (menuBarModel != nullptr)
        menuBarModel->menuItemsChanged();
    if (commandManager != nullptr)
        commandManager->commandStatusChanged();

    juce::AttributedString msg;
    msg.append((isUndo ? "Undid " : "Redid ") + (name.isNotEmpty() ? name : juce::String("last change")),
               juce::FontOptions(12.0f, juce::Font::bold),
               juce::Colours::lightgreen);
    setStatusMessage(msg);
}

void AnimatorMainView::setTimelines(juce::OwnedArray<TimelineModel>* newTimelines)
{
    // A different scene is being shown, so the existing history describes clips that are no longer
    // here. Deliberately only on THIS setTimelines, not TimelineComponent's - that one is also
    // called by applyUndoRedo(), which must not wipe the history it is walking.
    if (newTimelines != timelines)
        if (auto* undo = getUndoManager())
            undo->clear();

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

void AnimatorMainView::setZoomSettings(ZoomSettings* pZoom)
{
    timelineViewport->getTimelineComponent()->setZoomSettings(pZoom);
}

void AnimatorMainView::setPlayheadPosition(ms_t timeMs)
{
    timelineViewport->getTimelineComponent()->setPlayheadPosition(timeMs);
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

                // Import: no submenu - goes straight to the file chooser, then asks what to do with it
                menu.addItem(100, "Import");

                // Export submenu: one entry per existing group, plus "All Groups"
                juce::PopupMenu exportSubMenu;
                if (owner->timelines != nullptr && !owner->timelines->isEmpty())
                {
                    for (int i = 0; i < owner->timelines->size(); ++i)
                    {
                        juce::String timelineName = "Group " + juce::String(i + 1);
                        exportSubMenu.addItem(200 + i, timelineName);
                    }
                    exportSubMenu.addSeparator();
                    exportSubMenu.addItem(199, "All Groups");
                }
                else
                {
                    exportSubMenu.addItem(1, "No timelines available", false);
                }
                menu.addSubMenu("Export", exportSubMenu);

                menu.addSeparator();
                menu.addItem(150, "Load Demo");

                menu.addSeparator();
                menu.addItem(6, "Preferences");
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
        case 100: // Import
            importScene();
            break;

        case 150: // Load Demo
            confirmLoadDemo();
            break;

        case 6: // Preferences
            showPreferencesDialog();
            break;

        case 10: // Undo
            performUndo();
            break;

        case 11: // Redo
            performRedo();
            break;

        case 199: // Export - All Groups
            exportAllScenes();
            break;

        default:
            if (menuItemID >= 200 && menuItemID < 300)
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

static bool readTimelinesFromXml(const juce::XmlElement& xml, juce::OwnedArray<TimelineModel>& outTimelines,
                                 bool& outHasScaler, double& outScaler)
{
    outHasScaler = xml.hasAttribute(xmlAttributeDistanceScaler);
    outScaler = xml.getDoubleAttribute(xmlAttributeDistanceScaler, ScalingInfo::Infinite);

    if (xml.hasTagName("Timeline"))
    {
        auto tm = std::make_unique<TimelineModel>();
        if (!tm->fromXml(xml))
            return false;

        outTimelines.add(tm.release());
        return true;
    }

    if (xml.hasTagName("AnimatorTimelines"))
    {
        for (auto* xTimeline : xml.getChildWithTagNameIterator("Timeline"))
        {
            auto tm = std::make_unique<TimelineModel>();
            if (tm->fromXml(*xTimeline))
                outTimelines.add(tm.release());
        }
        return !outTimelines.isEmpty();
    }

    return false;
}

void AnimatorMainView::importScene()
{
    juce::FileChooser chooser("Import Scene...",
                             UiState::startingFile(UiState::Folders::animatorScene, {},
                                                   juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)),
                             "*.xml");

    if (!chooser.browseForFileToOpen())
        return;

    auto file = chooser.getResult();
    UiState::rememberFolder(UiState::Folders::animatorScene, file);
    auto xml = juce::XmlDocument::parse(file);

    juce::OwnedArray<TimelineModel> importedGroups;
    bool fileHasScaler = false;
    double fileScaler = ScalingInfo::Infinite;
    if (xml == nullptr || !readTimelinesFromXml(*xml, importedGroups, fileHasScaler, fileScaler))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Import Error",
                                              "Invalid scene file format.");
        return;
    }

    const double currentScaler = getCurrentDistanceScaler(pSourceSet);
    const bool fileIsInfinite = juce::exactlyEqual(fileScaler, ScalingInfo::Infinite);
    const bool currentIsInfinite = juce::exactlyEqual(currentScaler, ScalingInfo::Infinite);

    // Only offer a choice when we actually know the file's original scaler, it differs from the
    // current project's, and both are finite - there's no meaningful size ratio to offer
    // otherwise (an unrecorded/older file, or infinite scaling on either side).
    const bool offerRescale = fileHasScaler && !fileIsInfinite && !currentIsInfinite
                            && !juce::exactlyEqual(fileScaler, currentScaler);

    if (offerRescale)
        confirmRescaleOnImport(std::move(importedGroups), fileScaler, currentScaler);
    else
        showImportOptionsDialog(std::move(importedGroups));
}

void AnimatorMainView::confirmRescaleOnImport(juce::OwnedArray<TimelineModel>&& importedGroups, double fileScaler, double currentScaler)
{
    auto sharedGroups = std::make_shared<juce::OwnedArray<TimelineModel>>(std::move(importedGroups));

    // Non-blocking for the same reason as the other confirmation dialogs in this file: a callback
    // form returns immediately instead of running a nested modal loop.
    juce::Component::SafePointer<AnimatorMainView> safeThis(this);

    juce::String message = "This scene was exported with a different global scaling factor than "
        "the current project (file: " + juce::String(fileScaler, 2) + ", current: " + juce::String(currentScaler, 2) + ").\n\n"
        "Keep the original coordinates, or rescale them to match the current project's size?";

    juce::AlertWindow::showOkCancelBox(juce::AlertWindow::QuestionIcon,
                                       "Different Scaling Factor",
                                       message,
                                       "Rescale",
                                       "Keep Original",
                                       this,
                                       juce::ModalCallbackFunction::create([safeThis, sharedGroups, fileScaler, currentScaler](int result)
                                       {
                                           if (safeThis == nullptr)
                                               return;

                                           if (result != 0) // "Rescale" clicked
                                               rescaleMovementClips(*sharedGroups, currentScaler / fileScaler);

                                           safeThis->showImportOptionsDialog(std::move(*sharedGroups));
                                       }));
}

void AnimatorMainView::closeImportSceneDialog()
{
    if (importSceneWindow != nullptr)
    {
        auto* w = importSceneWindow;
        importSceneWindow = nullptr;
        delete w;
    }
}

void AnimatorMainView::showImportOptionsDialog(juce::OwnedArray<TimelineModel>&& importedGroups)
{
    closeImportSceneDialog();

    auto sharedGroups = std::make_shared<juce::OwnedArray<TimelineModel>>(std::move(importedGroups));
    const int importedCount = sharedGroups->size();
    const int existingCount = (timelines != nullptr) ? timelines->size() : 0;
    const bool cursorIsSet = timelineViewport->getTimelineComponent()->isCursorSet();

    // These callbacks already run on a fresh call stack (deferred via MessageManager::callAsync
    // by ImportSceneOptionsComponent/ImportSceneDialog), but AnimatorMainView could still have
    // been closed in the meantime, so guard "this" the same way TimelineDialog does.
    juce::Component::SafePointer<AnimatorMainView> safeThis(this);

    auto content = std::make_unique<ImportSceneOptionsComponent>(importedCount, existingCount, cursorIsSet,
        [safeThis, sharedGroups](bool confirmed, ImportSceneResult result)
        {
            if (safeThis == nullptr)
                return;

            safeThis->closeImportSceneDialog();

            if (confirmed)
                safeThis->applyImportedGroups(*sharedGroups, result);
        });

    importSceneWindow = new ImportSceneDialog(std::move(content), [safeThis]
    {
        if (safeThis != nullptr)
            safeThis->closeImportSceneDialog();
    });

    importSceneWindow->setVisible(true);
}

void AnimatorMainView::applyImportedGroups(const juce::OwnedArray<TimelineModel>& importedGroups, const ImportSceneResult& result)
{
    if (timelines == nullptr)
        timelines = new juce::OwnedArray<TimelineModel>();

    auto* timelineComp = timelineViewport->getTimelineComponent();

    pushUndoStep("Import Scene");

    // insertTimelineAtCursor() pushes a step of its own; without suppressing it here, importing N
    // groups would leave N+1 entries in the history instead of the single "Import Scene" above.
    std::unique_ptr<AnimatorUndoManager::ScopedSuppressor> undoSuppressor;
    if (auto* undo = getUndoManager())
        undoSuppressor = std::make_unique<AnimatorUndoManager::ScopedSuppressor>(*undo);

    for (int i = 0; i < importedGroups.size(); ++i)
    {
        const auto& decision = result.groupDecisions[i];

        switch (decision.mode)
        {
            case ImportSceneMode::Ignore:
                break;

            case ImportSceneMode::AppendAsNew:
                timelines->add(new TimelineModel(*importedGroups[i]));
                break;

            case ImportSceneMode::AppendAtCursor:
            {
                // Insert into a freshly-added empty group, so the same cursor-offset logic used
                // for existing targets shifts these clips too, instead of keeping their raw
                // from-the-file timing (which starts at/near 0).
                timelines->add(new TimelineModel());
                const int newIndex = timelines->size() - 1;
                timelineComp->insertTimelineAtCursor(newIndex, *importedGroups[i]);
                break;
            }

            case ImportSceneMode::Replace:
                if (decision.targetIndex >= 0 && decision.targetIndex < timelines->size())
                    timelines->set(decision.targetIndex, new TimelineModel(*importedGroups[i]), true);
                break;

            case ImportSceneMode::InsertAtCursor:
                if (decision.targetIndex >= 0 && decision.targetIndex < timelines->size())
                    timelineComp->insertTimelineAtCursor(decision.targetIndex, *importedGroups[i]);
                break;
        }
    }

    timelineViewport->setTimelines(timelines);
    timelineViewport->repaint();

    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                          "Import Successful",
                                          "Scene data imported successfully.");
}

void AnimatorMainView::closePreferencesDialog()
{
    if (preferencesWindow != nullptr)
    {
        auto* w = preferencesWindow;
        preferencesWindow = nullptr;
        delete w;
    }
}

void AnimatorMainView::showPreferencesDialog()
{
    closePreferencesDialog();

    juce::Component::SafePointer<AnimatorMainView> safeThis(this);
    const bool initialDisplayInSeconds = timelineViewport->getTimelineComponent()->isDisplayTimeInSeconds();

    auto content = std::make_unique<PreferencesOptionsComponent>(initialDisplayInSeconds,
        [safeThis](bool displayInSeconds)
        {
            if (safeThis != nullptr)
                safeThis->setDisplayTimeInSeconds(displayInSeconds);
        },
        [safeThis]
        {
            if (safeThis != nullptr)
                safeThis->closePreferencesDialog();
        });

    preferencesWindow = new PreferencesDialog(std::move(content), [safeThis]
    {
        if (safeThis != nullptr)
            safeThis->closePreferencesDialog();
    });

    preferencesWindow->setVisible(true);
}

void AnimatorMainView::setDisplayTimeInSeconds(bool useSeconds)
{
    // Persisted like the rest of AnimatorSettings (per plugin instance, via EncoderSettings), and
    // mirrored into TimelineComponent's own cached copy so it can reformat immediately without
    // going through the engine.
    if (pAnimatorEngine != nullptr)
        pAnimatorEngine->setDisplayTimeInSeconds(useSeconds);

    timelineViewport->getTimelineComponent()->setDisplayTimeInSeconds(useSeconds);
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
                             UiState::startingFile(UiState::Folders::animatorScene, {},
                                                   juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)),
                             "*.xml");

    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension("xml");
        UiState::rememberFolder(UiState::Folders::animatorScene, file);
        auto xml = timelineToExport->toXml();
        if (xml != nullptr)
            xml->setAttribute(xmlAttributeDistanceScaler, getCurrentDistanceScaler(pSourceSet));

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

void AnimatorMainView::exportAllScenes()
{
    if (timelines == nullptr || timelines->isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                              "Export Error",
                                              "No timeline data to export.");
        return;
    }

    juce::FileChooser chooser("Export All Groups...",
                             UiState::startingFile(UiState::Folders::animatorScene, {},
                                                   juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)),
                             "*.xml");

    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension("xml");
        UiState::rememberFolder(UiState::Folders::animatorScene, file);

        juce::XmlElement root("AnimatorTimelines");
        root.setAttribute(xmlAttributeDistanceScaler, getCurrentDistanceScaler(pSourceSet));
        for (auto* tm : *timelines)
            root.addChildElement(tm->toXml().release());

        if (root.writeTo(file))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                  "Export Successful",
                                                  "All groups exported successfully to: " + file.getFullPathName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Export Error",
                                                  "Failed to export scene file.");
        }
    }
}

static inline MovementClip makeDemoMovementClip(juce::String id, ms_t start, ms_t length, juce::Colour col,
                                                 MovementType type, bool useStart,
                                                 Point3D<double> startPt, Point3D<double> targetPt,
                                                 double count = 1.0, double radiusChange = 0.0)
{
    MovementClip c;
    c.id = std::move(id);
    c.start = start;
    c.length = length;
    c.colour = col;
    c.movementType = type;
    c.useStartPoint = useStart;
    c.startPointGroup = startPt;
    c.targetPointGroup = targetPt;
    c.count = count;
    c.radiusChange = radiusChange;
    return c;
}

static inline ActionClip makeDemoActionClip(juce::String id, ms_t start, ms_t length, juce::Colour col)
{
    ActionClip c;
    c.id = std::move(id);
    c.start = start;
    c.length = length;
    c.colour = col;
    return c;
}

static inline ActionClip makeDemoActionClip(juce::String id, ms_t start, ms_t length, juce::Colour col, ActionDefinition action)
{
    ActionClip c = makeDemoActionClip(std::move(id), start, length, col);
    c.actions.add(action);
    return c;
}

void AnimatorMainView::confirmLoadDemo()
{
    // Non-blocking: showOkCancelBox with a callback returns immediately rather than running a
    // nested modal loop, so it can't stall audio processing while the user decides.
    juce::Component::SafePointer<AnimatorMainView> safeThis(this);

    juce::AlertWindow::showOkCancelBox(juce::AlertWindow::WarningIcon,
                                       "Load Demo",
                                       "This will replace all existing timeline data with demo content. "
                                       "This action cannot be undone. Continue?",
                                       "Load Demo",
                                       "Cancel",
                                       this,
                                       juce::ModalCallbackFunction::create([safeThis](int result)
                                       {
                                           if (safeThis != nullptr && result != 0)
                                               safeThis->loadDemoContent();
                                       }));
}

void AnimatorMainView::loadDemoContent()
{
    if (timelines == nullptr)
        timelines = new juce::OwnedArray<TimelineModel>();

    pushUndoStep("Load Demo Scene");

    timelines->clear(true);

    // Reproducibility: the FIRST movement clip and the FIRST stretch action of each timeline pin
    // their own starting state explicitly (useStartPoint / useStartValue with a real value), which
    // is what the rest of the chain hangs off. Later clips deliberately keep starting from the live
    // position/stretch - that's both how the demo shows off the "undefined start" mode and what
    // keeps it malleable, since editing one clip's target then flows into the next instead of
    // leaving a jump.
    //
    // So the demo replays identically when played from the beginning. It can still diverge if you
    // scrub straight into the middle of a timeline, or mute an upstream clip, because the chained
    // clips then capture whatever the group happens to be at. Pin the individual clip if you need
    // that case to be exact too.
    //
    // ROTATION pins the same way, via useStartValue - the start value is an absolute angle the group
    // snaps to when the clip starts (AnimatorMath::computeClipRotation). Note that a start angle set
    // on any one axis makes every axis of that clip absolute, with unset axes starting at 0 degrees,
    // so the first rotation clip of a timeline pins all the axes it actually uses.

    // Timeline 1: Linear Path - straight-line moves plus a full rotation and a grow/shrink
    {
        auto* t = new TimelineModel();

        t->movement.clips.add(makeDemoMovementClip("Move Right", 0, 1200, juce::Colours::orange,
            MovementType::MoveToCartesian, true, Point3D<double>(0.0, 0.0, 0.0), Point3D<double>(4.0, 0.0, 0.0)));
        t->movement.clips.add(makeDemoMovementClip("Move Left", 1400, 1400, juce::Colours::orangered,
            MovementType::MoveToCartesian, false, Point3D<double>(), Point3D<double>(-4.0, 0.0, 0.0)));
        t->movement.clips.add(makeDemoMovementClip("Return Center", 3000, 1000, juce::Colours::goldenrod,
            MovementType::MoveToCartesian, false, Point3D<double>(), Point3D<double>(0.0, 0.0, 0.0)));

        // 360 degrees over 2s at 180 deg/s, from an explicit 0 degree start orientation
        t->actions.clips.add(makeDemoActionClip("Rotate Full Circle", 0, 2000, juce::Colours::slateblue,
            ActionDefinition{ActionType::RotationZ, TimingType::ConstantPerSecond, 180.0, 0.0, true}));
        // doubles in size by the end of the clip, from an explicit baseline of 1.0
        t->actions.clips.add(makeDemoActionClip("Grow", 2200, 1000, juce::Colours::mediumseagreen,
            ActionDefinition{ActionType::Stretch, TimingType::RelativeDuringClip, 1.0, 1.0, true}));
        // halves whatever the Grow clip above left behind, i.e. back to the original size
        t->actions.clips.add(makeDemoActionClip("Shrink", 3300, 900, juce::Colours::seagreen,
            ActionDefinition{ActionType::Stretch, TimingType::RelativeDuringClip, -0.5}));

        timelines->add(t);
    }

    // Timeline 2: Orbit & Spiral - showcases the Circle and Spiral movement types
    {
        auto* t = new TimelineModel();

        // one full revolution, radius 3, around the origin
        t->movement.clips.add(makeDemoMovementClip("Circle Orbit", 0, 3000, juce::Colours::cornflowerblue,
            MovementType::Circle, true, Point3D<double>(3.0, 0.0, 0.0), Point3D<double>(0.0, 0.0, 0.0), 1.0));
        // continues from wherever the orbit ended, spiraling outward over 2 rounds
        t->movement.clips.add(makeDemoMovementClip("Spiral Outward", 3200, 3000, juce::Colours::royalblue,
            MovementType::Spiral, false, Point3D<double>(), Point3D<double>(0.0, 0.0, 0.0), 2.0, 1.0));

        // tilts up by 45 degrees over the clip, from an explicit 0 degree start orientation
        t->actions.clips.add(makeDemoActionClip("Tilt Up", 0, 3000, juce::Colours::mediumseagreen,
            ActionDefinition{ActionType::RotationX, TimingType::RelativeDuringClip, 45.0, 0.0, true}));
        // grows at a constant rate of 0.3/s starting from a baseline of 1.0 (demonstrates a start value with
        // Constant per Second timing)
        t->actions.clips.add(makeDemoActionClip("Grow While Spiraling", 3200, 3000, juce::Colours::seagreen,
            ActionDefinition{ActionType::Stretch, TimingType::ConstantPerSecond, 0.3, 1.0, true}));

        timelines->add(t);
    }

    // Timeline 3: Timing Types - contrasts Absolute Target vs Relative During Clip timing, and a polar
    // (arcing) move vs a straight-line Cartesian move
    {
        auto* t = new TimelineModel();

        // arcs from the front position to the right, moving through polar (azimuth/elevation/distance) space
        t->movement.clips.add(makeDemoMovementClip("Arc Move (Polar)", 0, 2000, juce::Colours::purple,
            MovementType::MoveToPolar, true, Point3D<double>(0.0, 3.0, 0.0), Point3D<double>(3.0, 0.0, 0.0)));
        // returns to the front position in a straight line
        t->movement.clips.add(makeDemoMovementClip("Return (Cartesian)", 2200, 1500, juce::Colours::darkorchid,
            MovementType::MoveToCartesian, false, Point3D<double>(), Point3D<double>(0.0, 3.0, 0.0)));

        // rotates from an explicit 0 to an absolute 90 degree orientation by the end of the clip
        t->actions.clips.add(makeDemoActionClip("Rotate To 90 (Absolute)", 0, 2000, juce::Colours::gold,
            ActionDefinition{ActionType::RotationY, TimingType::AbsoluteTarget, 90.0, 0.0, true}));
        // rotates by a further 45 degrees relative to wherever it started the clip
        t->actions.clips.add(makeDemoActionClip("Rotate 45 More (Relative)", 2200, 1500, juce::Colours::darkkhaki,
            ActionDefinition{ActionType::RotationY, TimingType::RelativeDuringClip, 45.0}));
        // grows at a constant rate of 0.4/s from an explicit baseline of 1.0 (first stretch of this
        // timeline, so it pins the baseline the rest of the timeline builds on)
        t->actions.clips.add(makeDemoActionClip("Stretch (Constant/s)", 3800, 1500, juce::Colours::lightblue,
            ActionDefinition{ActionType::Stretch, TimingType::ConstantPerSecond, 0.4, 1.0, true}));

        timelines->add(t);
    }

    // Timeline 4: Complex Example - a Spiral move combined with a two-axis rotation and a stretch
    {
        auto* t = new TimelineModel();

        // spirals inward toward the origin while completing 1.5 rounds
        t->movement.clips.add(makeDemoMovementClip("Complex Spiral", 0, 3000, juce::Colours::teal,
            MovementType::Spiral, true, Point3D<double>(2.0, 0.0, 0.0), Point3D<double>(0.0, 0.0, 0.0), 1.5, -0.5));

        // First rotation of this timeline, so both axes pin their own start angle. Pinning either one
        // would already make the whole clip absolute (and leave the other starting from 0 anyway) -
        // spelling out both is what makes that intentional rather than incidental.
        ActionClip rotationClip = makeDemoActionClip("3D Rotation", 500, 1000, juce::Colours::orange);
        rotationClip.actions.add(ActionDefinition{ActionType::RotationX, TimingType::AbsoluteTarget, 45.0, 0.0, true});
        rotationClip.actions.add(ActionDefinition{ActionType::RotationY, TimingType::RelativeDuringClip, 90.0, 0.0, true});
        t->actions.clips.add(rotationClip);

        // first stretch of this timeline, so it pins its own baseline rather than inheriting one
        ActionClip stretchClip = makeDemoActionClip("Dynamic Stretch", 1600, 800, juce::Colours::red);
        stretchClip.actions.add(ActionDefinition{ActionType::Stretch, TimingType::ConstantPerSecond, 2.0, 1.0, true});
        t->actions.clips.add(stretchClip);

        timelines->add(t);
    }

    timelineViewport->setTimelines(timelines);
    timelineViewport->repaint();

    juce::AttributedString msg;
    msg.append("Demo content loaded",
               juce::FontOptions(12.0f, juce::Font::bold),
               juce::Colours::lightgreen);
    setStatusMessage(msg);
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
        toolbar->repaint();
    }

    // Update command manager to refresh menu checkmarks
    if (commandManager != nullptr)
    {
        commandManager->commandStatusChanged();
    }
}

void AnimatorMainView::toggleOnOff()
{
    bool newState = !pAnimatorEngine->getAnimatorState();
    pAnimatorEngine->setAnimatorState(newState);

    // Update toolbar button states
    if (toolbar != nullptr)
    {
        toolbar->refreshButtonStates();
        toolbar->repaint();
    }

    // Update command manager to refresh menu states
    if (commandManager != nullptr)
    {
        commandManager->commandStatusChanged();
    }
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
    SvgHelper::loadSVGIcon(deleteButton.get(), BinaryData::trash_icon_svg, BinaryData::trash_icon_svgSize, "Delete Selected Clips");
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
        {
            // Naming the step ("Undo Move Clip") makes it obvious what is about to be reversed,
            // which matters when the history spans several different kinds of edit.
            const auto name = getUndoManager() != nullptr ? getUndoManager()->getUndoName() : juce::String();
            result.setInfo(name.isNotEmpty() ? "Undo " + name : "Undo", "Undo last manipulation", "Edit", 0);
            result.addDefaultKeypress('Z', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(getUndoManager() != nullptr && getUndoManager()->canUndo());
            break;
        }
            
        case CMD_redo:
        {
            const auto name = getUndoManager() != nullptr ? getUndoManager()->getRedoName() : juce::String();
            result.setInfo(name.isNotEmpty() ? "Redo " + name : "Redo", "Redo last undone manipulation", "Edit", 0);
            result.addDefaultKeypress('Y', isMac ? juce::ModifierKeys::commandModifier : juce::ModifierKeys::ctrlModifier);
            result.setActive(getUndoManager() != nullptr && getUndoManager()->canRedo());
            break;
        }
            
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
            performUndo();
            return true;
        case CMD_redo:
            performRedo();
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

    pushUndoStep("Add Timeline");

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
                                               pushUndoStep("Remove Timelines");

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
                                               pushUndoStep("Remove Timeline");
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
