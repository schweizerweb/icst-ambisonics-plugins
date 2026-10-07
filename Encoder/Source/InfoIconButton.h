#pragma once
#include "JuceHeader.h"

// A small circled "i" that shows a longer explanation on click, so a control can be labelled with
// just its name instead of carrying its description in brackets after it.
//
// Follows the existing info-button pattern in this app (OSCRxSettingsComponent, ImportExport): a
// CallOutBox pointing back at the button. The icon is drawn rather than loaded from BinaryData, so
// it needs no asset and scales to whatever size the layout gives it.
class InfoIconButton : public juce::Button   // juce::Button already is a SettableTooltipClient
{
public:
    explicit InfoIconButton(const juce::String& infoText)
        : juce::Button("Info"), text(infoText)
    {
        // Hovering is enough for most people; the click-through exists for touch and for text long
        // enough that a tooltip would be awkward.
        setTooltip(text);
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    void setInfoText(const juce::String& newText)
    {
        text = newText;
        setTooltip(text);
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);
        const float side = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto circle = bounds.withSizeKeepingCentre(side, side);

        const float alpha = shouldDrawButtonAsDown ? 0.9f : (shouldDrawButtonAsHighlighted ? 0.75f : 0.45f);

        g.setColour(juce::Colours::white.withAlpha(alpha));
        g.drawEllipse(circle, 1.2f);

        g.setFont(juce::Font(juce::FontOptions(side * 0.68f, juce::Font::bold)));
        g.drawText("i", circle, juce::Justification::centred, false);
    }

    void clicked() override
    {
        constexpr int width = 320;
        constexpr int padding = 12;

        const juce::Font font(juce::FontOptions(14.0f));

        // Measured rather than estimated, so a long explanation is never clipped and a short one
        // doesn't get a half-empty box.
        juce::AttributedString measured;
        measured.setText(text);
        measured.setFont(font);

        juce::TextLayout layout;
        layout.createLayout(measured, (float)(width - padding * 2));

        auto label = std::make_unique<juce::Label>();
        label->setText(text, juce::dontSendNotification);
        label->setJustificationType(juce::Justification::topLeft);
        label->setFont(font);
        label->setBorderSize({ padding, padding, padding, padding });

        // No explicit text colour: the CallOutBox paints itself in the LookAndFeel's (dark)
        // widgetBackground, so the Label has to keep the matching light defaultText. Forcing a
        // colour here is what made the first version near-invisible.
        label->setSize(width, (int)std::ceil(layout.getHeight()) + padding * 2);

        juce::CallOutBox::launchAsynchronously(std::move(label), getScreenBounds(), nullptr);
    }

private:
    juce::String text;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoIconButton)
};
