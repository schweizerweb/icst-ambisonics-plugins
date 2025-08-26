/*
================================================================================
    Beispiel-Implementierung eines Dialog-Managers nach dem gezeigten Vorbild.
    Abhängig von JUCE. Zeigt ein TimelineWidgetMS in einem ResizableWindow.
================================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "TimelineComponent.h"

// Action-String analog zu ACTION_CLOSE_ANIMATOR
static const juce::String ACTION_CLOSE_TIMELINE = "ACTION_CLOSE_TIMELINE";

// ---------------------------------- Dialog -----------------------------------
class TimelineDialog : public juce::DocumentWindow,
    public juce::ActionBroadcaster
{
public:
    TimelineDialog(juce::ActionListener* listener, juce::Component* content)
        : juce::DocumentWindow("Timeline",
            juce::Colours::darkgrey,
            DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setResizable(true, true);
        setContentOwned(content, true);
        setAlwaysOnTop(false);
        setVisible(false);

        if (listener != nullptr)
            addActionListener(listener);
    }

    ~TimelineDialog() override = default;

    void updatePosition(juce::Rectangle<int> parentScreenBounds,
        int w = 900, int h = 500)
    {
        auto display = juce::Desktop::getInstance()
            .getDisplays().getPrimaryDisplay()->userArea;
        juce::Rectangle<int> target(w, h);

        auto right = juce::Rectangle<int>(parentScreenBounds.getRight() + 12,
            parentScreenBounds.getY(), w, h);
        if (display.contains(right))
            setBounds(right);
        else
            centreWithSize(w, h);
    }

    // jetzt passt override:
    void closeButtonPressed() override
    {
        sendActionMessage(ACTION_CLOSE_TIMELINE);
    }
};


// ------------------------------- Dialog-Manager -------------------------------
class TimelineDialogManager : public juce::ActionListener
{
public:
    TimelineDialogManager() = default;

    ~TimelineDialogManager() override
    {
        if (window != nullptr)
        {
            delete window;
            window = nullptr;
        }
    }

    // ActionListener: reagiert auf CLOSE und räumt auf
    void actionListenerCallback(const juce::String& message) override
    {
        if (message == ACTION_CLOSE_TIMELINE)
        {
            delete window;
            window = nullptr;
        }
    }

    // Dialog öffnen; pParent bestimmt Referenz-Position
    void show(juce::Component* pParent)
    {
        if (window != nullptr)
            delete window;

        // Inhalt: dein TimelineWidgetMS
        auto* timeline = new TimelineWidgetMS();

        // Dialog erzeugen; Manager (this) als Listener registrieren
        window = new TimelineDialog(this, timeline);

        // Position relativ zum Parent (wie im Vorbild)
        window->updatePosition(pParent != nullptr ? pParent->getScreenBounds()
                                                  : juce::Desktop::getInstance().getDisplays()
                                                        .getPrimaryDisplay()->userArea);
        window->setVisible(true);
        window->toFront(true);
    }

    // Optional: Dialog gerade geöffnet?
    bool isOpen() const { return window != nullptr; }

    // Optional: explizit schließen (z. B. aus externer Logik)
    void close()
    {
        if (window != nullptr)
        {
            delete window;
            window = nullptr;
        }
    }

private:
    TimelineDialog* window = nullptr;
};
