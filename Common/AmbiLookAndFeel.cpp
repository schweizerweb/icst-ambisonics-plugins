#include "AmbiLookAndFeel.h"

AmbiLookAndFeel::AmbiLookAndFeel()
{
    //setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

void AmbiLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(juce::Colours::orange);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::red);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::yellow);
    g.fillPath(p);
}

void AmbiLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle, Slider&)
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
