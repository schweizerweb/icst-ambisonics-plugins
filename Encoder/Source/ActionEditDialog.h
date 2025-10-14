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
        
        // Value editor
        addAndMakeVisible(valueLabel);
        updateValueLabel();
        
        addAndMakeVisible(valueEditor);
        valueEditor.setText(juce::String(action.getValue()), juce::dontSendNotification);
        
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
        
        // Buttons
        addAndMakeVisible(okButton);
        okButton.setButtonText("OK");
        okButton.addListener(this);
        
        addAndMakeVisible(cancelButton);
        cancelButton.setButtonText("Cancel");
        cancelButton.addListener(this);
        
        // Update initial state of all controls
        updateControlStates();
        
        setSize(400, 260);
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
        valueEditor.setBounds(valueRow.withWidth(controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Use start value checkbox row
        auto useStartValueRow = area.removeFromTop(rowHeight);
        useStartValueButton.setBounds(useStartValueRow.removeFromLeft(labelWidth + controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // Start value row
        auto startValueRow = area.removeFromTop(rowHeight);
        startValueLabel.setBounds(startValueRow.removeFromLeft(labelWidth));
        startValueEditor.setBounds(startValueRow.withWidth(controlWidth));
        
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
            // Validate and apply changes
            auto valueText = valueEditor.getText();
            auto startValueText = startValueEditor.getText();
            
            if (valueText.containsOnly("-0123456789.") &&
                startValueText.containsOnly("-0123456789."))
            {
                ActionType actionType = static_cast<ActionType>(typeCombo.getSelectedId());
                TimingType timingType = static_cast<TimingType>(timingCombo.getSelectedId());
                
                // Apply the settings
                targetAction.setAction(actionType);
                targetAction.setTiming(timingType);
                targetAction.setValue(valueText.getDoubleValue());
                targetAction.setStartValue(startValueText.getDoubleValue());
                targetAction.setUseStartValue(useStartValueButton.getToggleState());
                
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
        }
    }
    
    void comboBoxChanged(juce::ComboBox* /*comboBoxThatHasChanged*/) override
    {
        updateControlStates();
    }
    
private:
    ActionDefinition& targetAction;
    juce::String dialogTitle;
    
    juce::Label typeLabel, timingLabel, valueLabel, startValueLabel;
    juce::ComboBox typeCombo, timingCombo;
    juce::TextEditor valueEditor, startValueEditor;
    juce::TextButton okButton, cancelButton;
    juce::ToggleButton useStartValueButton;
    
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
        
        if (isRotation)
        {
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
            // For stretch: enable all timing types and start values
            useStartValueButton.setEnabled(true);
            
            // Re-enable AbsoluteTarget option for stretch
            timingCombo.setItemEnabled((int)TimingType::AbsoluteTarget, true);
            
            // Enable start value controls only for valid timing types
            bool startValueSupported = (currentTiming == TimingType::AbsoluteTarget ||
                                      currentTiming == TimingType::RelativeDuringClip);
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
        
        juce::String unitWithTiming = tempAction.getUnitWithTiming();
        juce::String labelText = "Value";
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
