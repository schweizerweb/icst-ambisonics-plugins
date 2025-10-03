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
    if(pSourceSet != nullptr)
    {
        pSourceSet->addChangeListener(this);
        updateCurrentPosition();
    }
    
    createControls();
}

MovementClipEditor::~MovementClipEditor()
{
    if (pSourceSet != nullptr)
    {
        pSourceSet->removeChangeListener(this);
    }
}

void MovementClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Calculate group heights - increased to accommodate new buttons
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
    currentClip.startPointGroup.setXYZ(startXSlider.getValue(), startYSlider.getValue(), startZSlider.getValue());
    currentClip.endPointGroup.setXYZ(targetXSlider.getValue(), targetYSlider.getValue(), targetZSlider.getValue());
    
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
    
    addAndMakeVisible(useStartPosition);
    useStartPosition.setButtonText("Use Defined Start Position");
    useStartPosition.setToggleState(currentClip.useStartPoint, juce::dontSendNotification);
    useStartPosition.onClick = [this] { updateStartPositionVisibility(); };
    
    // Create coordinate sliders for start and target positions
    double minVal = -10.0f;
    double maxVal = 10.0f;
    if(pSourceSet != nullptr)
    {
        ScalingInfo* scaling = pSourceSet->getScalingInfo();
        if(scaling != nullptr && !scaling->IsInfinite())
        {
            minVal = scaling->CartesianMin();
            maxVal = scaling->CartesianMax();
        }
    }
    
    createCoordinateSlider(startXSlider, startXLabel, "Start X:", minVal, maxVal, currentClip.startPointGroup.getX());
    createCoordinateSlider(startYSlider, startYLabel, "Start Y:", minVal, maxVal, currentClip.startPointGroup.getY());
    createCoordinateSlider(startZSlider, startZLabel, "Start Z:", minVal, maxVal, currentClip.startPointGroup.getZ());
    
    createCoordinateSlider(targetXSlider, targetXLabel, "Target X:", minVal, maxVal, currentClip.endPointGroup.getX());
    createCoordinateSlider(targetYSlider, targetYLabel, "Target Y:", minVal, maxVal, currentClip.endPointGroup.getY());
    createCoordinateSlider(targetZSlider, targetZLabel, "Target Z:", minVal, maxVal, currentClip.endPointGroup.getZ());
    
    // Create apply current position buttons
    createApplyCurrentPositionButton(applyCurrentStartButton, startXSlider, startYSlider, startZSlider);
    createApplyCurrentPositionButton(applyCurrentTargetButton, targetXSlider, targetYSlider, targetZSlider);
    
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
            xSlider.setValue(currentPosition.x);
            ySlider.setValue(currentPosition.y);
            zSlider.setValue(currentPosition.z);
        }
    };
}

void MovementClipEditor::updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid)
{
    if (isValid)
    {
        button.setButtonText(juce::String("Apply ") +
                             juce::String(vector.x, 2) + " (X); " +
                             juce::String(vector.y, 2) + " (Y); " +
                             juce::String(vector.z, 2) + " (Z)");
    }
    else
    {
        button.setButtonText("Unable to determine current position");
    }
}

int MovementClipEditor::getMovementControlsHeight() const
{
    const int rowHeight = 28;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    
    // Checkbox + 3 start sliders + start button + 3 target sliders + target button + spacing
    return rowHeight + verticalSpacing +
           (rowHeight * 3) + buttonSpacing + rowHeight + verticalSpacing +
           (rowHeight * 3) + buttonSpacing + rowHeight;
}

void MovementClipEditor::layoutMovementControls(juce::Rectangle<int> area)
{
    const int rowHeight = 28;
    const int labelWidth = 100;
    const int verticalSpacing = 8;
    const int buttonSpacing = 4;
    const int rightMargin = 10;
    const int labelLeftMargin = 10; // Labels indented from group box
    
    // Checkbox
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

void MovementClipEditor::updateCurrentPosition()
{
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

    updateApplyCurrentPositionButtonText(applyCurrentStartButton, currentPosition, currentPositionValid);
    updateApplyCurrentPositionButtonText(applyCurrentTargetButton, currentPosition, currentPositionValid);
    
    // Update button enablement
    applyCurrentTargetButton.setEnabled(currentPositionValid);
    applyCurrentStartButton.setEnabled(useStartPosition.getToggleState() && currentPositionValid);
    
    applyCurrentTargetButton.setAlpha(currentPositionValid ? 1.0f : 0.5f);
    applyCurrentStartButton.setAlpha((useStartPosition.getToggleState() && currentPositionValid) ? 1.0f : 0.5f);
}

void MovementClipEditor::changeListenerCallback(ChangeBroadcaster *source)
{
    updateCurrentPosition();
}
