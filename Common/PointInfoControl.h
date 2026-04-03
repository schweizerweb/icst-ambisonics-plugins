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
#include "AmbiDataSet.h"
#include "PointSelection.h"
#include "DawParameter.h"
#include "ColorBorderDrawableButton.h"

class PointInfoControl  : public Component,
                          public ChangeListener,
                          public Label::Listener,
                          public juce::Button::Listener
{
public:
    PointInfoControl (AmbiDataSet* pEditablePoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~PointInfoControl() override;
    
    void updateSelectedPoint(String exceptField = "");
	void changeListenerCallback(ChangeBroadcaster* source) override;
    void labelTextChanged(Label *labelThatHasChanged) override;
    void disableListeners();
	void enableListeners();
	void setFieldsEnabled(bool enable) const;
    int getRequiredHeight(int width) const;
    void controlVisibility() const; 

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
	AmbiDataSet* pEditablePoints;
	PointSelection* pPointSelection;
	RadarOptions* pRadarOptions;
    enum controlSize { narrow = 30, wide = 60 };
    bool shiftWasDownWhenEditingStarted = false;

    void setupLabel(juce::Label* l, bool editable = true);
    
    struct CoorLabel : public juce::Label
        {
            juce::String prefix = "", postfix = "";

            void paint(juce::Graphics& g) override
            {
                if (isBeingEdited()) { juce::Label::paint(g); return; }

                if(getText().isNotEmpty())
                {
                    // Draw background via LookAndFeel, then custom text
                    g.fillAll(findColour(juce::Label::backgroundColourId));
                    
                    g.setColour(findColour(juce::Label::textColourId));
                    g.setFont(getFont());
                    g.drawFittedText(prefix + getText() + postfix,
                                     getLocalBounds().reduced(4, 0),
                                     getJustificationType(), 1);
                }
            }
            
            CoorLabel(String name, String prefix_, String postfix_): Label(name), prefix(prefix_), postfix(postfix_) {}
        };
    
    std::unique_ptr<ColorBorderDrawableButton> btnSphere;
    std::unique_ptr<DrawableButton> btnDelete;
    std::unique_ptr<DrawableButton> btnGroup;
    std::unique_ptr<DrawableButton> btnUngroup;
    std::unique_ptr<ColorBorderDrawableButton> btnPolarCartesian;

    std::unique_ptr<CoorLabel> textName;
    std::unique_ptr<CoorLabel> textX;
    std::unique_ptr<CoorLabel> textY;
    std::unique_ptr<CoorLabel> textZ;
    std::unique_ptr<CoorLabel> textA;
    std::unique_ptr<CoorLabel> textE;
    std::unique_ptr<CoorLabel> textD;
    std::unique_ptr<CoorLabel> textCH;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointInfoControl)
};
