#pragma once

#include <JuceHeader.h>
#include "TimelineModel.h"

class ActionEditDialog : public juce::Component,
                         public juce::Button::Listener,
                         public juce::ComboBox::Listener
{
public:
    ActionEditDialog(ActionDefinition& action, const juce::String& title)
        : targetAction(action), dialogTitle(title)
    {
        setOpaque(true);
        
        // Type combo box
        addAndMakeVisible(typeLabel);
        typeLabel.setText("Action Type:", juce::dontSendNotification);
        typeLabel.setJustificationType(juce::Justification::left);
        
        addAndMakeVisible(typeCombo);
        typeCombo.addItem("Rotation X", (int)ActionType::RotationX);
        typeCombo.addItem("Rotation Y", (int)ActionType::RotationY);
        typeCombo.addItem("Rotation Z", (int)ActionType::RotationZ);
        typeCombo.addItem("Stretch", (int)ActionType::Stretch);
        typeCombo.addItem("Jitter", (int)ActionType::Jitter);
        typeCombo.setSelectedId((int)action.getAction(), juce::dontSendNotification);
        typeCombo.addListener(this);
        
        // Timing combo box
        addAndMakeVisible(timingLabel);
        timingLabel.setText("Timing Type:", juce::dontSendNotification);
        timingLabel.setJustificationType(juce::Justification::left);
        
        addAndMakeVisible(timingCombo);
        timingCombo.addItem("Absolute Target", (int)TimingType::AbsoluteTarget);
        timingCombo.addItem("Relative During Clip", (int)TimingType::RelativeDuringClip);
        timingCombo.addItem("Constant Per Second", (int)TimingType::ConstantPerSecond);
        timingCombo.setSelectedId((int)action.getTiming(), juce::dontSendNotification);
        timingCombo.addListener(this);
        
        // Value slider - a compact +/- spinner (matching the "Count"/"Radius change" fields in
        // MovementClipEditor) rather than free-text entry, for both the generic "Value" (Rotation/
        // Stretch) and "Intensity" (Jitter) roles this one control plays.
        addAndMakeVisible(valueLabel);
        updateValueLabel();

        addAndMakeVisible(valueSlider);
        valueSlider.setSliderStyle(juce::Slider::IncDecButtons);
        valueSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 70, 22);
        valueSlider.setRange(-1000.0, 1000.0, 0.1);
        // Jitter has no meaningful "zero" intensity (that's what Mute is for) - default a freshly
        // selected Jitter action to a visible 1.0 instead of inheriting the generic 0.0 default
        // every other action type starts from.
        valueSlider.setValue(action.getAction() == ActionType::Jitter && action.getValue() == 0.0
                                 ? 1.0 : action.getValue(), juce::dontSendNotification);
        valueSlider.onValueChange = [this] { pushLiveChange(); };

        // Use start value checkbox
        addAndMakeVisible(useStartValueButton);
        useStartValueButton.setButtonText("Use Start Value");
        useStartValueButton.setToggleState(action.getUseStartValue(), juce::dontSendNotification);
        useStartValueButton.addListener(this);
        
        // Start value editor
        addAndMakeVisible(startValueLabel);
        startValueLabel.setText("Start Value:", juce::dontSendNotification);
        startValueLabel.setJustificationType(juce::Justification::left);
        
        addAndMakeVisible(startValueEditor);
        startValueEditor.setText(juce::String(action.getStartValue()), juce::dontSendNotification);
        startValueEditor.onTextChange = [this] { pushLiveChange(); };

        // Jitter speed slider (only shown/enabled when ActionType == Jitter) - same +/- spinner
        // style as the Intensity field above.
        addAndMakeVisible(jitterSpeedLabel);
        jitterSpeedLabel.setText("Speed (cycles/s):", juce::dontSendNotification);
        jitterSpeedLabel.setJustificationType(juce::Justification::left);

        addAndMakeVisible(jitterSpeedSlider);
        jitterSpeedSlider.setSliderStyle(juce::Slider::IncDecButtons);
        jitterSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 70, 22);
        jitterSpeedSlider.setRange(-1000.0, 1000.0, 0.1);
        jitterSpeedSlider.setValue(action.getJitterSpeed(), juce::dontSendNotification);
        jitterSpeedSlider.onValueChange = [this] { pushLiveChange(); };

        // Buttons
        addAndMakeVisible(okButton);
        okButton.setButtonText("OK");
        okButton.addListener(this);
        
        addAndMakeVisible(cancelButton);
        cancelButton.setButtonText("Cancel");
        cancelButton.addListener(this);
        
        // Update initial state of all controls
        updateControlStates();

        setSize(400, 300);
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(25);
        
        const int rowHeight = 28;
        const int labelWidth = 120;
        const int controlWidth = 200;
        const int verticalSpacing = 12;
        
        // Type row
        auto typeRow = area.removeFromTop(rowHeight);
        typeLabel.setBounds(typeRow.removeFromLeft(labelWidth));
        typeCombo.setBounds(typeRow.withWidth(controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Timing row
        auto timingRow = area.removeFromTop(rowHeight);
        timingLabel.setBounds(timingRow.removeFromLeft(labelWidth));
        timingCombo.setBounds(timingRow.withWidth(controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Value row
        auto valueRow = area.removeFromTop(rowHeight);
        valueLabel.setBounds(valueRow.removeFromLeft(labelWidth));
        valueSlider.setBounds(valueRow.withWidth(controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Use start value checkbox row
        auto useStartValueRow = area.removeFromTop(rowHeight);
        useStartValueButton.setBounds(useStartValueRow.removeFromLeft(labelWidth + controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Start value row
        auto startValueRow = area.removeFromTop(rowHeight);
        startValueLabel.setBounds(startValueRow.removeFromLeft(labelWidth));
        startValueEditor.setBounds(startValueRow.withWidth(controlWidth));

        area.removeFromTop(verticalSpacing);

        // Jitter speed row
        auto jitterSpeedRow = area.removeFromTop(rowHeight);
        jitterSpeedLabel.setBounds(jitterSpeedRow.removeFromLeft(labelWidth));
        jitterSpeedSlider.setBounds(jitterSpeedRow.withWidth(controlWidth));

        area.removeFromTop(25);
        
        // Buttons - centered at bottom
        auto buttonRow = area.removeFromTop(30);
        auto buttonWidth = 80;
        auto totalButtonsWidth = buttonWidth * 2 + 10;
        auto buttonStartX = (getWidth() - totalButtonsWidth) / 2;
        
        okButton.setBounds(buttonStartX, buttonRow.getY(), buttonWidth, buttonRow.getHeight());
        cancelButton.setBounds(buttonStartX + buttonWidth + 10, buttonRow.getY(), buttonWidth, buttonRow.getHeight());
    }
    
    void paint(juce::Graphics& g) override
    {
        // Fill background
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }
    
    void buttonClicked(juce::Button* button) override
    {
        if (button == &okButton)
        {
            // Validate and apply changes - the Value/Speed sliders can only ever hold valid
            // numbers (they're sliders, not free text), so only the Start Value text field needs
            // checking here.
            auto startValueText = startValueEditor.getText();

            if (startValueText.containsOnly("-0123456789."))
            {
                pushLiveChange();

                if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                    dw->exitModalState(1);
            }
            else
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                    "Invalid Value", "Please enter valid numbers for the values.");
            }
        }
        else if (button == &cancelButton)
        {
            if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                dw->exitModalState(0);
        }
        else if (button == &useStartValueButton)
        {
            updateControlStates();
            pushLiveChange();
        }
    }

    void comboBoxChanged(juce::ComboBox* /*comboBoxThatHasChanged*/) override
    {
        updateControlStates();
        pushLiveChange();
    }

    // Invoked by the host editor (ActionClipEditor) immediately after construction, so every
    // control change here - not just the final OK click - feeds back into the Clip preview while
    // this modal dialog is still open. Safe to apply straight to targetAction on every change:
    // targetAction is always a local working copy (a freshly-constructed ActionDefinition for
    // "Add", or a copy read out of the clip for "Edit" - see ActionClipEditor::addAction/
    // editAction), only written back into the real clip by the caller if this dialog's modal
    // result is non-zero (OK, not Cancel).
    std::function<void()> onLiveChange;
    
private:
    ActionDefinition& targetAction;
    juce::String dialogTitle;
    
    juce::Label typeLabel, timingLabel, valueLabel, startValueLabel, jitterSpeedLabel;
    juce::ComboBox typeCombo, timingCombo;
    juce::TextEditor startValueEditor;
    juce::Slider valueSlider, jitterSpeedSlider;
    juce::TextButton okButton, cancelButton;
    juce::ToggleButton useStartValueButton;

    // Applies every control's current value onto targetAction and notifies the host editor, so
    // the Clip preview stays live while this dialog is open. See onLiveChange's own comment.
    void pushLiveChange()
    {
        targetAction.setAction(static_cast<ActionType>(typeCombo.getSelectedId()));
        targetAction.setTiming(static_cast<TimingType>(timingCombo.getSelectedId()));
        targetAction.setValue(valueSlider.getValue());
        if (startValueEditor.getText().containsOnly("-0123456789."))
            targetAction.setStartValue(startValueEditor.getText().getDoubleValue());
        targetAction.setUseStartValue(useStartValueButton.getToggleState());
        targetAction.setJitterSpeed(jitterSpeedSlider.getValue());

        if (onLiveChange)
            onLiveChange();
    }

    void updateControlStates()
    {
        ActionType currentAction = static_cast<ActionType>(typeCombo.getSelectedId());
        TimingType currentTiming = static_cast<TimingType>(timingCombo.getSelectedId());
        
        // Update value label first
        updateValueLabel();
        
        // Handle rotation-specific restrictions
        bool isRotation = (currentAction == ActionType::RotationX ||
                          currentAction == ActionType::RotationY ||
                          currentAction == ActionType::RotationZ);
        bool isJitter = (currentAction == ActionType::Jitter);

        if (isJitter)
        {
            // Jitter is continuous random wobble for the whole clip - it ignores TimingType and
            // has no start value concept, but needs its own Speed parameter instead.
            timingCombo.setEnabled(false);

            useStartValueButton.setEnabled(false);
            useStartValueButton.setToggleState(false, juce::dontSendNotification);
            startValueLabel.setEnabled(false);
            startValueEditor.setEnabled(false);
            startValueLabel.setVisible(false);
            startValueEditor.setVisible(false);

            jitterSpeedLabel.setVisible(true);
            jitterSpeedSlider.setVisible(true);
            jitterSpeedLabel.setEnabled(true);
            jitterSpeedSlider.setEnabled(true);

            // Default a freshly selected Jitter action to a visible intensity of 1.0 rather than
            // leaving whatever generic 0.0 the previous action type started from.
            if (valueSlider.getValue() == 0.0)
                valueSlider.setValue(1.0, juce::dontSendNotification);
        }
        else if (isRotation)
        {
            timingCombo.setEnabled(true);
            jitterSpeedLabel.setVisible(false);
            jitterSpeedSlider.setVisible(false);
            startValueLabel.setVisible(true);
            startValueEditor.setVisible(true);

            // For rotations: disable AbsoluteTarget timing and start values
            if (currentTiming == TimingType::AbsoluteTarget)
            {
                // Auto-switch to Relative During Clip if Absolute Target is selected for rotation
                timingCombo.setSelectedId((int)TimingType::RelativeDuringClip, juce::sendNotificationSync);
                currentTiming = TimingType::RelativeDuringClip;
            }

            // Disable start value controls for rotations
            useStartValueButton.setEnabled(false);
            useStartValueButton.setToggleState(false, juce::dontSendNotification);
            startValueLabel.setEnabled(false);
            startValueEditor.setEnabled(false);

            // Gray out AbsoluteTarget option in the combo box
            timingCombo.setItemEnabled((int)TimingType::AbsoluteTarget, false);
        }
        else
        {
            timingCombo.setEnabled(true);
            jitterSpeedLabel.setVisible(false);
            jitterSpeedSlider.setVisible(false);
            startValueLabel.setVisible(true);
            startValueEditor.setVisible(true);

            // For stretch: enable all timing types and start values
            useStartValueButton.setEnabled(true);

            // Re-enable AbsoluteTarget option for stretch
            timingCombo.setItemEnabled((int)TimingType::AbsoluteTarget, true);

            // Enable start value controls only for valid timing types
            ActionDefinition tempAction;
            tempAction.setTiming(currentTiming);
            bool startValueSupported = tempAction.shouldEnableStartValueControls();
            bool startValueEnabled = startValueSupported && useStartValueButton.getToggleState();

            startValueLabel.setEnabled(startValueEnabled);
            startValueEditor.setEnabled(startValueEnabled);
        }

        // Update visual appearance for disabled controls
        updateControlAppearance();
    }
    
    void updateValueLabel()
    {
        // Create temporary action to get the updated unit
        ActionDefinition tempAction;
        tempAction.setAction(static_cast<ActionType>(typeCombo.getSelectedId()));
        tempAction.setTiming(static_cast<TimingType>(timingCombo.getSelectedId()));
        
        bool isJitter = (static_cast<ActionType>(typeCombo.getSelectedId()) == ActionType::Jitter);
        juce::String unitWithTiming = tempAction.getUnitWithTiming();
        juce::String labelText = isJitter ? "Intensity" : "Value";
        if (!unitWithTiming.isEmpty())
        {
            labelText += " (" + unitWithTiming + ")";
        }
        labelText += ":";
        
        valueLabel.setText(labelText, juce::dontSendNotification);
        
        // Also update start value label if needed
        juce::String startLabelText = "Start Value";
        juce::String baseUnit = tempAction.getUnit();
        if (!baseUnit.isEmpty())
        {
            startLabelText += " (" + baseUnit + ")";
        }
        startLabelText += ":";
        startValueLabel.setText(startLabelText, juce::dontSendNotification);
    }
    
    void updateControlAppearance()
    {
        auto& lf = getLookAndFeel();
        auto normalTextColour = lf.findColour(juce::Label::textColourId);
        auto disabledTextColour = normalTextColour.withAlpha(0.4f);
        
        // Update start value controls appearance
        bool startValueEnabled = startValueLabel.isEnabled();
        startValueLabel.setColour(juce::Label::textColourId,
                                startValueEnabled ? normalTextColour : disabledTextColour);
        
        // Update use start value button appearance
        bool useStartEnabled = useStartValueButton.isEnabled();
        useStartValueButton.setColour(juce::ToggleButton::textColourId,
                                    useStartEnabled ? normalTextColour : disabledTextColour);
        
        repaint();
    }
};
