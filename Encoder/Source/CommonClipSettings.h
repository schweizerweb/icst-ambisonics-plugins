#pragma once

#include <JuceHeader.h>
#include "TimelineModel.h"

// Restricts typed input to what the underlying ms_t storage can actually represent, so the field
// doesn't invite precision it will silently round away: digits only in milliseconds mode, and
// digits with at most one "." and up to 3 digits after it (1ms resolution) in seconds mode.
class TimeValueInputFilter : public juce::TextEditor::InputFilter
{
public:
    explicit TimeValueInputFilter(const bool& displayInSecondsFlag) : displayInSeconds(displayInSecondsFlag) {}

    juce::String filterNewText(juce::TextEditor& editor, const juce::String& newInput) override
    {
        if (!displayInSeconds)
        {
            juce::String digitsOnly;
            for (auto c : newInput)
                if (juce::CharacterFunctions::isDigit(c))
                    digitsOnly += juce::String::charToString(c);
            return digitsOnly;
        }

        const auto existing = editor.getText();
        const auto selection = editor.getHighlightedRegion();
        const auto before = existing.substring(0, selection.getStart());
        const auto after = existing.substring(selection.getEnd());

        juce::String accepted;
        for (auto c : newInput)
        {
            if (!juce::CharacterFunctions::isDigit(c) && c != '.')
                continue;

            const auto candidate = before + accepted + juce::String::charToString(c) + after;

            if (c == '.' && candidate.indexOfChar('.') != candidate.lastIndexOfChar('.'))
                continue; // a decimal point already exists elsewhere in the field

            const auto dotIndex = candidate.indexOfChar('.');
            if (dotIndex >= 0 && candidate.length() - dotIndex - 1 > 3)
                continue; // would exceed the 1ms (3-decimal) resolution

            accepted += juce::String::charToString(c);
        }

        return accepted;
    }

private:
    const bool& displayInSeconds;
};

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
        startEditor.setText(formatTimeValue(clip.start, displayInSeconds), false);
        durationEditor.setText(formatTimeValue(clip.length, displayInSeconds), false);
        updateEndTimeDisplay();
        currentColour = clip.colour;
        updateColourButton();
    }

    void applyToClip(Clip& clip)
    {
        clip.id = nameEditor.getText();
        clip.start = parseTimeValue(startEditor.getText(), displayInSeconds);
        clip.length = parseTimeValue(durationEditor.getText(), displayInSeconds);
        clip.colour = currentColour;
    }

    // Display only - re-renders whatever is currently shown in the new unit, the underlying
    // ms_t clip data (read via applyToClip()) is never affected by this.
    void setDisplayInSeconds(bool useSeconds)
    {
        if (displayInSeconds == useSeconds)
            return;

        const ms_t start = parseTimeValue(startEditor.getText(), displayInSeconds);
        const ms_t duration = parseTimeValue(durationEditor.getText(), displayInSeconds);

        displayInSeconds = useSeconds;
        updateLabelsForUnit();

        startEditor.setText(formatTimeValue(start, displayInSeconds), false);
        durationEditor.setText(formatTimeValue(duration, displayInSeconds), false);
        updateEndTimeDisplay();
    }

    bool validate()
    {
        auto start = parseTimeValue(startEditor.getText(), displayInSeconds);
        auto duration = parseTimeValue(durationEditor.getText(), displayInSeconds);
        auto end = parseTimeValue(endEditor.getText(), displayInSeconds);

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
                                                  "End time must be at least " + formatTimeValueWithUnit(10, displayInSeconds) + " after start time.");
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
        startLabel.setJustificationType(juce::Justification::centredLeft);

        addAndMakeVisible(durationLabel);
        durationLabel.setJustificationType(juce::Justification::centredLeft);

        addAndMakeVisible(endLabel);
        endLabel.setJustificationType(juce::Justification::centredLeft);

        updateLabelsForUnit();

        addAndMakeVisible(colourLabel);
        colourLabel.setText("Colour:", juce::dontSendNotification);
        colourLabel.setJustificationType(juce::Justification::centredLeft);

        // Editors that will stretch to fill available width
        addAndMakeVisible(nameEditor);
        nameEditor.setTooltip("Clip Name");

        addAndMakeVisible(startEditor);
        startEditor.setTooltip("Start Time");
        startEditor.setJustification(juce::Justification::centredRight);
        startEditor.setInputFilter(&timeValueInputFilter, false);
        startEditor.onTextChange = [this] { onStartChanged(); };

        addAndMakeVisible(durationEditor);
        durationEditor.setTooltip("Duration");
        durationEditor.setJustification(juce::Justification::centredRight);
        durationEditor.setInputFilter(&timeValueInputFilter, false);
        durationEditor.onTextChange = [this] { onDurationChanged(); };

        addAndMakeVisible(endEditor);
        endEditor.setTooltip("End Time - Editing will adjust duration");
        endEditor.setJustification(juce::Justification::centredRight);
        endEditor.setInputFilter(&timeValueInputFilter, false);
        endEditor.onTextChange = [this] { onEndChanged(); };
        endEditor.onFocusLost = [this] { updateEndTimeDisplay(); };
        
        // Colour button that will stretch to fill available width
        addAndMakeVisible(colourButton);
        colourButton.setTooltip("Click to choose colour");
        colourButton.onClick = [this] { showColourSelector(); };
    }
    
    void onStartChanged()
    {
        auto start = parseTimeValue(startEditor.getText(), displayInSeconds);
        auto duration = parseTimeValue(durationEditor.getText(), displayInSeconds);

        if (start >= 0 && duration > 0)
        {
            updateEndTimeDisplay();
        }
    }

    void onDurationChanged()
    {
        auto start = parseTimeValue(startEditor.getText(), displayInSeconds);
        auto duration = parseTimeValue(durationEditor.getText(), displayInSeconds);

        if (start >= 0 && duration > 0)
        {
            updateEndTimeDisplay();
        }
    }

    void onEndChanged()
    {
        auto start = parseTimeValue(startEditor.getText(), displayInSeconds);
        auto end = parseTimeValue(endEditor.getText(), displayInSeconds);

        if (start >= 0 && end > start)
        {
            // Ensure minimum 10ms duration
            ms_t newDuration = juce::jmax<ms_t>(10, end - start);
            durationEditor.setText(formatTimeValue(newDuration, displayInSeconds), false);
            updateEndTimeDisplay(); // Re-calculate to ensure consistency
        }
    }

    void updateEndTimeDisplay()
    {
        // Don't rewrite the end field while the user is actively typing in it - onEndChanged()
        // calls this after every keystroke, and re-rendering the canonical (rounded, trailing-
        // zero-trimmed) text here would fight their typing, e.g. deleting the "." as soon as it's
        // entered. It's reformatted once they move on, via onFocusLost below.
        if (endEditor.hasKeyboardFocus(true))
            return;

        auto start = parseTimeValue(startEditor.getText(), displayInSeconds);
        auto duration = parseTimeValue(durationEditor.getText(), displayInSeconds);

        if (start >= 0 && duration > 0)
        {
            endEditor.setText(formatTimeValue(start + duration, displayInSeconds), false);
        }
    }

    void updateLabelsForUnit()
    {
        const juce::String unit = displayInSeconds ? " (s):" : " (ms):";
        startLabel.setText("Start" + unit, juce::dontSendNotification);
        durationLabel.setText("Duration" + unit, juce::dontSendNotification);
        endLabel.setText("End" + unit, juce::dontSendNotification);
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
    bool displayInSeconds = true;
    TimeValueInputFilter timeValueInputFilter { displayInSeconds };
};
