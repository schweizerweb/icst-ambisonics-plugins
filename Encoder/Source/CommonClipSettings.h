#pragma once

#include <JuceHeader.h>
#include "TimelineModel.h"
#include "SpeedCurveComponent.h"

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

        area.removeFromTop(verticalSpacing);

        // Muted row - self-labelled toggle, no separate label needed
        mutedToggle.setBounds(area.removeFromTop(rowHeight));

        area.removeFromTop(verticalSpacing);

        // Palindrome row - self-labelled toggle, no separate label needed
        palindromeToggle.setBounds(area.removeFromTop(rowHeight));

        area.removeFromTop(verticalSpacing);

        // Repetitions row - field stretches to right edge
        auto repeatRow = area.removeFromTop(rowHeight);
        repeatCountLabel.setBounds(repeatRow.removeFromLeft(labelWidth));
        repeatCountSlider.setBounds(repeatRow);

        area.removeFromTop(verticalSpacing);

        // Speed curve row - a label above a clickable thumbnail of the clip's own curve. Only a
        // thumbnail here: the full editor needs a square canvas with room for handles and presets,
        // which would make the (already tall) movement clip editor overflow a laptop screen.
        speedCurveLabel.setBounds(area.removeFromTop(20));
        speedCurveThumb.setBounds(area.removeFromTop(speedCurveThumbHeight));
    }

    int getRequiredHeight() const
    {
        const int rowHeight = 28;
        const int verticalSpacing = 8;
        const int topBottomMargin = 10;

        // 8 rows (name, start, duration, end, colour, muted, palindrome, repetitions) + margins,
        // plus the speed curve label and thumbnail.
        return topBottomMargin * 2 + (rowHeight * 8) + (verticalSpacing * 8)
             + 20 + speedCurveThumbHeight;
    }

    // Fired when the speed curve is edited in its popup, so the host editor can push the change
    // into its clip preview immediately instead of waiting for the next 150ms poll - the curve is
    // dragged continuously, and a lagging preview makes it impossible to judge.
    std::function<void()> onCurveEdited;

    // Read-only live value of the Duration field, independent of applyToClip()/Apply - for callers
    // (the clip preview) that need to react to edits as they happen rather than only once applied.
    ms_t getLiveLength() const
    {
        return parseTimeValue(durationEditor.getText(), displayInSeconds);
    }

    // Same contract as getLiveLength(), for the clip preview's live polling.
    bool getLivePalindrome() const { return palindromeToggle.getToggleState(); }
    int getLiveRepeatCount() const { return juce::jmax(1, (int)repeatCountSlider.getValue()); }
    AnimatorMath::EasingCurve getLiveEasing() const { return speedCurveThumb.getCurve(); }

    // Called by the host editor whenever something that determines whether a non-palindrome repeat
    // would visibly jump changes - ActionClipEditor calls this whenever its action list changes
    // (true if it contains a Rotation action), MovementClipEditor calls it whenever the movement
    // type changes (true for MoveTo, which always jumps from target back to start between repeats
    // unless Palindrome is on) - see enforcePalindromeConstraint()'s comment for why both need this.
    void setPalindromeRequiredForRepeat(bool required)
    {
        palindromeRequiredForRepeat = required;
        enforcePalindromeConstraint();
    }

    void setClipData(const Clip& clip)
    {
        nameEditor.setText(clip.id, false);
        startEditor.setText(formatTimeValue(clip.start, displayInSeconds), false);
        durationEditor.setText(formatTimeValue(clip.length, displayInSeconds), false);
        updateEndTimeDisplay();
        currentColour = clip.colour;
        updateColourButton();
        mutedToggle.setToggleState(clip.muted, juce::dontSendNotification);
        palindromeToggle.setToggleState(clip.palindrome, juce::dontSendNotification);
        repeatCountSlider.setValue(clip.repetitions, juce::dontSendNotification);

        AnimatorMath::EasingCurve curve;
        curve.x1 = clip.easeX1; curve.y1 = clip.easeY1;
        curve.x2 = clip.easeX2; curve.y2 = clip.easeY2;
        curve.enabled = clip.easingEnabled;
        curve.perRepetition = clip.easePerRepetition;
        speedCurveThumb.setCurve(curve);

        enforcePalindromeConstraint();
    }

    void applyToClip(Clip& clip)
    {
        clip.id = nameEditor.getText();
        clip.start = parseTimeValue(startEditor.getText(), displayInSeconds);
        clip.length = parseTimeValue(durationEditor.getText(), displayInSeconds);
        clip.colour = currentColour;
        clip.muted = mutedToggle.getToggleState();
        clip.repetitions = juce::jmax(1, (int)repeatCountSlider.getValue());
        clip.palindrome = palindromeToggle.getToggleState();
        if (palindromeRequiredForRepeat && clip.repetitions > 1)
            clip.palindrome = true; // defensive re-clamp - enforcePalindromeConstraint() already makes this unreachable through the UI

        const auto curve = speedCurveThumb.getCurve();
        clip.easeX1 = curve.x1; clip.easeY1 = curve.y1;
        clip.easeX2 = curve.x2; clip.easeY2 = curve.y2;
        clip.easingEnabled = curve.enabled;
        clip.easePerRepetition = curve.perRepetition;
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

        addAndMakeVisible(mutedToggle);
        mutedToggle.setButtonText("Muted (excluded from playback)");
        mutedToggle.setTooltip("Keeps the clip's data but skips it during playback - toggle with the 'M' key on the timeline too");

        addAndMakeVisible(palindromeToggle);
        palindromeToggle.setButtonText("Palindrome (play forward then backward)");
        palindromeToggle.onClick = [this] { enforcePalindromeConstraint(); };

        addAndMakeVisible(repeatCountLabel);
        repeatCountLabel.setText("Repetitions:", juce::dontSendNotification);
        repeatCountLabel.setJustificationType(juce::Justification::centredLeft);

        addAndMakeVisible(repeatCountSlider);
        repeatCountSlider.setSliderStyle(juce::Slider::IncDecButtons);
        repeatCountSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 70, 22);
        repeatCountSlider.setRange(1, 100, 1); // 100 is a generous practical ceiling, not an engine limit
        repeatCountSlider.setValue(1, juce::dontSendNotification);
        repeatCountSlider.onValueChange = [this] { enforcePalindromeConstraint(); };

        addAndMakeVisible(speedCurveLabel);
        speedCurveLabel.setText("Speed curve (click to edit):", juce::dontSendNotification);
        speedCurveLabel.setJustificationType(juce::Justification::centredLeft);

        addAndMakeVisible(speedCurveThumb);
        speedCurveThumb.setTooltip("Shape how the clip's progress runs over its length - "
                                   "the diagonal is constant speed.");
        speedCurveThumb.onClicked = [this] { openSpeedCurveEditor(); };

        enforcePalindromeConstraint();
    }

    // The editor is a popup rather than inline: it needs a square canvas plus presets, which would
    // push the (already tall) movement clip editor past what fits on a laptop screen.
    void openSpeedCurveEditor()
    {
        if (speedCurveWindow != nullptr)
        {
            speedCurveWindow->toFront(true);
            return;
        }

        auto content = std::make_unique<SpeedCurveEditorComponent>();
        auto* raw = content.get();
        raw->setCurve(speedCurveThumb.getCurve());

        // The scope choice does nothing with a single forward cycle, so it is hidden there rather
        // than left as a control that silently has no effect.
        raw->setScopeRelevant(getLiveRepeatCount() > 1 || getLivePalindrome());

        raw->onCurveChanged = [this](const AnimatorMath::EasingCurve& c)
        {
            speedCurveThumb.setCurve(c);
            if (onCurveEdited)
                onCurveEdited();
        };

        // The close is dispatched asynchronously (the callback destroys the window, which must not
        // happen inside its own close-button dispatch), so this must survive the editor being torn
        // down in the meantime.
        juce::Component::SafePointer<CommonClipSettings> safeThis(this);
        speedCurveWindow = std::make_unique<SpeedCurveDialog>(std::move(content),
            [safeThis]() mutable { if (safeThis != nullptr) safeThis->speedCurveWindow.reset(); });
        speedCurveWindow->setVisible(true);
    }

    // Two unrelated reasons a non-palindrome repeat would visibly jump at each repeat boundary,
    // both reported through the same flag (setPalindromeRequiredForRepeat()):
    //  - A Rotation action accumulates its angle as a RELATIVE rotation of the group's current
    //    orientation (see AnimatorMath::computeCycleState's comment), not an absolute set - unlike
    //    Movement/Stretch, repeating it without Palindrome would need a dedicated "undo the
    //    accumulated rotation" correction at each repeat boundary, which this feature deliberately
    //    does not implement.
    //  - A MoveTo movement (Cartesian or Polar) ends at a different point than it started (that's
    //    the whole point of it) - repeating it without Palindrome snaps instantly from the target
    //    back to the start at every repeat boundary, a jump Circle/Spiral don't have the same way
    //    (they return at least close to their start angle/radius every pass) and Stretch/Rotation
    //    don't need Palindrome to avoid (Stretch is a pure function of progress, so its own
    //    non-palindrome repeat is a deliberate, expected sawtooth, not a bug).
    // Either way, whenever Repetitions is above 1, Palindrome is locked on - disabled rather than
    // merely warned, so an invalid combination can never be created through the UI at all.
    void enforcePalindromeConstraint()
    {
        const bool mustForcePalindrome = palindromeRequiredForRepeat && repeatCountSlider.getValue() > 1;

        if (mustForcePalindrome && !palindromeToggle.getToggleState())
            palindromeToggle.setToggleState(true, juce::dontSendNotification);

        palindromeToggle.setEnabled(!mustForcePalindrome);
        palindromeToggle.setAlpha(mustForcePalindrome ? 0.7f : 1.0f);
        palindromeToggle.setTooltip(mustForcePalindrome
            ? "Locked on: this clip's content would jump at each repeat boundary without Palindrome"
            : "Each repetition plays the clip's motion forward, then backward, instead of just forward");
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
    juce::ToggleButton mutedToggle;
    juce::ToggleButton palindromeToggle;
    juce::Label repeatCountLabel;
    juce::Slider repeatCountSlider;
    bool palindromeRequiredForRepeat = false;

    static constexpr int speedCurveThumbHeight = 44;
    juce::Label speedCurveLabel;
    SpeedCurveComponent speedCurveThumb { SpeedCurveComponent::Mode::Thumbnail };
    std::unique_ptr<SpeedCurveDialog> speedCurveWindow;

    juce::Colour currentColour = juce::Colours::cornflowerblue;
    std::unique_ptr<juce::ColourSelector> colourSelectorPtr;
    bool displayInSeconds = true;
    TimeValueInputFilter timeValueInputFilter { displayInSeconds };
};
