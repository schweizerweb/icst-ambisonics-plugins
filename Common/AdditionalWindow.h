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

class AdditionalWindow : public DialogWindow
{
public:
    AdditionalWindow(String title, Component* pComponent): DialogWindow(title, Colours::white, false, true)
    {
        pDialogContent = pComponent;
    }
    
    void updatePosition (const Rectangle<int>& newAreaToPointTo)
    {
        // based on juce CallOutBox
        Rectangle<int> targetArea = newAreaToPointTo;
        const Displays::Display* d = Desktop::getInstance().getDisplays().getDisplayForRect(newAreaToPointTo);
        if(d == nullptr)
            return;
        
        Rectangle<int> newAreaToFitIn = d->userArea;
        
        float borderSpace = 1;
        float arrowSize = 0;
        
        Rectangle<int> newBounds (pDialogContent->getWidth()  + int(borderSpace * 2),
                                  pDialogContent->getHeight() + int(borderSpace * 2));
    
        float hw = (float)newBounds.getWidth() / 2.0f;
        float hh = (float)newBounds.getHeight() / 2.0f;
        float hwReduced = (float) (hw - borderSpace * 2);
        float hhReduced = (float) (hh - borderSpace * 2);
        float arrowIndent = borderSpace - arrowSize;
    
        Point<float> targets[4] = { { (float) targetArea.getCentreX(), (float) targetArea.getBottom() },
                                    { (float) targetArea.getRight(),   (float) targetArea.getCentreY() },
                                    { (float) targetArea.getX(),       (float) targetArea.getCentreY() },
                                    { (float) targetArea.getCentreX(), (float) targetArea.getY() } };
    
        Line<float> lines[4] = { { targets[0].translated (-hwReduced, hh - arrowIndent),    targets[0].translated (hwReduced, hh - arrowIndent) },
                                 { targets[1].translated (hw - arrowIndent, -hhReduced),    targets[1].translated (hw - arrowIndent, hhReduced) },
                                 { targets[2].translated (-(hw - arrowIndent), -hhReduced), targets[2].translated (-(hw - arrowIndent), hhReduced) },
                                 { targets[3].translated (-hwReduced, -(hh - arrowIndent)), targets[3].translated (hwReduced, -(hh - arrowIndent)) } };
    
        auto centrePointArea = newAreaToFitIn.reduced (int(hw), int(hh)).toFloat();
        auto targetCentre = targetArea.getCentre().toFloat();
    
        float nearest = 1.0e9f;
    
        for (int i = 0; i < 4; ++i)
        {
            Line<float> constrainedLine (centrePointArea.getConstrainedPoint (lines[i].getStart()),
                                         centrePointArea.getConstrainedPoint (lines[i].getEnd()));
    
            auto centre = constrainedLine.findNearestPointTo (targetCentre);
            auto distanceFromCentre = centre.getDistanceFrom (targets[i]);
    
            if (! centrePointArea.intersects (lines[i]))
                distanceFromCentre += 1000.0f;
    
            if (distanceFromCentre < nearest)
            {
                nearest = distanceFromCentre;
    
                newBounds.setPosition ((int) (centre.x - hw),
                                       (int) (centre.y - hh));
            }
        }
    
        setBounds (newBounds);
    }

private:
    Component* pDialogContent;
};
