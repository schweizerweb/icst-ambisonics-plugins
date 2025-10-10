#pragma once

#include <JuceHeader.h>

class ColorDrawableToggleButton : public juce::DrawableButton
{
public:
    ColorDrawableToggleButton(const juce::String& buttonName)
        : juce::DrawableButton(buttonName, juce::DrawableButton::ImageOnButtonBackground)
    {
        setClickingTogglesState(true);
        
        // Set up custom look and feel
        setupLookAndFeel();
        setLookAndFeel(&customLookAndFeel);
    }

    ~ColorDrawableToggleButton() override
    {
        setLookAndFeel(nullptr);
    }

    void setToggleColors(juce::Colour offColor, juce::Colour onColor)
    {
        customLookAndFeel.setToggleColors(offColor, onColor);
        repaint();
    }

    void setBorderThickness(int thickness)
    {
        customLookAndFeel.setBorderThickness(thickness);
        repaint();
    }

private:
    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        void setToggleColors(juce::Colour onColor, juce::Colour onBorder)
        {
            onBackgroundColor = onColor;
            onBorderColor = onBorder;
        }

        void setBorderThickness(int thickness)
        {
            borderThickness = thickness;
        }

        void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                 const juce::Colour& backgroundColour,
                                 bool shouldDrawButtonAsHighlighted,
                                 bool shouldDrawButtonAsDown) override
        {
            // Only apply custom styling when toggled ON
            if (button.getToggleState())
            {
                auto bounds = button.getLocalBounds().toFloat();
                
                juce::Colour bgColor = onBackgroundColor;
                juce::Colour borderColor = onBorderColor;
                
                if (shouldDrawButtonAsDown)
                {
                    bgColor = bgColor.darker(0.3f);
                    borderColor = borderColor.darker(0.3f);
                }
                else if (shouldDrawButtonAsHighlighted)
                {
                    bgColor = bgColor.brighter(0.2f);
                    borderColor = borderColor.brighter(0.2f);
                }
                
                g.setColour(bgColor);
                g.fillRoundedRectangle(bounds, 4.0f);
                
                if (borderThickness > 0)
                {
                    g.setColour(borderColor);
                    g.drawRoundedRectangle(bounds.reduced(borderThickness * 0.5f), 4.0f, borderThickness);
                }
            }
            else
            {
                // When toggled OFF, just use the default JUCE styling
                // This will automatically match your other buttons!
                LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour,
                                                   shouldDrawButtonAsHighlighted,
                                                   shouldDrawButtonAsDown);
            }
        }

    private:
        juce::Colour onBackgroundColor = juce::Colour(0xff4CAF50);
        juce::Colour onBorderColor = juce::Colours::lightgreen;
        int borderThickness = 2;
    };

    void setupLookAndFeel()
    {
        // Only need to set the ON state colors
        customLookAndFeel.setToggleColors(juce::Colour(0xff208020), juce::Colour(0xff4CAF50));
        customLookAndFeel.setBorderThickness(2);
    }

    CustomLookAndFeel customLookAndFeel;
};
