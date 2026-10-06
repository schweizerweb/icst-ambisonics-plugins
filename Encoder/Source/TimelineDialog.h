#pragma once
#include "JuceHeader.h"
#include "TimelineComponent.h"
#include "TimelineWidgetMS.h"
#include "../../Common/UiState.h"

// Action-String analog zu ACTION_CLOSE_ANIMATOR
static const juce::String ACTION_CLOSE_TIMELINE = "ACTION_CLOSE_TIMELINE";

// ---------------------------------- Dialog -----------------------------------
class TimelineDialog  : public juce::DocumentWindow,
                        public juce::ActionBroadcaster
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
    }

    ~TimelineDialog() override
    {
        UiState::rememberPosition(*this, UiState::Windows::animatorTimeline);
    }

    void updatePosition(juce::Rectangle<int> parentScreenBounds,
                        int w = 900, int h = 500)
    {
        // This window IS resizable, so unlike the fixed-size dialogs it restores the user's own
        // size as well as position.
        setSize(w, h);

        if (UiState::restorePosition(*this, UiState::Windows::animatorTimeline,
                                     /*centreIfUnknown*/ false, /*restoreSize*/ true))
            return;

        // Nothing remembered yet: fall back to sitting just right of the plugin window.
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