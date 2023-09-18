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

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "AmbiDataSet.h"
#include "PointSelection.h"
#include "DawParameter.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PointInfoControl  : public Component,
                          public ChangeListener,
                          public TextEditor::Listener,
                          public juce::Button::Listener
{
public:
    //==============================================================================
    PointInfoControl (AmbiDataSet* pEditablePoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~PointInfoControl() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateSelectedPoint(String exceptField = "");
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void textEditorTextChanged(TextEditor& source) override;
	void disableListeners();
	void enableListeners();
	void setFieldsEnabled(bool enable) const;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* group_png;
    static const int group_pngSize;
    static const char* ungroup_png;
    static const int ungroup_pngSize;
    static const char* groupobjectsicon_png;
    static const int groupobjectsicon_pngSize;
    static const char* ungroupobjectsicon_png;
    static const int ungroupobjectsicon_pngSize;
    static const char* trashbinicon_png;
    static const int trashbinicon_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbiDataSet* pEditablePoints;
	PointSelection* pPointSelection;
	RadarOptions* pRadarOptions;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Component> helperSlider;
    std::unique_ptr<juce::Label> labelBackground;
    std::unique_ptr<juce::TextEditor> textName;
    std::unique_ptr<juce::Label> labelName;
    std::unique_ptr<juce::TextEditor> textX;
    std::unique_ptr<juce::TextEditor> textY;
    std::unique_ptr<juce::TextEditor> textZ;
    std::unique_ptr<juce::Label> labelCartesian;
    std::unique_ptr<juce::TextEditor> textA;
    std::unique_ptr<juce::TextEditor> textE;
    std::unique_ptr<juce::TextEditor> textD;
    std::unique_ptr<juce::Label> labelPolar;
    std::unique_ptr<juce::Label> labelCH;
    std::unique_ptr<juce::TextEditor> textCH;
    std::unique_ptr<juce::ImageButton> btnUngroup;
    std::unique_ptr<juce::ImageButton> btnGroup;
    std::unique_ptr<juce::ImageButton> btnDelete;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointInfoControl)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
