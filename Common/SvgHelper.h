#pragma once
#include "JuceHeader.h"

class SvgHelper
{
public:
    static void loadSVGIcon(juce::DrawableButton* button, const char* svgData, size_t svgDataSize, const juce::String& tooltip)
    {
        if (svgData != nullptr && svgDataSize > 0)
        {
            auto svg = juce::Drawable::createFromImageData(svgData, svgDataSize);
            if (svg != nullptr)
            {
                svg->replaceColour(juce::Colours::black, juce::Colours::lightgrey);
                // Add some padding around the SVG
                svg->setTransformToFit(juce::Rectangle<float>(4, 4, 16, 16), juce::RectanglePlacement::centred);
                button->setImages(svg.get());
            }
        }
        button->setTooltip(tooltip);
    }
};
