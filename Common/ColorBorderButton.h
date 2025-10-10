class ColorBorderButton : public juce::ImageButton
{
public:
    ColorBorderButton(bool& toggleStateRef) : isToggled(toggleStateRef)
    {
        setClickingTogglesState(false);
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        ImageButton::paintButton(g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        
        if (isToggled)
        {
            g.setColour(borderColor);
            g.drawRect(getLocalBounds(), borderThickness);
        }
    }
    
    void setBorderProperties(juce::Colour color, int thickness)
    {
        borderColor = color;
        borderThickness = thickness;
    }
    
private:
    
    bool& isToggled;
    juce::Colour borderColor = juce::Colours::lightgreen;
    int borderThickness = 4;
};
