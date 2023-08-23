/*
  ==============================================================================

    ColorSelectionComponent.h
    Created: 19 Jan 2020 6:34:30pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ColorSelectionComponent    : public Component
{
public:
    ColorSelectionComponent(Colour initialColor, ChangeListener* pColorChangeListener, Button::Listener* pButtonListener, bool groupFlag)
    {
        colorSelector.reset(new ColourSelector(ColourSelector::showColourspace));
        colorSelector->setName("Colour");
        colorSelector->setCurrentColour(initialColor);
        colorSelector->addChangeListener(pColorChangeListener);
        colorSelector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
        colorSelector->setSize(200, 200);

        addAndMakeVisible(colorSelector.get());

        if (groupFlag)
        {
            button.reset(new TextButton("Apply color to sub-points"));
            button->addListener(pButtonListener);
            button->setSize(200, 25);
            button->setTopLeftPosition(0, 202);
            addAndMakeVisible(button.get());
        }

        setSize(200, groupFlag ? 230 : 200);
    }

    virtual ~ColorSelectionComponent() override
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    }

    void resized() override
    {
    }

private:
    std::unique_ptr<ColourSelector> colorSelector;
    std::unique_ptr<TextButton> button;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorSelectionComponent)
};
