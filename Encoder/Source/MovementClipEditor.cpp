#include "MovementClipEditor.h"
#include "TimelineComponent.h"
#include "CommonClipSettings.h"
#include "ClipEditorDialog.h"

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
}

MovementClipEditor::~MovementClipEditor()
{
    if (pPointSelection != nullptr)
    {
        pPointSelection->removeChangeListener(this);
    }
}

void MovementClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Calculate group heights
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
    const int movementGroupHeight = getMovementControlsHeight();
    const int buttonHeight = 28;
    
    // Clip settings group
    auto clipGroupArea = area.removeFromTop(clipGroupHeight);
    clipGroup.setBounds(clipGroupArea);
    commonSettings.setBounds(clipGroupArea.reduced(8, 20));
    
    area.removeFromTop(8); // Spacing between groups
    
    // Movement settings group
    auto movementGroupArea = area.removeFromTop(movementGroupHeight);
    movementGroup.setBounds(movementGroupArea);
    
    auto movementContentArea = movementGroupArea.reduced(8, 20);
    layoutMovementControls(movementContentArea);
    
    // Buttons at bottom
    auto buttonArea = area.removeFromBottom(buttonHeight).reduced(10, 0);
    cancelButton.setBounds(buttonArea.removeFromRight(80));
    buttonArea.removeFromRight(8); // Button spacing
    applyButton.setBounds(buttonArea.removeFromRight(80));
}

void MovementClipEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

int MovementClipEditor::getTotalRequiredHeight() const
{
    const int margins = 10 * 2;
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
    const int movementGroupHeight = getMovementControlsHeight();
    const int buttonHeight = 28;
    const int groupSpacing = 2 * 8;
    
    return margins + clipGroupHeight + groupSpacing + movementGroupHeight + buttonHeight;
}

bool MovementClipEditor::applyChanges()
{
    if (!commonSettings.validate())
        return false;
        
    commonSettings.applyToClip(currentClip);
    
    currentClip.useStartPoint = useStartPosition.getToggleState();
    currentClip.movementType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
    
    // Store count and radiusChange
    currentClip.count = countSlider.getValue();
    currentClip.radiusChange = radiusChangeSlider.getValue();
    
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
        
        currentClip.startPointGroup.setAed(startAzimuthRad, startElevationRad, startDistance);
        currentClip.targetPointGroup.setAed(targetAzimuthRad, targetElevationRad, targetDistance);
    }
    else
    {
        currentClip.startPointGroup.setXYZ(startXSlider.getPreciseValue(), startYSlider.getPreciseValue(), startZSlider.getPreciseValue());
        currentClip.targetPointGroup.setXYZ(targetXSlider.getPreciseValue(), targetYSlider.getPreciseValue(), targetZSlider.getPreciseValue());
    }
    
    if (auto* timelineModel = timelineComp.getTimelineModel(timelineIndex))
    {
        if (clipIndex >= 0 && clipIndex < timelineModel->movement.clips.size())
        {
            timelineModel->movement.clips.getReference(clipIndex) = currentClip;
            return true;
        }
    }
    
    return false;
}

void MovementClipEditor::createControls()
{
    setOpaque(true);
    
    addAndMakeVisible(clipGroup);
    addAndMakeVisible(movementGroup);
    
    addAndMakeVisible(commonSettings);
    commonSettings.setClipData(currentClip);
    
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
        // Send action message to close the window
        if (auto* broadcaster = findParentComponentOfClass<juce::ActionBroadcaster>())
            broadcaster->sendActionMessage(ACTION_CLOSE_CLIP_EDITOR);
    };
    
    // Movement type combo box
    addAndMakeVisible(movementTypeLabel);
    movementTypeLabel.setText("Movement Type:", juce::dontSendNotification);
    movementTypeLabel.setJustificationType(juce::Justification::centredLeft);
    
    addAndMakeVisible(movementTypeCombo);
    movementTypeCombo.addItem("MoveTo (Cartesian)", static_cast<int>(MovementType::MoveToCartesian) + 1);
    movementTypeCombo.addItem("MoveTo (Polar)", static_cast<int>(MovementType::MoveToPolar) + 1);
    movementTypeCombo.addItem("Circle", static_cast<int>(MovementType::Circle) + 1);
    movementTypeCombo.addItem("Spiral", static_cast<int>(MovementType::Spiral) + 1);
    movementTypeCombo.setSelectedId(static_cast<int>(currentClip.movementType) + 1);
    movementTypeCombo.onChange = [this] { onMovementTypeChanged(); };
    
    // Coordinate display toggle (editor only - doesn't affect stored data)
    addAndMakeVisible(usePolarDisplay);
    usePolarDisplay.setButtonText("Show Polar Coordinates (AED)");
    // Default to polar display for polar movement types, Cartesian for others
    bool defaultPolarDisplay = currentClip.movementType == MovementType::MoveToPolar;
    usePolarDisplay.setToggleState(defaultPolarDisplay, juce::dontSendNotification);
    usePolarDisplay.onClick = [this] { updateCoordinateSystem(); };
    
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
    
    createCoordinateSlider(countSlider, countLabel, "Count:", 0.1, 10.0, currentClip.count);
        createCoordinateSlider(radiusChangeSlider, radiusChangeLabel, "Radius change:", -5.0, 5.0, currentClip.radiusChange);
    
    // Create apply current position buttons
    createApplyCurrentPositionButton(applyCurrentStartButton, startXSlider, startYSlider, startZSlider);
    createApplyCurrentPositionButton(applyCurrentTargetButton, targetXSlider, targetYSlider, targetZSlider);
    
    // Initialize UI
    updateSliderLabelsAndRanges();
    updateControlVisibility();
    updateCurrentPosition();
}

void MovementClipEditor::createCoordinateSlider(PrecisionSlider& slider, juce::Label& label, const juce::String& name,
                           double min, double max, double defaultValue)
{
    addAndMakeVisible(slider);
    slider.setRange(min, max, 0.01);
    slider.setValue(defaultValue);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 22);
    
    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
}

void MovementClipEditor::createApplyCurrentPositionButton(juce::TextButton& button, PrecisionSlider& xSlider, PrecisionSlider& ySlider, PrecisionSlider& zSlider)
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
    
    if (usePolarDisplay.getToggleState())
    {
        // Convert Cartesian to polar (AED) and return in degrees for UI
        Point3D<double> point(currentPosition.x, currentPosition.y, currentPosition.z);
        return juce::Vector3D<double>(
            Constants::RadToGrad(point.getAzimuth()),  // Convert to degrees
            Constants::RadToGrad(point.getElevation()), // Convert to degrees
            point.getDistance()
        );
    }
    else
    {
        // Use Cartesian directly
        return currentPosition;
    }
}

void MovementClipEditor::updateSliderLabelsAndRanges()
{
    bool usePolar = usePolarDisplay.getToggleState();
    
    if (usePolar)
    {
        // Polar coordinates (Azimuth, Elevation, Distance)
        startXLabel.setText("Start Azimuth:", juce::dontSendNotification);
        startYLabel.setText("Start Elevation:", juce::dontSendNotification);
        startZLabel.setText("Start Distance:", juce::dontSendNotification);
        
        // Update target label based on movement type
        MovementType currentType = static_cast<MovementType>(movementTypeCombo.getSelectedId() - 1);
        juce::String targetLabel = "Target ";
        if (currentType == MovementType::Circle || currentType == MovementType::Spiral)
            targetLabel = "Center ";
        
        targetXLabel.setText(targetLabel + "Azimuth:", juce::dontSendNotification);
        targetYLabel.setText(targetLabel + "Elevation:", juce::dontSendNotification);
        targetZLabel.setText(targetLabel + "Distance:", juce::dontSendNotification);
        
        // Set polar ranges in degrees for UI
        ScalingInfo* scaling = pSourceSet->getScalingInfo();
        if (scaling != nullptr)
        {
            startXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);
            startYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);
            startZSlider.setRange(Constants::DistanceMin, scaling->IsInfinite() ? scaling->DistanceMax() : 15.0, 0.01);
            
            targetXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);
            targetYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);
            targetZSlider.setRange(Constants::DistanceMin, scaling->IsInfinite() ? scaling->DistanceMax() : 15.0, 0.01);
        }
        
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
        if (currentType == MovementType::Circle || currentType == MovementType::Spiral)
            targetLabel = "Center ";
        
        targetXLabel.setText(targetLabel + "X:", juce::dontSendNotification);
        targetYLabel.setText(targetLabel + "Y:", juce::dontSendNotification);
        targetZLabel.setText(targetLabel + "Z:", juce::dontSendNotification);
        
        // Set Cartesian ranges
        double minVal = -10.0;
        double maxVal = 10.0;
        if (pSourceSet != nullptr)
        {
            ScalingInfo* scaling = pSourceSet->getScalingInfo();
            if (scaling != nullptr && !scaling->IsInfinite())
            {
                minVal = scaling->CartesianMin();
                maxVal = scaling->CartesianMax();
            }
        }
        
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
    
    // Set default values for disabled controls
    if (newType != MovementType::Circle && newType != MovementType::Spiral)
    {
        countSlider.setValue(1.0f);
    }
    
    if (newType != MovementType::Spiral)
    {
        radiusChangeSlider.setValue(0.0f);
    }
    
    updateControlVisibility();
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
    
    // Update count slider enablement (always visible)
    bool countEnabled = (currentType == MovementType::Circle || currentType == MovementType::Spiral);
    countSlider.setEnabled(countEnabled);
    countLabel.setEnabled(countEnabled);
    countSlider.setAlpha(countEnabled ? 1.0f : 0.5f);
    countLabel.setAlpha(countEnabled ? 1.0f : 0.5f);
    
    // Set count to default value of 1 when not enabled
    if (!countEnabled && countSlider.getValue() != 1.0f)
    {
        countSlider.setValue(1.0f);
    }
    
    // Update radius change slider enablement (always visible)
    bool radiusChangeEnabled = (currentType == MovementType::Spiral);
    radiusChangeSlider.setEnabled(radiusChangeEnabled);
    radiusChangeLabel.setEnabled(radiusChangeEnabled);
    radiusChangeSlider.setAlpha(radiusChangeEnabled ? 1.0f : 0.5f);
    radiusChangeLabel.setAlpha(radiusChangeEnabled ? 1.0f : 0.5f);
    
    // Set radius change to default value of 0 when not enabled
    if (!radiusChangeEnabled && radiusChangeSlider.getValue() != 0.0f)
    {
        radiusChangeSlider.setValue(0.0f);
    }
}

int MovementClipEditor::getMovementControlsHeight() const
{
    const int rowHeight = 28;
    const int verticalSpacing = 8;
    
    // Fixed number of rows since all controls are always visible
    const int fixedRows = 1 +  // movement type combo
                          1 +  // polar display checkbox
                          1 +  // use start position checkbox
                          3 +  // start sliders
                          1 +  // start button
                          3 +  // target sliders
                          1 +  // target button
                          1 +  // count slider (always visible)
                          1;   // radius change slider (always visible)
    
    return (fixedRows * rowHeight) + (fixedRows * verticalSpacing);
}

void MovementClipEditor::layoutMovementControls(juce::Rectangle<int> area)
{
    const int rowHeight = 28;
    const int labelWidth = 120;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    const int rightMargin = 10;
    const int labelLeftMargin = 10;

    // Movement type combo
    auto typeArea = area.removeFromTop(rowHeight);
    movementTypeLabel.setBounds(typeArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    movementTypeCombo.setBounds(typeArea.withTrimmedRight(rightMargin));
    
    area.removeFromTop(verticalSpacing);
    
    // Polar display checkbox
    auto polarArea = area.removeFromTop(rowHeight);
    usePolarDisplay.setBounds(polarArea.withTrimmedLeft(labelLeftMargin));
    
    area.removeFromTop(verticalSpacing);
    
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
    
    // Target position controls
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
    
    // Target position apply button
    auto targetButtonArea = area.removeFromTop(rowHeight);
    applyCurrentTargetButton.setBounds(targetButtonArea.withTrimmedLeft(labelLeftMargin).withTrimmedRight(rightMargin));
    
    area.removeFromTop(verticalSpacing);
    
    // Count slider (ALWAYS VISIBLE)
    auto countArea = area.removeFromTop(rowHeight);
    countLabel.setBounds(countArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    countSlider.setBounds(countArea.withTrimmedRight(rightMargin));
    
    area.removeFromTop(verticalSpacing);
    
    // Radius change slider (ALWAYS VISIBLE)
    auto radiusArea = area.removeFromTop(rowHeight);
    radiusChangeLabel.setBounds(radiusArea.removeFromLeft(labelWidth).withTrimmedLeft(labelLeftMargin));
    radiusChangeSlider.setBounds(radiusArea.withTrimmedRight(rightMargin));
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
