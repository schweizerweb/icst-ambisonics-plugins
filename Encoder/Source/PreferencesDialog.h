#pragma once
#include "JuceHeader.h"
#include "../../Common/AdditionalWindow.h"

// Content shown inside PreferencesDialog. Each setting applies immediately when changed (there's
// no separate Apply/OK step) since these are simple display preferences, not destructive actions.
class PreferencesOptionsComponent : public juce::Component
{
public:
    PreferencesOptionsComponent(bool initialDisplayInSeconds,
                                std::function<void(bool displayInSeconds)> onTimeUnitChanged,
                                std::function<void()> onClose)
        : onTimeUnitChanged(std::move(onTimeUnitChanged)), onClose(std::move(onClose))
    {
        timeUnitLabel.setText("Time Unit:", juce::dontSendNotification);
        addAndMakeVisible(timeUnitLabel);

        millisecondsButton.setButtonText("Milliseconds");
        millisecondsButton.setRadioGroupId(timeUnitRadioGroupId, juce::dontSendNotification);
        millisecondsButton.onClick = [this] { notifyTimeUnitChanged(false); };
        addAndMakeVisible(millisecondsButton);

        secondsButton.setButtonText("Seconds");
        secondsButton.setRadioGroupId(timeUnitRadioGroupId, juce::dontSendNotification);
        secondsButton.onClick = [this] { notifyTimeUnitChanged(true); };
        addAndMakeVisible(secondsButton);

        (initialDisplayInSeconds ? secondsButton : millisecondsButton).setToggleState(true, juce::dontSendNotification);

        closeButton.setButtonText("Close");
        closeButton.onClick = [this] { requestClose(); };
        addAndMakeVisible(closeButton);

        setSize(300, getTotalRequiredHeight());
    }

    int getTotalRequiredHeight() const
    {
        return 2 * margin
             + 3 * (rowHeight + rowSpacing) // label, milliseconds, seconds
             + rowHeight;                   // close row (no trailing gap)
    }

private:
    void paint(juce::Graphics& g) override
    {
        // AdditionalWindow's DialogWindow background is hardcoded white; without this, Labels
        // (which paint a transparent background) use this app's light/white text colour and
        // become invisible. Matches the other animator dialogs for the same reason.
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(margin);

        timeUnitLabel.setBounds(area.removeFromTop(rowHeight));
        area.removeFromTop(rowSpacing);
        millisecondsButton.setBounds(area.removeFromTop(rowHeight));
        area.removeFromTop(rowSpacing);
        secondsButton.setBounds(area.removeFromTop(rowHeight));
        area.removeFromTop(rowSpacing);

        auto buttonRow = area.removeFromTop(rowHeight);
        closeButton.setBounds(buttonRow.removeFromRight(90));
    }

    void notifyTimeUnitChanged(bool displayInSeconds)
    {
        if (onTimeUnitChanged)
            onTimeUnitChanged(displayInSeconds);
    }

    void requestClose()
    {
        // onClose deletes this component's own window. Deleting it here, synchronously, would
        // destroy closeButton while its own click-dispatch is still unwinding on this exact call
        // stack - a use-after-free. Defer to the next message loop turn instead, so the button's
        // click handling has fully returned first (same reasoning as ImportSceneOptionsComponent).
        if (onClose)
        {
            auto callback = onClose;
            juce::MessageManager::callAsync([callback] { callback(); });
        }
    }

    static constexpr int timeUnitRadioGroupId = 2001;
    static constexpr int rowHeight = 26;
    static constexpr int rowSpacing = 6;
    static constexpr int margin = 10;

    std::function<void(bool)> onTimeUnitChanged;
    std::function<void()> onClose;

    juce::Label timeUnitLabel;
    juce::ToggleButton millisecondsButton, secondsButton;
    juce::TextButton closeButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreferencesOptionsComponent)
};

// Modal window hosting PreferencesOptionsComponent. The X/close button behaves like Close.
class PreferencesDialog : public AdditionalWindow
{
public:
    PreferencesDialog(std::unique_ptr<juce::Component> content, std::function<void()> onCloseRequested)
        : AdditionalWindow("Preferences", content.get()), closeCallback(std::move(onCloseRequested))
    {
        const int w = content->getWidth();
        const int h = content->getHeight();

        setAlwaysOnTop(true);
        setContentOwned(content.release(), true);
        setResizable(false, false);
        setUsingNativeTitleBar(false);

        // getTitleBarHeight() reflects the custom (non-native) title bar only once
        // setUsingNativeTitleBar(false) has already taken effect - must be queried after it.
        const int titleBarHeight = getTitleBarHeight();
        setSize(w, h + titleBarHeight);
        centreWithSize(getWidth(), getHeight());
    }

    void closeButtonPressed() override
    {
        // Deferred for the same reason as PreferencesOptionsComponent::requestClose().
        if (closeCallback)
        {
            auto callback = closeCallback;
            juce::MessageManager::callAsync([callback] { callback(); });
        }
    }

private:
    std::function<void()> closeCallback;
};
