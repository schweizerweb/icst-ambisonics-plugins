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
        
        // Buttons
        addAndMakeVisible(okButton);
        okButton.setButtonText("OK");
        okButton.addListener(this);
        
        addAndMakeVisible(cancelButton);
        cancelButton.setButtonText("Cancel");
        cancelButton.addListener(this);
        
        setSize(400, 190);
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(25);
        
        const int rowHeight = 28;
        const int labelWidth = 120;
        const int controlWidth = 200;
        const int verticalSpacing = 12;
        
        // Type row - centered in available width
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
            if (valueText.containsOnly("-0123456789."))
            {
                targetAction.setAction(static_cast<ActionType>(typeCombo.getSelectedId()));
                targetAction.setTiming(static_cast<TimingType>(timingCombo.getSelectedId()));
                targetAction.setValue(valueText.getDoubleValue());
                
                if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                    dw->exitModalState(1);
            }
            else
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                    "Invalid Value", "Please enter a valid number for the value.");
            }
        }
        else if (button == &cancelButton)
        {
            if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
                dw->exitModalState(0);
        }
    }
    
    void comboBoxChanged(juce::ComboBox* /*comboBoxThatHasChanged*/) override
    {
        updateValueLabel();
    }
    
private:
    ActionDefinition& targetAction;
    juce::String dialogTitle;
    
    juce::Label typeLabel, timingLabel, valueLabel;
    juce::ComboBox typeCombo, timingCombo;
    juce::TextEditor valueEditor;
    juce::TextButton okButton, cancelButton;
    
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
        
        // Trigger a repaint to update the layout if needed
        repaint();
    }
};
