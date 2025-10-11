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
        
        // NEW: Use start value checkbox
        addAndMakeVisible(useStartValueButton);
        useStartValueButton.setButtonText("Use Start Value");
        useStartValueButton.setToggleState(action.getUseStartValue(), juce::dontSendNotification);
        useStartValueButton.addListener(this);
        
        // NEW: Start value editor
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
        
        // Update initial state of start value controls
        updateStartValueControls();
        
        setSize(400, 260); // Increased height to accommodate new controls
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
        
        // NEW: Use start value checkbox row
        auto useStartValueRow = area.removeFromTop(rowHeight);
        useStartValueButton.setBounds(useStartValueRow.removeFromLeft(labelWidth + controlWidth));
        
        area.removeFromTop(verticalSpacing);
        
        // NEW: Start value row
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
                targetAction.setAction(static_cast<ActionType>(typeCombo.getSelectedId()));
                targetAction.setTiming(static_cast<TimingType>(timingCombo.getSelectedId()));
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
            updateStartValueControls();
        }
    }
    
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &timingCombo)
        {
            updateStartValueControls();
        }
        updateValueLabel();
    }
    
private:
    ActionDefinition& targetAction;
    juce::String dialogTitle;
    
    juce::Label typeLabel, timingLabel, valueLabel, startValueLabel;
    juce::ComboBox typeCombo, timingCombo;
    juce::TextEditor valueEditor, startValueEditor;
    juce::TextButton okButton, cancelButton;
    juce::ToggleButton useStartValueButton;
    
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
        if (!unitWithTiming.isEmpty())
        {
            // For start value, we don't include the "/s" for ConstantPerSecond
            juce::String baseUnit = tempAction.getUnit();
            if (!baseUnit.isEmpty())
            {
                startLabelText += " (" + baseUnit + ")";
            }
        }
        startLabelText += ":";
        startValueLabel.setText(startLabelText, juce::dontSendNotification);
        
        // Trigger a repaint to update the layout if needed
        repaint();
    }
    
    void updateStartValueControls()
    {
        TimingType currentTiming = static_cast<TimingType>(timingCombo.getSelectedId());
        bool shouldEnable = (currentTiming == TimingType::AbsoluteTarget ||
                           currentTiming == TimingType::RelativeDuringClip);
        
        // Enable/disable the checkbox based on timing type
        useStartValueButton.setEnabled(shouldEnable);
        
        // If timing type doesn't support start value, uncheck and disable
        if (!shouldEnable)
        {
            useStartValueButton.setToggleState(false, juce::dontSendNotification);
        }
        
        // Enable/disable start value controls based on checkbox state
        bool startValueEnabled = shouldEnable && useStartValueButton.getToggleState();
        startValueLabel.setEnabled(startValueEnabled);
        startValueEditor.setEnabled(startValueEnabled);
        
        // Update start value label appearance based on enabled state
        if (startValueEnabled)
        {
            startValueLabel.setColour(juce::Label::textColourId,
                                    getLookAndFeel().findColour(juce::Label::textColourId));
        }
        else
        {
            startValueLabel.setColour(juce::Label::textColourId,
                                    getLookAndFeel().findColour(juce::Label::textColourId).withAlpha(0.5f));
        }
    }
};
