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
#include "../../Common/FilterBankInfo.h"
#include "SingleFilterSettingsComponent.h"

// Forward declare - compiler only needs pointer/reference
class IIRFilterGraph;

class FilterHandleComponent : public juce::Component
{
public:
    FilterHandleComponent (IIRFilterGraph& g, FilterBankInfo& b, int filterIdx);

    void setVisual (juce::Colour c, float a);
    void setFilterComponent(SingleFilterSettingsComponent* pFilterComponent);
    void paint (juce::Graphics& g) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

private:
    IIRFilterGraph& graph;
    FilterBankInfo& bank;
    SingleFilterSettingsComponent* pFilterComponent;
    int filterIndex;
    juce::Colour colour { juce::Colours::lightgreen };
    float alpha { 1.0f };
};
