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
    
    // Calculate group heights - increased to accommodate new checkbox
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 40;
    const int movementGroupHeight = getMovementControlsHeight() + 40;
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
    const int clipGroupHeight = commonSettings.getRequiredHeight() + 30;
    const int movementGroupHeight = getMovementControlsHeight() + 40;
    const int buttonHeight = 28;
    const int groupSpacing = 3 * 8;
    
    return margins + clipGroupHeight + groupSpacing + movementGroupHeight + buttonHeight;
}

bool MovementClipEditor::applyChanges()
{
    if (!commonSettings.validate())
        return false;
        
    commonSettings.applyToClip(currentClip);
    
    currentClip.useStartPoint = useStartPosition.getToggleState();
    currentClip.usePolar = usePolarCoordinates.getToggleState();
    
    if (usePolarCoordinates.getToggleState())
    {
        // Convert from degrees (UI) to radians (storage)
        double startAzimuthRad = Constants::GradToRad(startXSlider.getValue());
        double startElevationRad = Constants::GradToRad(startYSlider.getValue());
        double startDistance = startZSlider.getValue();
        
        double targetAzimuthRad = Constants::GradToRad(targetXSlider.getValue());
        double targetElevationRad = Constants::GradToRad(targetYSlider.getValue());
        double targetDistance = targetZSlider.getValue();
        
        currentClip.startPointGroup.setAed(startAzimuthRad, startElevationRad, startDistance);
        currentClip.endPointGroup.setAed(targetAzimuthRad, targetElevationRad, targetDistance);
    }
    else
    {
        currentClip.startPointGroup.setXYZ(startXSlider.getValue(), startYSlider.getValue(), startZSlider.getValue());
        currentClip.endPointGroup.setXYZ(targetXSlider.getValue(), targetYSlider.getValue(), targetZSlider.getValue());
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
    
    // Add the new polar coordinates checkbox
    addAndMakeVisible(usePolarCoordinates);
    usePolarCoordinates.setButtonText("Use Polar Coordinates (AED)");
    usePolarCoordinates.setToggleState(currentClip.usePolar, juce::dontSendNotification);
    usePolarCoordinates.onClick = [this] { updateCoordinateSystem(); };
    
    addAndMakeVisible(useStartPosition);
    useStartPosition.setButtonText("Use Defined Start Position");
    useStartPosition.setToggleState(currentClip.useStartPoint, juce::dontSendNotification);
    useStartPosition.onClick = [this] { updateStartPositionVisibility(); };
    
    // Create coordinate sliders - ranges will be set by updateSliderLabelsAndRanges
    createCoordinateSlider(startXSlider, startXLabel, "Start X:", -10.0, 10.0, 0.0);
    createCoordinateSlider(startYSlider, startYLabel, "Start Y:", -10.0, 10.0, 0.0);
    createCoordinateSlider(startZSlider, startZLabel, "Start Z:", -10.0, 10.0, 0.0);
    
    createCoordinateSlider(targetXSlider, targetXLabel, "Target X:", -10.0, 10.0, 0.0);
    createCoordinateSlider(targetYSlider, targetYLabel, "Target Y:", -10.0, 10.0, 0.0);
    createCoordinateSlider(targetZSlider, targetZLabel, "Target Z:", -10.0, 10.0, 0.0);
    
    // Create apply current position buttons
    createApplyCurrentPositionButton(applyCurrentStartButton, startXSlider, startYSlider, startZSlider);
    createApplyCurrentPositionButton(applyCurrentTargetButton, targetXSlider, targetYSlider, targetZSlider);
    
    // Initialize slider values based on current clip and coordinate system
    updateSliderLabelsAndRanges();
    updateStartPositionVisibility();
    updateCurrentPosition(); // Initial update to set button states
}

void MovementClipEditor::createCoordinateSlider(UnlimitedRangeSlider& slider, juce::Label& label, const juce::String& name,
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

void MovementClipEditor::createApplyCurrentPositionButton(juce::TextButton& button, UnlimitedRangeSlider& xSlider, UnlimitedRangeSlider& ySlider, UnlimitedRangeSlider& zSlider)
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
    
    if (usePolarCoordinates.getToggleState())
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
    
    if (usePolarCoordinates.getToggleState())
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
    bool usePolar = usePolarCoordinates.getToggleState();
    
    if (usePolar)
    {
        // Polar coordinates (Azimuth, Elevation, Distance)
        startXLabel.setText("Start Azimuth:", juce::dontSendNotification);
        startYLabel.setText("Start Elevation:", juce::dontSendNotification);
        startZLabel.setText("Start Distance:", juce::dontSendNotification);
        
        targetXLabel.setText("Target Azimuth:", juce::dontSendNotification);
        targetYLabel.setText("Target Elevation:", juce::dontSendNotification);
        targetZLabel.setText("Target Distance:", juce::dontSendNotification);
        
        // Set polar ranges in degrees for UI
        ScalingInfo* scaling = pSourceSet->getScalingInfo();
        if (scaling != nullptr)
        {
            startXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);  // Azimuth in degrees
            startYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);    // Elevation in degrees
            startZSlider.setRange(Constants::DistanceMin, scaling->IsInfinite() ? scaling->DistanceMax() : 15.0, 0.01);     // Distance
            
            targetXSlider.setRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);
            targetYSlider.setRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);
            targetZSlider.setRange(Constants::DistanceMin, scaling->IsInfinite() ? scaling->DistanceMax() : 15.0, 0.01);
        }
        
        // Convert current polar values from radians (storage) to degrees (UI)
        Point3D<double> startPoint(currentClip.startPointGroup);
        startXSlider.setValue(Constants::RadToGrad(startPoint.getAzimuth()));  // Convert to degrees
        startYSlider.setValue(Constants::RadToGrad(startPoint.getElevation())); // Convert to degrees
        startZSlider.setValue(startPoint.getDistance());
        
        Point3D<double> targetPoint(currentClip.endPointGroup);
        targetXSlider.setValue(Constants::RadToGrad(targetPoint.getAzimuth()));  // Convert to degrees
        targetYSlider.setValue(Constants::RadToGrad(targetPoint.getElevation())); // Convert to degrees
        targetZSlider.setValue(targetPoint.getDistance());
    }
    else
    {
        // Cartesian coordinates (X, Y, Z) - no conversion needed
        startXLabel.setText("Start X:", juce::dontSendNotification);
        startYLabel.setText("Start Y:", juce::dontSendNotification);
        startZLabel.setText("Start Z:", juce::dontSendNotification);
        
        targetXLabel.setText("Target X:", juce::dontSendNotification);
        targetYLabel.setText("Target Y:", juce::dontSendNotification);
        targetZLabel.setText("Target Z:", juce::dontSendNotification);
        
        // Set Cartesian ranges based on source set scaling
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
        
        targetXSlider.setValue(currentClip.endPointGroup.getX());
        targetYSlider.setValue(currentClip.endPointGroup.getY());
        targetZSlider.setValue(currentClip.endPointGroup.getZ());
    }
}

void MovementClipEditor::updateCoordinateSystem()
{
    bool wasPolar = !usePolarCoordinates.getToggleState();
    
    if (wasPolar != usePolarCoordinates.getToggleState())
    {
        // Store the actual point values before switching
        Point3D<double> startPoint, targetPoint;
        
        if (wasPolar)
        {
            // Was polar, now switching to Cartesian
            // Convert current polar values (in degrees from UI) to actual points
            double startAzimuthRad = Constants::GradToRad(startXSlider.getValue());
            double startElevationRad = Constants::GradToRad(startYSlider.getValue());
            double startDistance = startZSlider.getValue();
            
            double targetAzimuthRad = Constants::GradToRad(targetXSlider.getValue());
            double targetElevationRad = Constants::GradToRad(targetYSlider.getValue());
            double targetDistance = targetZSlider.getValue();
            
            startPoint.setAed(startAzimuthRad, startElevationRad, startDistance);
            targetPoint.setAed(targetAzimuthRad, targetElevationRad, targetDistance);
        }
        else
        {
            // Was Cartesian, now switching to polar
            // Convert current Cartesian values to actual points
            startPoint.setXYZ(startXSlider.getValue(), startYSlider.getValue(), startZSlider.getValue());
            targetPoint.setXYZ(targetXSlider.getValue(), targetYSlider.getValue(), targetZSlider.getValue());
        }
        
        // Update UI
        updateSliderLabelsAndRanges();
        
        // Set values in new coordinate system
        if (usePolarCoordinates.getToggleState())
        {
            // Convert from radians to degrees for display
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
    }
    else
    {
        // Just update labels and ranges without converting values
        updateSliderLabelsAndRanges();
    }
    
    updateCurrentPosition(true);
    repaint();
}

int MovementClipEditor::getMovementControlsHeight() const
{
    const int rowHeight = 28;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    
    // New checkbox + existing checkbox + 3 start sliders + start button + 3 target sliders + target button + spacing
    return rowHeight + verticalSpacing +  // usePolarCoordinates
           rowHeight + verticalSpacing +  // useStartPosition
           (rowHeight * 3) + buttonSpacing + rowHeight + verticalSpacing +  // Start controls
           (rowHeight * 3) + buttonSpacing + rowHeight;  // Target controls
}

void MovementClipEditor::layoutMovementControls(juce::Rectangle<int> area)
{
    const int rowHeight = 28;
    const int labelWidth = 100;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    const int rightMargin = 10;
    const int labelLeftMargin = 10; // Labels indented from group box
    
    // Polar coordinates checkbox
    auto polarCheckboxArea = area.removeFromTop(rowHeight);
    usePolarCoordinates.setBounds(polarCheckboxArea.withTrimmedLeft(labelLeftMargin));
    
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
}

void MovementClipEditor::updateStartPositionVisibility()
{
    bool enabled = useStartPosition.getToggleState();
    
    // Enable/disable start position controls with visual feedback
    startXSlider.setEnabled(enabled);
    startYSlider.setEnabled(enabled);
    startZSlider.setEnabled(enabled);
    startXLabel.setEnabled(enabled);
    startYLabel.setEnabled(enabled);
    startZLabel.setEnabled(enabled);
    applyCurrentStartButton.setEnabled(enabled && currentPositionValid);
    
    startXSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startYSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startZSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startXLabel.setAlpha(enabled ? 1.0f : 0.5f);
    startYLabel.setAlpha(enabled ? 1.0f : 0.5f);
    startZLabel.setAlpha(enabled ? 1.0f : 0.5f);
    applyCurrentStartButton.setAlpha((enabled && currentPositionValid) ? 1.0f : 0.5f);
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
