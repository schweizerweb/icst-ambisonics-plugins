#pragma once
#include "JuceHeader.h"
#include "TimelineComponent.h"
#include "TimelineWidgetMS.h"

// Action-String analog zu ACTION_CLOSE_ANIMATOR
static const juce::String ACTION_CLOSE_TIMELINE = "ACTION_CLOSE_TIMELINE";

// ---------------------------------- Dialog -----------------------------------
class TimelineDialog  : public juce::DocumentWindow,
                        public juce::ActionBroadcaster,
                        private juce::Timer
{
public:
    TimelineDialog(juce::ActionListener* listener, juce::Component* content)
        : juce::DocumentWindow("Animator",
                               juce::Colours::darkgrey,
                               DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(false);
        setResizable(true, true);
        setContentOwned(content, true);
        setAlwaysOnTop(true);
        setVisible(false);

        if (listener != nullptr)
            addActionListener(listener);

        startIntroWarning();
    }

    ~TimelineDialog() override = default;

    void updatePosition(juce::Rectangle<int> parentScreenBounds,
                        int w = 900, int h = 500)
    {
        auto display = juce::Desktop::getInstance()
                           .getDisplays().getPrimaryDisplay()->userArea;

        auto right = juce::Rectangle<int>(parentScreenBounds.getRight() + 12,
                                          parentScreenBounds.getY(), w, h);
        if (display.contains(right))
            setBounds(right);
        else
            centreWithSize(w, h);
    }

    void closeButtonPressed() override
    {
        sendActionMessage(ACTION_CLOSE_TIMELINE);
    }

    void paintOverChildren(juce::Graphics& g) override
    {
        // permanent compact badge
        auto badge = getCompactWarningBounds().toFloat();

        g.setColour(juce::Colour(0xffc46a1a).withAlpha(0.72f));
        g.fillRoundedRectangle(badge, 10.0f);

        g.setColour(juce::Colours::white.withAlpha(0.82f));
        g.drawRoundedRectangle(badge, 10.0f, 1.0f);

        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(14.0f, juce::Font::bold));
        g.drawFittedText("Experimental - compatibility not guaranteed",
                         badge.reduced(10.0f, 5.0f).toNearestInt(),
                         juce::Justification::centred, 1);

        // large intro overlay fades out completely
        if (showLargeOverlay)
        {
            const float a = overlayAlpha;

            g.setColour(juce::Colours::black.withAlpha(0.50f * a));
            g.fillAll();

            auto full = getLocalBounds().toFloat();
            juce::Rectangle<float> panel(
                full.getCentreX() - 280.0f,
                full.getCentreY() - 85.0f,
                560.0f, 170.0f);

            g.setColour(juce::Colour(0xffc46a1a).withAlpha(0.93f * a));
            g.fillRoundedRectangle(panel, 12.0f);

            g.setColour(juce::Colours::white.withAlpha(0.88f * a));
            g.drawRoundedRectangle(panel, 12.0f, 1.5f);

            auto text = panel.reduced(16.0f).toNearestInt();

            g.setColour(juce::Colours::white.withAlpha(a));
            g.setFont(juce::Font(22.0f, juce::Font::bold));
            g.drawFittedText("Experimental Animator", text.removeFromTop(34),
                             juce::Justification::centredTop, 1);

            g.setFont(juce::Font(15.5f));
            g.drawFittedText(
                "This version is still experimental.\n"
                "Future compatibility of animations created with it\n"
                "is not guaranteed.",
                text, juce::Justification::centredTop, 3);
        }
    }

private:
    bool showLargeOverlay = true;
    float overlayAlpha = 1.0f;
    bool fadeStarted = false;

    void startIntroWarning()
    {
        showLargeOverlay = true;
        overlayAlpha = 1.0f;
        fadeStarted = false;

        juce::Timer::callAfterDelay(5000, [safe = juce::Component::SafePointer<TimelineDialog>(this)]()
        {
            if (safe != nullptr)
                safe->beginFadeOut();
        });
    }

    void beginFadeOut()
    {
        if (fadeStarted)
            return;

        fadeStarted = true;
        startTimerHz(30);
    }

    void timerCallback() override
    {
        overlayAlpha -= 0.07f;

        if (overlayAlpha <= 0.0f)
        {
            overlayAlpha = 0.0f;
            showLargeOverlay = false;
            stopTimer();
        }

        repaint();
    }

    juce::Rectangle<int> getCompactWarningBounds() const
    {
        const int margin = 6;
        const int titleBarOffset = 24;
        const int w = 300;
        const int h = 32;

        return { getWidth() - w - margin, titleBarOffset + margin, w, h };
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

    void actionListenerCallback(const juce::String& message) override
    {
        if (message == ACTION_CLOSE_TIMELINE)
        {
            delete window;
            window = nullptr;
        }
    }

    void show(juce::Component* pParent,
              AmbisonicEncoderAudioProcessor* pProcessor,
              PointSelection* pPointSelection,
              AnimatorEngine* pEngine)
    {
        if (window != nullptr)
            delete window;

        auto* timeline = new TimelineWidgetMS(pEngine);
        timeline->setModels(pProcessor->getTimelines());
        timeline->setSourceSet(pProcessor->getSources());
        timeline->setSelectionControl(pPointSelection);
        timeline->setPlayheadProvider([pProcessor]() -> PlayheadSnapshot
        {
            PlayheadSnapshot s;
            const auto& st = pProcessor->playheadState;

            s.valid = st.valid.load(std::memory_order_acquire);
            if (!s.valid) return s;

            const double secs = st.timeSeconds.load(std::memory_order_relaxed);
            if (!std::isfinite(secs)) return s;

            s.timeMs = (ms_t) std::llround(secs * 1000.0);
            s.playing = st.playing.load(std::memory_order_relaxed);
            s.bpm = st.bpm.load(std::memory_order_relaxed);

            s.looping = st.looping.load(std::memory_order_relaxed);
            if (s.looping)
            {
                const double ls = st.loopStartSeconds.load(std::memory_order_relaxed);
                const double le = st.loopEndSeconds.load(std::memory_order_relaxed);

                if (std::isfinite(ls) && std::isfinite(le) && le > ls)
                {
                    s.loopStartMs = (ms_t) std::llround(ls * 1000.0);
                    s.loopEndMs = (ms_t) std::llround(le * 1000.0);
                }
                else
                {
                    s.looping = false;
                }
            }

            return s;
        });

        window = new TimelineDialog(this, timeline);

        window->updatePosition(
            pParent != nullptr
                ? pParent->getScreenBounds()
                : juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea);

        window->setVisible(true);
        window->toFront(true);
    }

    bool isOpen() const { return window != nullptr; }

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