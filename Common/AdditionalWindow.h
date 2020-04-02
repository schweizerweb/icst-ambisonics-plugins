/*
  ==============================================================================

    AdditionalWindow.h
    Created: 2 Apr 2020 8:30:06pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class AdditionalWindow : public DialogWindow
{
public:
    AdditionalWindow(String title, Component* pComponent): DialogWindow("Encoder settings", Colours::white, false, true)
    {
        pDialogContent = pComponent;
    }
    
    void updatePosition (const Rectangle<int>& newAreaToPointTo)
    {
        // based on juce CallOutBox
        Rectangle<int> targetArea = newAreaToPointTo;
        Rectangle<int> newAreaToFitIn = Desktop::getInstance().getDisplays().findDisplayForRect(newAreaToPointTo).userArea;
        
        auto borderSpace = 1;
        auto arrowSize = 0;
        
        Rectangle<int> newBounds (pDialogContent->getWidth()  + borderSpace * 2,
                                  pDialogContent->getHeight() + borderSpace * 2);
    
        auto hw = newBounds.getWidth() / 2;
        auto hh = newBounds.getHeight() / 2;
        auto hwReduced = (float) (hw - borderSpace * 2);
        auto hhReduced = (float) (hh - borderSpace * 2);
        auto arrowIndent = borderSpace - arrowSize;
    
        Point<float> targets[4] = { { (float) targetArea.getCentreX(), (float) targetArea.getBottom() },
                                    { (float) targetArea.getRight(),   (float) targetArea.getCentreY() },
                                    { (float) targetArea.getX(),       (float) targetArea.getCentreY() },
                                    { (float) targetArea.getCentreX(), (float) targetArea.getY() } };
    
        Line<float> lines[4] = { { targets[0].translated (-hwReduced, hh - arrowIndent),    targets[0].translated (hwReduced, hh - arrowIndent) },
                                 { targets[1].translated (hw - arrowIndent, -hhReduced),    targets[1].translated (hw - arrowIndent, hhReduced) },
                                 { targets[2].translated (-(hw - arrowIndent), -hhReduced), targets[2].translated (-(hw - arrowIndent), hhReduced) },
                                 { targets[3].translated (-hwReduced, -(hh - arrowIndent)), targets[3].translated (hwReduced, -(hh - arrowIndent)) } };
    
        auto centrePointArea = newAreaToFitIn.reduced (hw, hh).toFloat();
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
