#include "MovementClipEditor.h"
#include "TimelineComponent.h"
#include "CommonClipSettings.h"
#include "ClipEditorDialog.h"
#include "../../Common/UiState.h"

MovementClipEditor::MovementClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx)
    : timelineComp(timeline), timelineIndex(timelineIdx), clipIndex(clipIdx)
{
    if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
    {
        if (clipIndex >= 0 && clipIndex < timelineModel->movement.clips.size())
        {
            currentClip = timelineModel->movement.clips.getReference(clipIndex);
        }
    }
    
    pSourceSet = timelineComp.getSources();
    pPointSelection = timelineComp.getPointSelection();
    if(pPointSelection != nullptr)
    {
        pPointSelection->addChangeListener(this);
    }

    createControls();
    startTimer(150);
}

MovementClipEditor::~MovementClipEditor()
{
    stopTimer();

    if (pPointSelection != nullptr)
    {
        pPointSelection->removeChangeListener(this);
    }
}

void MovementClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    // Landscape: Clip Properties | Preview | Movement Properties, side by side, all the same fixed
    // height whatever the movement type. Stacking Movement Properties underneath made the dialog
    // ~980px tall for a Spiral, and sizing it per type made it jump every time the type changed.
    auto clipGroupArea = area.removeFromLeft(getClipPropertiesWidth());
    clipGroup.setBounds(clipGroupArea);
    commonSettings.setBounds(clipGroupArea.reduced(8, 20));

    area.removeFromLeft(8); // spacing between columns

    auto previewArea = area.removeFromLeft(getPreviewWidth());
    previewGroup.setBounds(previewArea);

    // The view toggle sits under the radars, inside the Preview group it belongs to.
    auto previewContent = previewArea.reduced(8, 20);
    auto previewFlagRow = previewContent.removeFromBottom(24);
    realWorldInfo.setBounds(previewFlagRow.removeFromRight(22).reduced(1, 2));
    previewFlagRow.removeFromRight(4);
    realWorldToggle.setBounds(previewFlagRow);
    preview.setBounds(previewContent);

    area.removeFromLeft(8);

    // Movement Properties stops short of the other two columns, and Apply/Cancel occupy the gap it
    // leaves - the buttons sit inside the layout rather than in a band of their own below everything.
    auto movementColumn = area;
    auto movementArea = movementColumn.removeFromTop(movementColumn.getHeight() - getButtonInsetHeight());
    movementGroup.setBounds(movementArea);
    layoutMovementControls(movementArea.reduced(8, 20));

    movementColumn.removeFromTop(8); // gap between the group box and the buttons

    cancelButton.setBounds(movementColumn.removeFromRight(80));
    movementColumn.removeFromRight(8); // Button spacing
    applyButton.setBounds(movementColumn.removeFromRight(80));
}

void MovementClipEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

int MovementClipEditor::getTotalRequiredHeight() const
{
    // Deliberately a constant, not derived from the current movement type: the dialog must not
    // resize when the type combo changes. getMovementControlsHeight() is still the authority on
    // whether a type FITS - see the assertion in layoutMovementControls().
    //
    // No separate button row: Apply/Cancel live in the gap under the (shorter) Movement Properties
    // column, so the columns' own height is the whole content height.
    return 10 * 2 + getFixedColumnHeight();
}

int MovementClipEditor::getTotalRequiredWidth() const
{
    return 10 * 2 + getClipPropertiesWidth() + 8 + getPreviewWidth() + 8 + getMovementPropertiesWidth();
}

bool MovementClipEditor::applyChanges()
{
    if (!commonSettings.validate())
        return false;

    // Point3D's operator= only accepts a non-const lvalue (a pre-existing quirk, not something
    // introduced here), so assigning directly from the temporary buildClipFromControls() returns
    // won't compile - go through a named local first.
    MovementClip built = buildClipFromControls();
    currentClip = built;

    if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
    {
        if (clipIndex >= 0 && clipIndex < timelineModel->movement.clips.size())
        {
            // One undo step per Apply - but only when the stored clip genuinely differs, so pressing
            // Apply twice, or applying after an edit that was typed and then reverted, doesn't bury
            // the real change behind no-op steps.
            if (!(timelineModel->movement.clips.getReference(clipIndex) == currentClip))
                timelineComp.pushUndoStep("Edit Movement Clip");

            timelineModel->movement.clips.getReference(clipIndex) = currentClip;
            return true;
        }
    }

    return false;
}

bool MovementClipEditor::isDirty()
{
    // Point3D's operator= only accepts a non-const lvalue (the same pre-existing quirk noted in
    // applyChanges()), so comparing directly against the temporary buildClipFromControls() return
    // won't compile - go through a named local first.
    MovementClip built = buildClipFromControls();
    return built != dirtyBaseline;
}

bool MovementClipEditor::confirmDiscardIfDirty()
{
    if (!isDirty())
        return true;

    // showYesNoCancelBox returns 1 for the first button, 2 for the second, 0 for the third
    // (or if the box is dismissed) - mapped here to Save & Close / Discard / Keep Editing.
    const int result = juce::AlertWindow::showYesNoCancelBox(juce::AlertWindow::WarningIcon,
        "Unsaved Changes", "This clip has unsaved changes.",
        "Save & Close", "Discard", "Keep Editing");

    if (result == 1) // Save & Close
    {
        if (!applyChanges())
            return false; // validation failed (e.g. invalid Start/Duration/End) - stay open, same as the Apply button would

        timelineComp.repaint();
        return true;
    }

    if (result == 2) // Discard
        return true;

    return false; // Keep Editing, or the box was dismissed
}

MovementClip MovementClipEditor::buildClipFromControls()
{
    MovementClip clip = currentClip;

    commonSettings.applyToClip(clip);

    clip.useStartPoint = useStartPosition.getToggleState();
    clip.movementType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);

    clip.count = countSlider.getValue();
    clip.radiusChange = radiusChangeSlider.getValue();
    clip.tension = tensionSlider.getValue();
    clip.closedPath = closedPathToggle.getToggleState();
    clip.heightRise = heightRiseSlider.getValue();
    clip.freqRatioA = freqASlider.getValue();
    clip.freqRatioB = freqBSlider.getValue();
    clip.phaseDeg = phaseSlider.getValue();
    clip.randomSeed = (int)randomSeedSlider.getValue();
    // waypoints are carried over from currentClip (which the table and the preview's drag handles
    // mutate live), so nothing to copy from a control here.

    // Convert coordinates based on display mode for storage
    if (usePolarDisplay.getToggleState())
    {
        // Convert from degrees (UI) to radians (storage)
        double startAzimuthRad = Constants::GradToRad(startXSlider.getPreciseValue());
        double startElevationRad = Constants::GradToRad(startYSlider.getPreciseValue());
        double startDistance = startZSlider.getPreciseValue();

        double targetAzimuthRad = Constants::GradToRad(targetXSlider.getPreciseValue());
        double targetElevationRad = Constants::GradToRad(targetYSlider.getPreciseValue());
        double targetDistance = targetZSlider.getPreciseValue();

        clip.startPointGroup.setAed(startAzimuthRad, startElevationRad, startDistance);
        clip.targetPointGroup.setAed(targetAzimuthRad, targetElevationRad, targetDistance);
    }
    else
    {
        clip.startPointGroup.setXYZ(startXSlider.getPreciseValue(), startYSlider.getPreciseValue(), startZSlider.getPreciseValue());
        clip.targetPointGroup.setXYZ(targetXSlider.getPreciseValue(), targetYSlider.getPreciseValue(), targetZSlider.getPreciseValue());
    }

    return clip;
}

void MovementClipEditor::createControls()
{
    setOpaque(true);
    
    addAndMakeVisible(clipGroup);
    addAndMakeVisible(movementGroup);
    addAndMakeVisible(previewGroup);

    addAndMakeVisible(commonSettings);
    commonSettings.setDisplayInSeconds(timelineComp.isDisplayTimeInSeconds());
    commonSettings.setClipData(currentClip);
    // See ActionClipEditor: the curve needs sub-poll feedback while being dragged.
    commonSettings.onCurveEdited = [this] { preview.setMovementClip(buildClipFromControls()); };
    refreshPalindromeConstraint();

    addAndMakeVisible(preview);
    preview.setScalingInfo(pSourceSet != nullptr ? pSourceSet->getScalingInfo() : nullptr);
    preview.setZoomSettings(timelineComp.getZoomSettings());

    addAndMakeVisible(realWorldToggle);
    realWorldToggle.setButtonText("Real-world scale");
    realWorldToggle.onClick = [this] { preview.setRealWorldView(realWorldToggle.getToggleState()); };
    addAndMakeVisible(realWorldInfo);

    // Nothing to scale to without either a finite distance scaler or a main-radar zoom.
    realWorldToggle.setEnabled(preview.getRealWorldRadius() > 0.0);

    preview.setMovementClip(currentClip);
    
    addAndMakeVisible(applyButton);
    applyButton.onClick = [this] {
        if (applyChanges())
        {
            timelineComp.repaint();
            // Send action message to close the window
            if (auto* broadcaster = findParentComponentOfClass<juce::ActionBroadcaster>())
                broadcaster->sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
        }
    };
    
    addAndMakeVisible(cancelButton);
    cancelButton.onClick = [this] {
        if (!confirmDiscardIfDirty())
            return;

        // Send action message to close the window
        if (auto* broadcaster = findParentComponentOfClass<juce::ActionBroadcaster>())
            broadcaster->sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
    };
    
    // Movement type combo box
    addAndMakeVisible(movementTypeLabel);
    movementTypeLabel.setText("Movement Type:", juce::dontSendNotification);
    movementTypeLabel.setJustificationType(juce::Justification::centredLeft);
    
    addAndMakeVisible(movementTypeCombo);
    // Driven from movementTypeToString() so the combo, the timeline tooltip and the enum can't
    // drift apart as types are added.
    for (int i = 0; i <= static_cast<int>(MovementType::RandomWalk); ++i)
        movementTypeCombo.addItem(movementTypeToString(static_cast<MovementType>(i)), i + 1);
    movementTypeCombo.setSelectedId(static_cast<int>(currentClip.movementType) + 1);
    movementTypeCombo.onChange = [this] { onMovementTypeChanged(); };
    
    // Coordinate display toggle (editor only - doesn't affect stored data)
    addAndMakeVisible(usePolarDisplay);
    usePolarDisplay.setButtonText("Show Polar Coordinates (AED)");
    // Default to polar display for polar movement types, Cartesian for others
    bool defaultPolarDisplay = currentClip.movementType == MovementType::MoveToPolar;
    usePolarDisplay.setToggleState(defaultPolarDisplay, juce::dontSendNotification);
    usePolarDisplay.onClick = [this] { updateCoordinateSystem(); };

    addAndMakeVisible(closedPathToggle);
    closedPathToggle.setButtonText("Closed path");
    closedPathToggle.setToggleState(currentClip.closedPath, juce::dontSendNotification);
    closedPathToggle.onClick = [this]
    {
        // Closing the path removes the jump at a repeat boundary, so the Palindrome lock has to be
        // re-evaluated as soon as it changes.
        refreshPalindromeConstraint();
        preview.setMovementClip(buildClipFromControls());
    };
    addAndMakeVisible(closedPathInfo);
    
    addAndMakeVisible(useStartPosition);
    useStartPosition.setButtonText("Use Defined Start Position");
    useStartPosition.setToggleState(currentClip.useStartPoint, juce::dontSendNotification);
    useStartPosition.onClick = [this] { updateControlVisibility(); };
    
    // Create coordinate sliders
    createCoordinateSlider(startXSlider, startXLabel, "Start X:", -10.0, 10.0, 0.0);
    createCoordinateSlider(startYSlider, startYLabel, "Start Y:", -10.0, 10.0, 0.0);
    createCoordinateSlider(startZSlider, startZLabel, "Start Z:", -10.0, 10.0, 0.0);
    
    createCoordinateSlider(targetXSlider, targetXLabel, "Target X:", -10.0, 10.0, 0.0);
    createCoordinateSlider(targetYSlider, targetYLabel, "Target Y:", -10.0, 10.0, 0.0);
    createCoordinateSlider(targetZSlider, targetZLabel, "Target Z:", -10.0, 10.0, 0.0);
    
    createStandardSlider(countSlider, countLabel, "Rotations:", currentClip.count);
    createStandardSlider(radiusChangeSlider, radiusChangeLabel, "Radius change / rotation:", currentClip.radiusChange);

    // Per-type parameters. Like count/radiusChange these are created once from currentClip and
    // then only ever READ by buildClipFromControls() - deliberately never re-read from currentClip
    // by any update function, which is what would clobber unsaved edits.
    createStandardSlider(tensionSlider, tensionLabel, "Tension:", currentClip.tension);
    addAndMakeVisible(tensionInfo);
    tensionSlider.setRange(0.0, 1.0, 0.01);
    tensionSlider.setValue(currentClip.tension);

    createStandardSlider(heightRiseSlider, heightRiseLabel, "Height rise:", currentClip.heightRise);
    createStandardSlider(freqASlider, freqALabel, "Frequency X:", currentClip.freqRatioA);
    createStandardSlider(freqBSlider, freqBLabel, "Frequency Y:", currentClip.freqRatioB);

    createStandardSlider(phaseSlider, phaseLabel, "Phase:", currentClip.phaseDeg);
    phaseSlider.setRange(-360.0, 360.0, 1.0);
    phaseSlider.setValue(currentClip.phaseDeg);

    createStandardSlider(randomSeedSlider, randomSeedLabel, "Seed:", currentClip.randomSeed);
    randomSeedSlider.setRange(0.0, 99999.0, 1.0);
    randomSeedSlider.setValue(currentClip.randomSeed);

    // Waypoint table (Spline/Polygon)
    waypointModel = std::make_unique<WaypointTableListModel>(currentClip.waypoints, waypointTable);
    waypointModel->setScalingInfo(pSourceSet != nullptr ? pSourceSet->getScalingInfo() : nullptr);
    waypointModel->onWaypointEdited = [this] { preview.setMovementClip(buildClipFromControls()); };

    addAndMakeVisible(waypointTable);
    waypointTable.setModel(waypointModel.get());
    waypointTable.setHeaderHeight(22);
    waypointTable.getHeader().addColumn("#", WaypointTableListModel::ColumnIndex, 34, 34, 34);
    waypointTable.getHeader().addColumn("X", WaypointTableListModel::ColumnX, 80);
    waypointTable.getHeader().addColumn("Y", WaypointTableListModel::ColumnY, 80);
    waypointTable.getHeader().addColumn("Z", WaypointTableListModel::ColumnZ, 80);
    waypointTable.getHeader().addColumn("Break", WaypointTableListModel::ColumnBreak, 54, 54, 54);

    addAndMakeVisible(addWaypointButton);
    addWaypointButton.onClick = [this] { addWaypoint(); };

    addAndMakeVisible(removeWaypointButton);
    removeWaypointButton.onClick = [this] { removeSelectedWaypoint(); };
    
    // Create apply current position buttons
    createApplyCurrentPositionButton(applyCurrentStartButton, startXSlider, startYSlider, startZSlider);
    createApplyCurrentPositionButton(applyCurrentTargetButton, targetXSlider, targetYSlider, targetZSlider);
    
    // Initialize UI
    updateSliderLabelsAndRanges();
    updateControlVisibility();
    updateCurrentPosition();

    // Dragging a Start/Target handle in the preview writes straight into the same sliders a typed
    // edit would, respecting the current Polar/Cartesian display mode - so Apply/Cancel/dirty-
    // detection/buildClipFromControls() all treat a drag exactly like any other slider edit.
    preview.onPointDragged = [this](bool isStartHandle, juce::Vector3D<double> newWorldPos)
    {
        const auto converted = convertCartesianToSelectedSystem(newWorldPos);
        auto& xSlider = isStartHandle ? startXSlider : targetXSlider;
        auto& ySlider = isStartHandle ? startYSlider : targetYSlider;
        auto& zSlider = isStartHandle ? startZSlider : targetZSlider;
        xSlider.setValue(converted.x);
        ySlider.setValue(converted.y);
        zSlider.setValue(converted.z);
        preview.setMovementClip(buildClipFromControls());
    };

    // Waypoint handles mutate currentClip.waypoints directly - unlike Start/Target there's no
    // slider to funnel them through, and the table is a view onto that same array. Each one pushes
    // to the preview immediately rather than waiting for the 150ms poll, so the handle doesn't lag
    // the cursor.
    preview.onWaypointDragged = [this](int index, juce::Vector3D<double> newWorldPos)
    {
        if (index < 0 || index >= currentClip.waypoints.size()) return;

        auto& wp = currentClip.waypoints.getReference(index);
        wp.x = newWorldPos.x;
        wp.y = newWorldPos.y;
        wp.z = newWorldPos.z;

        waypointTable.repaint();
        preview.setMovementClip(buildClipFromControls());
    };

    preview.onWaypointAdded = [this](juce::Vector3D<double> worldPos)
    {
        MovementWaypoint wp;
        wp.x = worldPos.x;
        wp.y = worldPos.y;
        wp.z = worldPos.z;
        currentClip.waypoints.add(wp);

        refreshWaypointTable();
        waypointTable.selectRow(currentClip.waypoints.size() - 1);
    };

    // Double-clicking the path inserts there rather than appending, so the shape is preserved. The
    // new point inherits its neighbour's break flag handling implicitly: it's placed AFTER the span's
    // first point, mid-sub-path, so it never starts a segment of its own.
    preview.onWaypointInserted = [this](int insertIndex, juce::Vector3D<double> worldPos)
    {
        if (insertIndex < 0 || insertIndex > currentClip.waypoints.size()) return;

        MovementWaypoint wp;
        wp.x = worldPos.x;
        wp.y = worldPos.y;
        wp.z = worldPos.z;
        wp.startsNewSegment = false;

        currentClip.waypoints.insert(insertIndex, wp);

        refreshWaypointTable();
        waypointTable.selectRow(insertIndex);
        preview.setMovementClip(buildClipFromControls());
    };

    preview.onWaypointRemoved = [this](int index)
    {
        if (index < 0 || index >= currentClip.waypoints.size()) return;

        currentClip.waypoints.remove(index);

        // Row 0 always begins the first sub-path, so a break flag shuffled up into it is
        // meaningless (and its table cell is disabled, so the user couldn't clear it themselves).
        if (!currentClip.waypoints.isEmpty())
            currentClip.waypoints.getReference(0).startsNewSegment = false;

        refreshWaypointTable();
    };

    // Captured last, after every control above has been populated from currentClip - see
    // dirtyBaseline's own comment on why this must be the slider-quantized build, not currentClip
    // itself. Point3D's operator= only accepts a non-const lvalue, so assigning directly from the
    // temporary buildClipFromControls() return won't compile - go through a named local first.
    MovementClip baseline = buildClipFromControls();
    dirtyBaseline = baseline;
}

void MovementClipEditor::createCoordinateSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name,
                           double min, double max, double defaultValue)
{
    addAndMakeVisible(slider);
    slider.setRange(min, max, 0.01);
    slider.setValue(defaultValue);

    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
}

void MovementClipEditor::createStandardSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name, double defaultValue)
{
    addAndMakeVisible(slider);
    slider.setUpDownStyle(true); // always a compact updown spinner, same footprint as Start/Target X/Y/Z
    slider.setRange(-1000, 1000, 0.1);
    slider.setValue(defaultValue);

    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
}

void MovementClipEditor::createApplyCurrentPositionButton(juce::TextButton& button, CoordinateValueControl& xSlider, CoordinateValueControl& ySlider, CoordinateValueControl& zSlider)
{
    addAndMakeVisible(button);
    button.onClick = [this, &xSlider, &ySlider, &zSlider] {
        if (currentPositionValid)
        {
            auto position = getCurrentPositionInSelectedSystem();
            xSlider.setValue(position.x);
            ySlider.setValue(position.y);
            zSlider.setValue(position.z);
        }
    };
}

void MovementClipEditor::updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid)
{
    button.setButtonText(getCoordinateDisplayText(vector, isValid));
}

juce::String MovementClipEditor::getCoordinateDisplayText(const juce::Vector3D<double>& vector, bool isValid) const
{
    if (!isValid)
    {
        return "Unable to determine current position";
    }
    
    if (usePolarDisplay.getToggleState())
    {
        // Display in polar coordinates (AED)
        return juce::String("Apply ") +
               juce::String(vector.x, 1) + "° (A); " +
               juce::String(vector.y, 1) + "° (E); " +
               juce::String(vector.z, 2) + " (D)";
    }
    else
    {
        // Display in Cartesian coordinates (XYZ)
        return juce::String("Apply ") +
               juce::String(vector.x, 2) + " (X); " +
               juce::String(vector.y, 2) + " (Y); " +
               juce::String(vector.z, 2) + " (Z)";
    }
}

juce::Vector3D<double> MovementClipEditor::getCurrentPositionInSelectedSystem() const
{
    if (!currentPositionValid)
        return juce::Vector3D<double>();

    return convertCartesianToSelectedSystem(currentPosition);
}

// Shared by getCurrentPositionInSelectedSystem() (the live group position, for the "Apply Current
// Position" buttons) and preview.onPointDragged (an arbitrary dragged position) - both need the
// same Cartesian->Polar conversion before writing into the Start/Target sliders.
juce::Vector3D<double> MovementClipEditor::convertCartesianToSelectedSystem(juce::Vector3D<double> cartesian) const
{
    if (usePolarDisplay.getToggleState())
    {
        // Convert Cartesian to polar (AED) and return in degrees for UI
        Point3D<double> point(cartesian.x, cartesian.y, cartesian.z);
        return juce::Vector3D<double>(
            Constants::RadToGrad(point.getAzimuth()),  // Convert to degrees
            Constants::RadToGrad(point.getElevation()), // Convert to degrees
            point.getDistance()
        );
    }
    else
    {
        // Use Cartesian directly
        return cartesian;
    }
}

void MovementClipEditor::updateSliderLabelsAndRanges()
{
    bool usePolar = usePolarDisplay.getToggleState();
    
    if (usePolar)
    {
        // Polar coordinates (Azimuth, Elevation, Distance)
        // juce::String's implicit const char* constructor assumes ASCII and asserts/crashes on
        // bytes >= 128 (which "°" always has in UTF-8) - CharPointer_UTF8 is the safe wrapper.
        startXLabel.setText(juce::String(juce::CharPointer_UTF8("Start Azimuth [°]:")), juce::dontSendNotification);
        startYLabel.setText(juce::String(juce::CharPointer_UTF8("Start Elevation [°]:")), juce::dontSendNotification);
        startZLabel.setText("Start Distance:", juce::dontSendNotification);

        // Update target label based on movement type
        MovementType currentType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
        juce::String targetLabel = "Target ";
        if (movementTypeUsesTargetAsCentre(currentType))
            targetLabel = "Center ";

        // targetLabel is already a juce::String here, so appending the degree symbol via operator+
        // goes through the UTF-8-safe path (unlike constructing a String directly from a raw
        // literal containing it).
        targetXLabel.setText(targetLabel + "Azimuth [°]:", juce::dontSendNotification);
        targetYLabel.setText(targetLabel + "Elevation [°]:", juce::dontSendNotification);
        targetZLabel.setText(targetLabel + "Distance:", juce::dontSendNotification);

        // Azimuth/Elevation are angles - always sliders, always the same fixed range regardless
        // of the distance scaler.
        startXSlider.setUpDownStyle(false);
        startYSlider.setUpDownStyle(false);
        targetXSlider.setUpDownStyle(false);
        targetYSlider.setUpDownStyle(false);

        startXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);
        startYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);
        targetXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);
        targetYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

        // Distance is scaler-dependent: a bounded slider (matching the real scaler, like X/Y/Z
        // in Cartesian mode) when finite, an unbounded updown spinner when infinite - a
        // draggable slider whose range spans an unbounded scaler doesn't give meaningful control.
        // CartesianMax() already substitutes a large-but-finite bound when infinite, so this
        // doesn't need (and must not add) its own separate finite/infinite branch.
        ScalingInfo* scaling = pSourceSet->getScalingInfo();
        bool infinite = scaling != nullptr && scaling->IsInfinite();
        double distanceMax = scaling != nullptr ? scaling->CartesianMax() : 15.0;

        startZSlider.setUpDownStyle(infinite);
        targetZSlider.setUpDownStyle(infinite);
        startZSlider.setRange(Constants::DistanceMin, distanceMax, 0.01);
        targetZSlider.setRange(Constants::DistanceMin, distanceMax, 0.01);

        // Convert current values for display
        startXSlider.setValue(Constants::RadToGrad(currentClip.startPointGroup.getAzimuth()));
        startYSlider.setValue(Constants::RadToGrad(currentClip.startPointGroup.getElevation()));
        startZSlider.setValue(currentClip.startPointGroup.getDistance());
        
        targetXSlider.setValue(Constants::RadToGrad(currentClip.targetPointGroup.getAzimuth()));
        targetYSlider.setValue(Constants::RadToGrad(currentClip.targetPointGroup.getElevation()));
        targetZSlider.setValue(currentClip.targetPointGroup.getDistance());
    }
    else
    {
        // Cartesian coordinates (X, Y, Z)
        startXLabel.setText("Start X:", juce::dontSendNotification);
        startYLabel.setText("Start Y:", juce::dontSendNotification);
        startZLabel.setText("Start Z:", juce::dontSendNotification);
        
        // Update target label based on movement type
        MovementType currentType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
        juce::String targetLabel = "Target ";
        if (movementTypeUsesTargetAsCentre(currentType))
            targetLabel = "Center ";
        
        targetXLabel.setText(targetLabel + "X:", juce::dontSendNotification);
        targetYLabel.setText(targetLabel + "Y:", juce::dontSendNotification);
        targetZLabel.setText(targetLabel + "Z:", juce::dontSendNotification);
        
        // Set Cartesian ranges: CartesianMin/Max already substitute a large-but-finite bound when
        // infinite, so this must not gate on IsInfinite() itself - doing so is what previously
        // left X/Y/Z clamped to a hardcoded +/-10 in infinite mode instead of the real bound.
        double minVal = -10.0;
        double maxVal = 10.0;
        ScalingInfo* scaling = (pSourceSet != nullptr) ? pSourceSet->getScalingInfo() : nullptr;
        if (scaling != nullptr)
        {
            minVal = scaling->CartesianMin();
            maxVal = scaling->CartesianMax();
        }

        bool infinite = scaling != nullptr && scaling->IsInfinite();
        startXSlider.setUpDownStyle(infinite);
        startYSlider.setUpDownStyle(infinite);
        startZSlider.setUpDownStyle(infinite);
        targetXSlider.setUpDownStyle(infinite);
        targetYSlider.setUpDownStyle(infinite);
        targetZSlider.setUpDownStyle(infinite);

        startXSlider.setRange(minVal, maxVal, 0.01);
        startYSlider.setRange(minVal, maxVal, 0.01);
        startZSlider.setRange(minVal, maxVal, 0.01);

        targetXSlider.setRange(minVal, maxVal, 0.01);
        targetYSlider.setRange(minVal, maxVal, 0.01);
        targetZSlider.setRange(minVal, maxVal, 0.01);
        
        // Use Cartesian values directly
        startXSlider.setValue(currentClip.startPointGroup.getX());
        startYSlider.setValue(currentClip.startPointGroup.getY());
        startZSlider.setValue(currentClip.startPointGroup.getZ());
        
        targetXSlider.setValue(currentClip.targetPointGroup.getX());
        targetYSlider.setValue(currentClip.targetPointGroup.getY());
        targetZSlider.setValue(currentClip.targetPointGroup.getZ());
    }
}

void MovementClipEditor::updateCoordinateSystem()
{
    // Store current values before switching
    Point3D<double> startPoint, targetPoint;
    
    if (usePolarDisplay.getToggleState())
    {
        // Switching to polar display - convert current Cartesian to polar for display
        startPoint.setXYZ(startXSlider.getPreciseValue(), startYSlider.getPreciseValue(), startZSlider.getPreciseValue());
        targetPoint.setXYZ(targetXSlider.getPreciseValue(), targetYSlider.getPreciseValue(), targetZSlider.getPreciseValue());
    }
    else
    {
        // Switching to Cartesian display - convert current polar to Cartesian for display
        double startAzimuthRad = Constants::GradToRad(startXSlider.getPreciseValue());
        double startElevationRad = Constants::GradToRad(startYSlider.getPreciseValue());
        double startDistance = startZSlider.getPreciseValue();
        
        double targetAzimuthRad = Constants::GradToRad(targetXSlider.getPreciseValue());
        double targetElevationRad = Constants::GradToRad(targetYSlider.getPreciseValue());
        double targetDistance = targetZSlider.getPreciseValue();
        
        startPoint.setAed(startAzimuthRad, startElevationRad, startDistance);
        targetPoint.setAed(targetAzimuthRad, targetElevationRad, targetDistance);
    }
    
    // Update UI
    updateSliderLabelsAndRanges();
    
    // Set values in new coordinate system for display
    if (usePolarDisplay.getToggleState())
    {
        startXSlider.setValue(Constants::RadToGrad(startPoint.getAzimuth()));
        startYSlider.setValue(Constants::RadToGrad(startPoint.getElevation()));
        startZSlider.setValue(startPoint.getDistance());
        
        targetXSlider.setValue(Constants::RadToGrad(targetPoint.getAzimuth()));
        targetYSlider.setValue(Constants::RadToGrad(targetPoint.getElevation()));
        targetZSlider.setValue(targetPoint.getDistance());
    }
    else
    {
        startXSlider.setValue(startPoint.getX());
        startYSlider.setValue(startPoint.getY());
        startZSlider.setValue(startPoint.getZ());
        
        targetXSlider.setValue(targetPoint.getX());
        targetYSlider.setValue(targetPoint.getY());
        targetZSlider.setValue(targetPoint.getZ());
    }
    
    updateCurrentPosition(true);
    repaint();
}

void MovementClipEditor::onMovementTypeChanged()
{
    MovementType newType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
    
    // Update coordinate display preference based on movement type
    bool shouldUsePolarDisplay = (newType == MovementType::MoveToPolar);
    if (usePolarDisplay.getToggleState() != shouldUsePolarDisplay)
    {
        usePolarDisplay.setToggleState(shouldUsePolarDisplay, juce::sendNotification);
    }
    else
    {
        // Still need to update labels even if display mode doesn't change
        updateSliderLabelsAndRanges();
    }
    
    // Reset parameters the new type doesn't use, so a clip doesn't silently carry a stale value
    // from a type it no longer is. This lives here (an explicit user type change) and NOT in
    // updateControlVisibility(), which also fires from unrelated toggles - see the note there.
    if (!showsCount())        countSlider.setValue(1.0);
    if (!showsRadiusChange()) radiusChangeSlider.setValue(0.0);
    if (!showsHeightRise())   heightRiseSlider.setValue(0.0);

    // An open-ended path (MoveTo, Spline, Polygon, Random Walk) ends somewhere other than where it
    // started, so a non-palindrome repeat snaps visibly at every repeat boundary - the same
    // mechanism as the Rotation-action constraint in CommonClipSettings.
    refreshPalindromeConstraint();

    updateControlVisibility();
    ensureWaypointsSeeded();

    // The row set just changed, so the Movement Properties column has to be laid out again.
    //
    // This used to happen as a side effect of setSize(getTotalRequiredWidth(), getTotalRequiredHeight()),
    // back when the dialog grew and shrank with the movement type. Both are constants now, so that
    // call became a no-op and took the relayout with it - leaving newly shown controls at whatever
    // bounds they last had. Ask for the relayout directly instead of relying on a size change.
    resized();
}

void MovementClipEditor::refreshPalindromeConstraint()
{
    // Built from the LIVE controls rather than currentClip: this runs from the movement-type combo
    // and the closed-path toggle, both of which fire before those values are written back.
    MovementClip probe = currentClip;
    probe.movementType = getSelectedMovementType();
    probe.closedPath = closedPathToggle.getToggleState();

    commonSettings.setPalindromeRequiredForRepeat(movementClipRequiresPalindromeForRepeat(probe));
}

void MovementClipEditor::updateControlVisibility()
{
    MovementType currentType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
    
    // Update start position controls
    bool startPositionEnabled = useStartPosition.getToggleState();
    startXSlider.setEnabled(startPositionEnabled);
    startYSlider.setEnabled(startPositionEnabled);
    startZSlider.setEnabled(startPositionEnabled);
    startXLabel.setEnabled(startPositionEnabled);
    startYLabel.setEnabled(startPositionEnabled);
    startZLabel.setEnabled(startPositionEnabled);
    applyCurrentStartButton.setEnabled(startPositionEnabled && currentPositionValid);
    
    float startAlpha = startPositionEnabled ? 1.0f : 0.5f;
    startXSlider.setAlpha(startAlpha);
    startYSlider.setAlpha(startAlpha);
    startZSlider.setAlpha(startAlpha);
    startXLabel.setAlpha(startAlpha);
    startYLabel.setAlpha(startAlpha);
    startZLabel.setAlpha(startAlpha);
    applyCurrentStartButton.setAlpha((startPositionEnabled && currentPositionValid) ? 1.0f : 0.5f);
    
    // Per-type rows are now SHOWN or HIDDEN rather than merely dimmed, and crucially their values
    // are no longer reset here. Resetting on every visibility update was destructive: this function
    // also runs from useStartPosition.onClick, so toggling an unrelated checkbox would silently
    // wipe a Spiral's radiusChange or a Circle's count. Type-switch defaults now live solely in
    // onMovementTypeChanged(), which is an explicit user action.
    usePolarDisplay.setVisible(showsPolarToggle());

    const bool targetVisible = showsTargetRows();
    targetXSlider.setVisible(targetVisible);
    targetYSlider.setVisible(targetVisible);
    targetZSlider.setVisible(targetVisible);
    targetXLabel.setVisible(targetVisible);
    targetYLabel.setVisible(targetVisible);
    targetZLabel.setVisible(targetVisible);
    applyCurrentTargetButton.setVisible(targetVisible);

    const bool tableVisible = showsWaypointTable();
    waypointTable.setVisible(tableVisible);
    addWaypointButton.setVisible(tableVisible);
    removeWaypointButton.setVisible(tableVisible);

    auto setRowVisible = [](juce::Component& label, juce::Component& control, bool visible)
    {
        label.setVisible(visible);
        control.setVisible(visible);
    };

    setRowVisible(countLabel, countSlider, showsCount());
    setRowVisible(radiusChangeLabel, radiusChangeSlider, showsRadiusChange());
    setRowVisible(closedPathToggle, closedPathInfo, showsWaypointTable());
    setRowVisible(tensionLabel, tensionSlider, showsTension());
    tensionInfo.setVisible(showsTension());
    setRowVisible(heightRiseLabel, heightRiseSlider, showsHeightRise());
    setRowVisible(freqALabel, freqASlider, showsFreqA());
    setRowVisible(freqBLabel, freqBSlider, showsFreqB());
    setRowVisible(phaseLabel, phaseSlider, showsPhase());
    setRowVisible(randomSeedLabel, randomSeedSlider, showsRandomSeed());

    // Rose reuses the Lissajous "Frequency X" control as its petal count, and Lissajous relabels
    // the start controls because it's the one type that does not begin at its start point.
    freqALabel.setText(currentType == MovementType::Rose ? "Petals:" : "Frequency X:", juce::dontSendNotification);
    countLabel.setText(currentType == MovementType::RandomWalk ? "Wander cycles:" : "Rotations:", juce::dontSendNotification);
}

MovementType MovementClipEditor::getSelectedMovementType() const
{
    return static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
}

void MovementClipEditor::refreshWaypointTable()
{
    waypointTable.updateContent();
    waypointTable.repaint();
    preview.setMovementClip(buildClipFromControls());
}

// Switching to Spline/Polygon with nothing in the list would otherwise leave the clip with no path
// at all - the group would just hold still for the clip's whole duration with no hint why. Seeding
// a short two-point path makes the type immediately do something visible and draggable.
void MovementClipEditor::ensureWaypointsSeeded()
{
    if (!movementTypeUsesWaypoints(getSelectedMovementType()) || !currentClip.waypoints.isEmpty())
        return;

    const auto startPos = currentPositionValid ? currentPosition : juce::Vector3D<double>();

    MovementWaypoint first;
    first.x = startPos.x;
    first.y = startPos.y;
    first.z = startPos.z;
    currentClip.waypoints.add(first);

    MovementWaypoint second = first;
    second.x += 1.0;
    currentClip.waypoints.add(second);

    refreshWaypointTable();
}

void MovementClipEditor::addWaypoint()
{
    // Append just past the last point so the new one is visible and separable rather than landing
    // exactly on top of its neighbour (a zero-length span contributes no time and can't be grabbed).
    MovementWaypoint wp;
    if (!currentClip.waypoints.isEmpty())
    {
        wp = currentClip.waypoints.getReference(currentClip.waypoints.size() - 1);
        wp.x += 1.0;
        wp.startsNewSegment = false;
    }
    else if (currentPositionValid)
    {
        wp.x = currentPosition.x;
        wp.y = currentPosition.y;
        wp.z = currentPosition.z;
    }

    currentClip.waypoints.add(wp);
    refreshWaypointTable();
    waypointTable.selectRow(currentClip.waypoints.size() - 1);
}

void MovementClipEditor::removeSelectedWaypoint()
{
    const int selected = waypointTable.getSelectedRow();
    if (selected < 0 || selected >= currentClip.waypoints.size())
        return;

    currentClip.waypoints.remove(selected);

    // The first waypoint always starts the first sub-path, so a break flag that has shuffled up
    // into row 0 is meaningless - clear it rather than leaving an uneditable stale flag there.
    if (!currentClip.waypoints.isEmpty())
        currentClip.waypoints.getReference(0).startsNewSegment = false;

    refreshWaypointTable();
}

// Waypoint paths carry their coordinates in the table, which is XYZ-only, so the polar toggle and
// the target/centre rows have nothing to act on for them.
bool MovementClipEditor::showsPolarToggle() const   { return !movementTypeUsesWaypoints(getSelectedMovementType()); }
bool MovementClipEditor::showsTargetRows() const    { return !movementTypeUsesWaypoints(getSelectedMovementType()); }
bool MovementClipEditor::showsWaypointTable() const { return movementTypeUsesWaypoints(getSelectedMovementType()); }

bool MovementClipEditor::showsCount() const
{
    const auto type = getSelectedMovementType();
    return type == MovementType::Circle || type == MovementType::Spiral
        || type == MovementType::Helix || type == MovementType::Lissajous
        || type == MovementType::Rose || type == MovementType::RandomWalk;
}

bool MovementClipEditor::showsRadiusChange() const
{
    const auto type = getSelectedMovementType();
    return type == MovementType::Spiral || type == MovementType::Helix;
}

bool MovementClipEditor::showsTension() const    { return getSelectedMovementType() == MovementType::Spline; }
bool MovementClipEditor::showsHeightRise() const { return getSelectedMovementType() == MovementType::Helix; }
bool MovementClipEditor::showsFreqB() const      { return getSelectedMovementType() == MovementType::Lissajous; }
bool MovementClipEditor::showsRandomSeed() const { return getSelectedMovementType() == MovementType::RandomWalk; }

bool MovementClipEditor::showsFreqA() const
{
    const auto type = getSelectedMovementType();
    return type == MovementType::Lissajous || type == MovementType::Rose;
}

bool MovementClipEditor::showsPhase() const
{
    const auto type = getSelectedMovementType();
    return type == MovementType::Lissajous || type == MovementType::Rose;
}

int MovementClipEditor::getMovementControlsHeight() const
{
    const int rowHeight = 28;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;

    // Mirrors layoutMovementControls() BLOCK FOR BLOCK, not control by control. Charging every
    // control rowHeight+verticalSpacing over-reports: the three coordinate sliders are packed with
    // no gaps between them, and their Apply button sits on the smaller buttonSpacing. That came to
    // 20px per coordinate block - 40px for a type with both Start and Target - which showed up as
    // dead space at the bottom of the column, since the dialog is sized from this number.
    const int row = rowHeight + verticalSpacing;
    const int coordinateBlock = (3 * rowHeight) + buttonSpacing + rowHeight + verticalSpacing;

    int height = row;                              // movement type combo
    if (showsPolarToggle())   height += row;
    height += row;                                 // use start position checkbox
    height += coordinateBlock;                     // start X/Y/Z + "apply current"
    if (showsTargetRows())    height += coordinateBlock;

    // Parameter rows are packed with no gap between them (see layoutMovementControls), so they cost
    // one rowHeight each plus a single trailing gap for the block.
    int paramRows = 0;
    if (showsCount())         ++paramRows;
    if (showsRadiusChange())  ++paramRows;
    if (showsTension())       ++paramRows;
    if (showsHeightRise())    ++paramRows;
    if (showsFreqA())         ++paramRows;
    if (showsFreqB())         ++paramRows;
    if (showsPhase())         ++paramRows;
    if (showsRandomSeed())    ++paramRows;

    if (paramRows > 0)
        height += paramRows * rowHeight + verticalSpacing;

    // The table stretches into whatever the fixed-height column leaves over, so this is only its
    // minimum - enough to be usable if a future type ever squeezes it. The closed-path toggle sits
    // above it on its own row.
    if (showsWaypointTable())
        height += row + verticalSpacing + getWaypointTableHeight() + verticalSpacing + rowHeight;

    return height;
}

void MovementClipEditor::layoutMovementControls(juce::Rectangle<int> area)
{
    // The column is a fixed height for every movement type, so a type whose controls don't fit would
    // silently clip its last row rather than growing the dialog. Catch that here instead.
    jassert(getMovementControlsHeight() <= area.getHeight());

    const int rowHeight = 28;
    const int labelWidth = 170; // wide enough for "Radius change / rotation:"
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    const int rightMargin = 10;
    const int labelLeftMargin = 10;

    // Movement type combo
    auto typeArea = area.removeFromTop(rowHeight);
    movementTypeLabel.setBounds(typeArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    movementTypeCombo.setBounds(typeArea.withTrimmedRight(rightMargin));
    
    area.removeFromTop(verticalSpacing);

    // Polar display checkbox (hidden for waypoint paths, whose table is XYZ-only)
    if (showsPolarToggle())
    {
        auto polarArea = area.removeFromTop(rowHeight);
        usePolarDisplay.setBounds(polarArea.withTrimmedLeft(labelLeftMargin));

        area.removeFromTop(verticalSpacing);
    }
    
    // Start position checkbox
    auto checkboxArea = area.removeFromTop(rowHeight);
    useStartPosition.setBounds(checkboxArea.withTrimmedLeft(labelLeftMargin));
    
    area.removeFromTop(verticalSpacing);
    
    // Start position controls
    auto startXArea = area.removeFromTop(rowHeight);
    startXLabel.setBounds(startXArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    startXSlider.setBounds(startXArea.withTrimmedRight(rightMargin));
    
    auto startYArea = area.removeFromTop(rowHeight);
    startYLabel.setBounds(startYArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    startYSlider.setBounds(startYArea.withTrimmedRight(rightMargin));
    
    auto startZArea = area.removeFromTop(rowHeight);
    startZLabel.setBounds(startZArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    startZSlider.setBounds(startZArea.withTrimmedRight(rightMargin));
    
    area.removeFromTop(buttonSpacing);
    
    // Start position apply button
    auto startButtonArea = area.removeFromTop(rowHeight);
    applyCurrentStartButton.setBounds(startButtonArea.withTrimmedLeft(labelLeftMargin).withTrimmedRight(rightMargin));
    
    area.removeFromTop(verticalSpacing);
    
    // Target/centre position controls - replaced by the waypoint table for Spline/Polygon
    if (showsTargetRows())
    {
        auto targetXArea = area.removeFromTop(rowHeight);
        targetXLabel.setBounds(targetXArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetXSlider.setBounds(targetXArea.withTrimmedRight(rightMargin));

        auto targetYArea = area.removeFromTop(rowHeight);
        targetYLabel.setBounds(targetYArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetYSlider.setBounds(targetYArea.withTrimmedRight(rightMargin));

        auto targetZArea = area.removeFromTop(rowHeight);
        targetZLabel.setBounds(targetZArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
        targetZSlider.setBounds(targetZArea.withTrimmedRight(rightMargin));

        area.removeFromTop(buttonSpacing);

        auto targetButtonArea = area.removeFromTop(rowHeight);
        applyCurrentTargetButton.setBounds(targetButtonArea.withTrimmedLeft(labelLeftMargin).withTrimmedRight(rightMargin));

        area.removeFromTop(verticalSpacing);
    }

    // One optional parameter row - must stay in lockstep with getMovementControlsHeight().
    //
    // Packed with no gap between consecutive rows, matching the Start/Target coordinate rows just
    // above. They used to carry a full verticalSpacing each, which made the parameter block at the
    // bottom look loose next to the tightly-packed coordinates above it.
    auto layoutParamRow = [&](bool visible, juce::Label& label, CoordinateValueControl& control,
                              InfoIconButton* info = nullptr)
    {
        if (!visible) return;

        auto rowArea = area.removeFromTop(rowHeight);
        auto labelArea = rowArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin);

        // The icon sits at the right-hand end of the label column, so it never eats into the value
        // control and the rows stay aligned whether or not a row has one.
        if (info != nullptr)
            info->setBounds(labelArea.removeFromRight(rowHeight).reduced(2, 3));

        label.setBounds(labelArea);
        control.setBounds(rowArea.withTrimmedRight(rightMargin));
    };

    layoutParamRow(showsCount(), countLabel, countSlider);
    layoutParamRow(showsRadiusChange(), radiusChangeLabel, radiusChangeSlider);
    layoutParamRow(showsTension(), tensionLabel, tensionSlider, &tensionInfo);
    layoutParamRow(showsHeightRise(), heightRiseLabel, heightRiseSlider);
    layoutParamRow(showsFreqA(), freqALabel, freqASlider);
    layoutParamRow(showsFreqB(), freqBLabel, freqBSlider);
    layoutParamRow(showsPhase(), phaseLabel, phaseSlider);
    layoutParamRow(showsRandomSeed(), randomSeedLabel, randomSeedSlider);

    // Closed-path toggle, directly above the waypoint table it applies to.
    if (showsWaypointTable())
    {
        auto closedRow = area.removeFromTop(rowHeight);
        auto closedArea = closedRow.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin);
        closedPathInfo.setBounds(closedArea.removeFromRight(rowHeight).reduced(2, 3));
        closedPathToggle.setBounds(closedArea);
    }

    // Waypoint table with its Add/Remove row underneath. The buttons come off the BOTTOM first so
    // the table takes every remaining pixel of the fixed-height column - a Spline needs far fewer
    // rows than a Lissajous, and that slack is worth far more as visible waypoints than as padding.
    if (showsWaypointTable())
    {
        area.removeFromTop(verticalSpacing);

        auto buttonArea = area.removeFromBottom(rowHeight).withTrimmedLeft(labelLeftMargin);
        addWaypointButton.setBounds(buttonArea.removeFromLeft(110));
        buttonArea.removeFromLeft(8);
        removeWaypointButton.setBounds(buttonArea.removeFromLeft(110));

        area.removeFromBottom(verticalSpacing);

        waypointTable.setBounds(area.withTrimmedLeft(labelLeftMargin).withTrimmedRight(rightMargin));
    }
}

void MovementClipEditor::updateCurrentPosition(bool force)
{
    auto lastPosition = currentPosition;
    auto lastPositionValid = currentPositionValid;
    
    currentPositionValid = false;
    
    if (pSourceSet != nullptr)
    {
        auto grp = pSourceSet->getGroup(timelineIndex);
        if (grp != nullptr)
        {
            currentPosition = grp->getVector3D();
            currentPositionValid = true;
        }
        else
        {
            currentPositionValid = false;
        }
    }

    if (currentPositionValid)
        preview.setReferencePosition(currentPosition);

    if(force || lastPositionValid != currentPositionValid || !approximatelyEqual(lastPosition.x, currentPosition.x) || !approximatelyEqual(lastPosition.y, currentPosition.y) || !approximatelyEqual(lastPosition.z, currentPosition.z))
    {
        updateApplyCurrentPositionButtonText(applyCurrentStartButton, getCurrentPositionInSelectedSystem(), currentPositionValid);
        updateApplyCurrentPositionButtonText(applyCurrentTargetButton, getCurrentPositionInSelectedSystem(), currentPositionValid);
        
        // Update button enablement
        applyCurrentTargetButton.setEnabled(currentPositionValid);
        applyCurrentStartButton.setEnabled(useStartPosition.getToggleState() && currentPositionValid);
        
        applyCurrentTargetButton.setAlpha(currentPositionValid ? 1.0f : 0.5f);
        applyCurrentStartButton.setAlpha((useStartPosition.getToggleState() && currentPositionValid) ? 1.0f : 0.5f);
    }
}

void MovementClipEditor::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    updateCurrentPosition();
}
