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

    void setToggleColors(juce::Colour offColor, juce::Colour onColor,
                        juce::Colour offBorder = juce::Colours::transparentBlack,
                        juce::Colour onBorder = juce::Colours::lightgreen)
    {
        customLookAndFeel.setToggleColors(offColor, onColor, offBorder, onBorder);
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
        void setToggleColors(juce::Colour offColor, juce::Colour onColor,
                            juce::Colour offBorder, juce::Colour onBorder)
        {
            offBackgroundColor = offColor;
            onBackgroundColor = onColor;
            offBorderColor = offBorder;
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
            auto bounds = button.getLocalBounds().toFloat();
            
            juce::Colour bgColor, borderColor;
            if (button.getToggleState())
            {
                bgColor = onBackgroundColor; // Toggled on
                borderColor = onBorderColor;
            }
            else
            {
                bgColor = offBackgroundColor; // Toggled off
                borderColor = offBorderColor;
            }
            
            // Apply interactive states
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
            
            // Draw background
            g.setColour(bgColor);
            g.fillRoundedRectangle(bounds, 4.0f);
            
            // Draw border (always draw border, but with different colors)
            if (borderThickness > 0)
            {
                g.setColour(borderColor);
                g.drawRoundedRectangle(bounds.reduced(borderThickness * 0.5f), 4.0f, borderThickness);
            }
        }

    private:
        juce::Colour offBackgroundColor = juce::Colour(0xff2d2d30);
        juce::Colour onBackgroundColor = juce::Colour(0xff4CAF50);
        juce::Colour offBorderColor = juce::Colours::grey.withAlpha(0.5f);  // Subtle border when off
        juce::Colour onBorderColor = juce::Colours::lightgreen;             // Bright border when on
        int borderThickness = 2;
    };

    void setupLookAndFeel()
    {
        customLookAndFeel.setToggleColors(
            juce::Colour(0xff2d2d30),                    // Off state background
            juce::Colour(0xff4CAF50),                    // On state background
            juce::Colours::grey.withAlpha(0.5f),         // Off state border
            juce::Colours::lightgreen                    // On state border
        );
        customLookAndFeel.setBorderThickness(2);
    }

    CustomLookAndFeel customLookAndFeel;
};
