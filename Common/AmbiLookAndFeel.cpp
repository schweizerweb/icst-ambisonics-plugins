#include "AmbiLookAndFeel.h"

AmbiLookAndFeel::AmbiLookAndFeel()
{
    //setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

void AmbiLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    Colour c = slider.findColour(juce::Slider::ColourIds::rotarySliderFillColourId);
    g.setColour(c);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::darkgrey);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    Colour invColor = c.contrasting(); // c.contrasting(Colours::yellow, 0.3f);
    
    g.setColour(invColor);
    g.fillPath(p);

    auto rxt = centreX - radius * 0.6f;
    auto ryt = centreY - radius * 0.6f;
    auto rwt = radius * 1.2f;
    auto value = slider.getValue();
    auto valueString = value > 0 ? ("+" + String(value, 1)) : String(value, 1);
    g.drawFittedText(valueString, rxt, ryt, rwt, rwt/2, Justification::centred, 1);
    g.drawFittedText(slider.getTextValueSuffix(), rxt, ryt + rwt / 2, rwt, rwt / 2, Justification::centred, 1);
}

void AmbiLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/, Slider::SliderStyle, Slider&)
{
    // fill
    g.setColour(juce::Colours::grey);
    g.fillRect(x, y, width, height);

    // outline
    g.setColour(findColour(juce::Slider::ColourIds::thumbColourId));
    g.fillRect(Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));

    //Graphics::ScopedSaveState state(g);
    //g.addTransform(AffineTransform::rotation(-MathConstants<float>::halfPi,
    //    width * 0.5f,
    //    height * 0.5f));
    //g.setColour(juce::Colours::white);
    //g.drawText(String(sliderPos, 2, false), x, height-20, width, height, Justification::bottom);
}
