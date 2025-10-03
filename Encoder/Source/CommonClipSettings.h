#pragma once

#include <JuceHeader.h>
#include "TimelineModel.h"

class CommonClipSettings : public juce::Component, public juce::ChangeListener
{
public:
    CommonClipSettings()
    {
        createControls();
    }
    
    ~CommonClipSettings() override
    {
        if (colourSelectorPtr != nullptr)
        {
            colourSelectorPtr->removeChangeListener(this);
        }
    }
    
    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        
        const int rowHeight = 28;
        const int labelWidth = 100;  // Fixed label width
        const int verticalSpacing = 8;
        
        // Name row - field stretches to right edge
        auto nameRow = area.removeFromTop(rowHeight);
        nameLabel.setBounds(nameRow.removeFromLeft(labelWidth));
        nameEditor.setBounds(nameRow); // Takes remaining width
        
        area.removeFromTop(verticalSpacing);
        
        // Start time row - field stretches to right edge
        auto startRow = area.removeFromTop(rowHeight);
        startLabel.setBounds(startRow.removeFromLeft(labelWidth));
        startEditor.setBounds(startRow); // Takes remaining width
        
        area.removeFromTop(verticalSpacing);
        
        // Duration row - field stretches to right edge
        auto durationRow = area.removeFromTop(rowHeight);
        durationLabel.setBounds(durationRow.removeFromLeft(labelWidth));
        durationEditor.setBounds(durationRow); // Takes remaining width
        
        area.removeFromTop(verticalSpacing);
        
        // End time row - field stretches to right edge
        auto endRow = area.removeFromTop(rowHeight);
        endLabel.setBounds(endRow.removeFromLeft(labelWidth));
        endEditor.setBounds(endRow); // Takes remaining width
        
        area.removeFromTop(verticalSpacing);
        
        // Colour row - button stretches to right edge
        auto colourRow = area.removeFromTop(rowHeight);
        colourLabel.setBounds(colourRow.removeFromLeft(labelWidth));
        colourButton.setBounds(colourRow); // Takes remaining width
    }
    
    int getRequiredHeight() const
    {
        const int rowHeight = 28;
        const int verticalSpacing = 8;
        const int topBottomMargin = 10;
        
        // 5 rows (name, start, duration, end, colour) + margins
        return topBottomMargin * 2 + (rowHeight * 5) + (verticalSpacing * 4);
    }

    void setClipData(const Clip& clip)
    {
        nameEditor.setText(clip.id, false);
        startEditor.setText(juce::String(clip.start), false);
        durationEditor.setText(juce::String(clip.length), false);
        updateEndTimeDisplay();
        currentColour = clip.colour;
        updateColourButton();
    }
    
    void applyToClip(Clip& clip)
    {
        clip.id = nameEditor.getText();
        clip.start = startEditor.getText().getIntValue();
        clip.length = durationEditor.getText().getIntValue();
        clip.colour = currentColour;
    }
    
    bool validate()
    {
        auto start = startEditor.getText().getIntValue();
        auto duration = durationEditor.getText().getIntValue();
        auto end = endEditor.getText().getIntValue();
        
        if (start < 0)
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Invalid Start Time",
                                                  "Start time cannot be negative.");
            startEditor.grabKeyboardFocus();
            return false;
        }
        
        if (duration <= 0)
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Invalid Duration",
                                                  "Duration must be greater than 0.");
            durationEditor.grabKeyboardFocus();
            return false;
        }
        
        if (end < start + 10)
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                  "Invalid End Time",
                                                  "End time must be at least 10ms after start time.");
            endEditor.grabKeyboardFocus();
            return false;
        }
        
        return true;
    }
    
    juce::Colour getCurrentColour() const { return currentColour; }
    
    // ChangeListener implementation
    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (auto* selector = dynamic_cast<juce::ColourSelector*>(source))
        {
            currentColour = selector->getCurrentColour();
            updateColourButton();
            
            // Close the callout box
            if (auto* callout = findParentComponentOfClass<juce::CallOutBox>())
            {
                callout->dismiss();
            }
            
            // Remove listener and clean up
            selector->removeChangeListener(this);
            colourSelectorPtr.reset();
        }
    }
    
private:
    void createControls()
    {
        // Labels with increased font size for better readability
        addAndMakeVisible(nameLabel);
        nameLabel.setText("Name:", juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centredLeft);
        
        addAndMakeVisible(startLabel);
        startLabel.setText("Start (ms):", juce::dontSendNotification);
        startLabel.setJustificationType(juce::Justification::centredLeft);
        
        addAndMakeVisible(durationLabel);
        durationLabel.setText("Duration (ms):", juce::dontSendNotification);
        durationLabel.setJustificationType(juce::Justification::centredLeft);
        
        addAndMakeVisible(endLabel);
        endLabel.setText("End (ms):", juce::dontSendNotification);
        endLabel.setJustificationType(juce::Justification::centredLeft);
        
        addAndMakeVisible(colourLabel);
        colourLabel.setText("Colour:", juce::dontSendNotification);
        colourLabel.setJustificationType(juce::Justification::centredLeft);
        
        // Editors that will stretch to fill available width
        addAndMakeVisible(nameEditor);
        nameEditor.setTooltip("Clip Name");
        
        addAndMakeVisible(startEditor);
        startEditor.setTooltip("Start Time in milliseconds");
        startEditor.onTextChange = [this] { onStartChanged(); };
        
        addAndMakeVisible(durationEditor);
        durationEditor.setTooltip("Duration in milliseconds");
        durationEditor.onTextChange = [this] { onDurationChanged(); };
        
        addAndMakeVisible(endEditor);
        endEditor.setTooltip("End Time - Editing will adjust duration");
        endEditor.onTextChange = [this] { onEndChanged(); };
        
        // Colour button that will stretch to fill available width
        addAndMakeVisible(colourButton);
        colourButton.setTooltip("Click to choose colour");
        colourButton.onClick = [this] { showColourSelector(); };
    }
    
    void onStartChanged()
    {
        auto start = startEditor.getText().getIntValue();
        auto duration = durationEditor.getText().getIntValue();
        
        if (start >= 0 && duration > 0)
        {
            updateEndTimeDisplay();
        }
    }
    
    void onDurationChanged()
    {
        auto start = startEditor.getText().getIntValue();
        auto duration = durationEditor.getText().getIntValue();
        
        if (start >= 0 && duration > 0)
        {
            updateEndTimeDisplay();
        }
    }
    
    void onEndChanged()
    {
        auto start = startEditor.getText().getIntValue();
        auto end = endEditor.getText().getIntValue();
        
        if (start >= 0 && end > start)
        {
            // Ensure minimum 10ms duration
            auto newDuration = juce::jmax(10, end - start);
            durationEditor.setText(juce::String(newDuration), false);
            updateEndTimeDisplay(); // Re-calculate to ensure consistency
        }
    }
    
    void updateEndTimeDisplay()
    {
        auto start = startEditor.getText().getIntValue();
        auto duration = durationEditor.getText().getIntValue();
        
        if (start >= 0 && duration > 0)
        {
            endEditor.setText(juce::String(start + duration), false);
        }
    }
    
    void showColourSelector()
    {
        auto selector = std::make_unique<juce::ColourSelector>();
        selector->setName("Background");
        selector->setCurrentColour(currentColour);
        selector->setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
        selector->setSize(300, 400);
        
        selector->addChangeListener(this);
        
        // Store the selector so it doesn't get destroyed immediately
        colourSelectorPtr = std::move(selector);
        
        // Show as callout box attached to the colour button
        auto targetArea = colourButton.getScreenBounds();
        juce::CallOutBox::launchAsynchronously(std::move(colourSelectorPtr), targetArea, nullptr);
    }
    
    void updateColourButton()
    {
        colourButton.setColour(juce::TextButton::buttonColourId, currentColour);
        colourButton.setColour(juce::TextButton::textColourOffId,
                              currentColour.getPerceivedBrightness() > 0.5f ? juce::Colours::black : juce::Colours::white);
        colourButton.setButtonText(currentColour.toDisplayString(false));
    }
    
    juce::Label nameLabel, startLabel, durationLabel, endLabel, colourLabel;
    juce::TextEditor nameEditor, startEditor, durationEditor, endEditor;
    juce::TextButton colourButton;
    
    juce::Colour currentColour = juce::Colours::cornflowerblue;
    std::unique_ptr<juce::ColourSelector> colourSelectorPtr;
};
