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
    
    createControls();
}

void MovementClipEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Calculate group heights
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
    createCoordinateSlider(startXSlider, startXLabel, "Start X:", -10.0f, 10.0f, currentClip.startPointGroup.getX());
    createCoordinateSlider(startYSlider, startYLabel, "Start Y:", -10.0f, 10.0f, currentClip.startPointGroup.getY());
    createCoordinateSlider(startZSlider, startZLabel, "Start Z:", -10.0f, 10.0f, currentClip.startPointGroup.getZ());
    
    createCoordinateSlider(targetXSlider, targetXLabel, "Target X:", -10.0f, 10.0f, currentClip.endPointGroup.getX());
    createCoordinateSlider(targetYSlider, targetYLabel, "Target Y:", -10.0f, 10.0f, currentClip.endPointGroup.getY());
    createCoordinateSlider(targetZSlider, targetZLabel, "Target Z:", -10.0f, 10.0f, currentClip.endPointGroup.getZ());
    
    updateStartPositionVisibility();
}

void MovementClipEditor::createCoordinateSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                           float min, float max, float defaultValue)
{
    addAndMakeVisible(slider);
    slider.setRange(min, max, 0.01f);
    slider.setValue(defaultValue);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 22);
    
    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredLeft);
}

int MovementClipEditor::getMovementControlsHeight() const
{
    const int rowHeight = 28;
    const int verticalSpacing = 8;
    
    // Checkbox + 3 start sliders + 3 target sliders + spacing
    return rowHeight + verticalSpacing + (rowHeight * 3) + verticalSpacing + (rowHeight * 3);
}

void MovementClipEditor::layoutMovementControls(juce::Rectangle<int> area)
{
    const int rowHeight = 28;
    const int labelWidth = 100;
    const int verticalSpacing = 8;
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
    
    startXSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startYSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startZSlider.setAlpha(enabled ? 1.0f : 0.5f);
    startXLabel.setAlpha(enabled ? 1.0f : 0.5f);
    startYLabel.setAlpha(enabled ? 1.0f : 0.5f);
    startZLabel.setAlpha(enabled ? 1.0f : 0.5f);
}
