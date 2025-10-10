#pragma once

#include <JuceHeader.h>

class ColorBorderDrawableButton : public juce::DrawableButton
{
public:
    ColorBorderDrawableButton(const juce::String& buttonName, bool& toggleStateRef)
        : juce::DrawableButton(buttonName, juce::DrawableButton::ImageOnButtonBackground),
          isToggled(toggleStateRef)
    {
        setClickingTogglesState(false);
        
        // Set up custom look and feel
        setupLookAndFeel();
        setLookAndFeel(&customLookAndFeel);
    }

    ~ColorBorderDrawableButton() override
    {
        setLookAndFeel(nullptr);
    }

    void setToggleColors(juce::Colour normalColor, juce::Colour toggledColor)
    {
        customLookAndFeel.setToggleColors(normalColor, toggledColor);
        repaint();
    }

    void setBorderProperties(juce::Colour color, int thickness)
    {
        customLookAndFeel.setBorderProperties(color, thickness);
        repaint();
    }

private:
    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        void setToggleColors(juce::Colour normalColor, juce::Colour toggledColor)
        {
            normalBackgroundColor = normalColor;
            toggledBackgroundColor = toggledColor;
        }

        void setBorderProperties(juce::Colour color, int thickness)
        {
            borderColor = color;
            borderThickness = thickness;
        }

        void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                 const juce::Colour& backgroundColour,
                                 bool shouldDrawButtonAsHighlighted,
                                 bool shouldDrawButtonAsDown) override
        {
            auto& customButton = static_cast<ColorBorderDrawableButton&>(button);
            
            if (customButton.isToggled)
            {
                // Apply custom styling when toggled ON
                auto bounds = button.getLocalBounds().toFloat();
                
                juce::Colour bgColor = toggledBackgroundColor;
                juce::Colour borderCol = borderColor;
                
                if (shouldDrawButtonAsDown)
                {
                    bgColor = bgColor.darker(0.3f);
                    borderCol = borderCol.darker(0.3f);
                }
                else if (shouldDrawButtonAsHighlighted)
                {
                    bgColor = bgColor.brighter(0.2f);
                    borderCol = borderCol.brighter(0.2f);
                }
                
                g.setColour(bgColor);
                g.fillRoundedRectangle(bounds, 4.0f);
                
                if (borderThickness > 0)
                {
                    g.setColour(borderCol);
                    g.drawRoundedRectangle(bounds.reduced(borderThickness * 0.5f), 4.0f, borderThickness);
                }
            }
            else
            {
                // When toggled OFF, just use the default JUCE styling
                LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour,
                                                   shouldDrawButtonAsHighlighted,
                                                   shouldDrawButtonAsDown);
            }
        }

    private:
        juce::Colour normalBackgroundColor = juce::Colour(0xff2d2d30);
        juce::Colour toggledBackgroundColor = juce::Colour(0xff4CAF50);
        juce::Colour borderColor = juce::Colours::lightgreen;
        int borderThickness = 2;
    };

    void setupLookAndFeel()
    {
        customLookAndFeel.setToggleColors(juce::Colour(0xff2d2d30), juce::Colour(0xff208020));
        customLookAndFeel.setBorderProperties(juce::Colour(0xff4CAF50), 2);
    }

    bool& isToggled;
    CustomLookAndFeel customLookAndFeel;
};
