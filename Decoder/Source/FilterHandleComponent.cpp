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


#include "FilterHandleComponent.h"
#include "IIRFilterGraph.h"

FilterHandleComponent::FilterHandleComponent (IIRFilterGraph& g, FilterBankInfo& b, int filterIdx)
    : graph (g), bank (b), filterIndex (filterIdx)
{
    setInterceptsMouseClicks (true, true);
    setMouseCursor (juce::MouseCursor::DraggingHandCursor);
}

void FilterHandleComponent::setVisual (juce::Colour c, float a)
{
    colour = c; alpha = a; repaint();
}

void FilterHandleComponent::setFilterComponent(SingleFilterSettingsComponent *pComponent)
{
    pFilterComponent = pComponent;
}

void FilterHandleComponent::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (1.0f);
    g.setColour (colour.withAlpha (alpha));
    g.fillEllipse (r);
    g.setColour (juce::Colours::black.withAlpha (alpha));
    g.drawEllipse (r, 3.0f);
}

void FilterHandleComponent::mouseDrag (const juce::MouseEvent& e)
{
    graph.dragHandleTo (bank, filterIndex, e.getEventRelativeTo (&graph).position);
}

void FilterHandleComponent::mouseWheelMove(const MouseEvent& /*event*/, const MouseWheelDetails& wheel)
{
    graph.handleMouseWheel ( bank, filterIndex, wheel.deltaY);
}
