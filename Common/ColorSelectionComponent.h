/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
