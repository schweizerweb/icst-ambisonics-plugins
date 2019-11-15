/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
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
                          public Button::Listener
{
public:
    //==============================================================================
    PointInfoControl (AmbiDataSet* pEditablePoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~PointInfoControl();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateSelectedPoint(String exceptField = "");
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void textEditorTextChanged(TextEditor& source) override;
	void disableListeners();
	void enableListeners();
	void setFieldsEnabled(bool enable) const;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* group_png;
    static const int group_pngSize;
    static const char* ungroup_png;
    static const int ungroup_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AmbiDataSet* pEditablePoints;
	PointSelection* pPointSelection;
	RadarOptions* pRadarOptions;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> textName;
    std::unique_ptr<Label> labelName;
    std::unique_ptr<Label> labelX;
    std::unique_ptr<TextEditor> textX;
    std::unique_ptr<Label> labelY;
    std::unique_ptr<TextEditor> textY;
    std::unique_ptr<Label> labelZ;
    std::unique_ptr<TextEditor> textZ;
    std::unique_ptr<Label> labelCartesian;
    std::unique_ptr<Label> labelA;
    std::unique_ptr<TextEditor> textA;
    std::unique_ptr<Label> labelE;
    std::unique_ptr<TextEditor> textE;
    std::unique_ptr<Label> labelD;
    std::unique_ptr<TextEditor> textD;
    std::unique_ptr<Label> labelPolar;
    std::unique_ptr<Label> labelCH;
    std::unique_ptr<TextEditor> textCH;
    std::unique_ptr<ImageButton> btnGroup;
    std::unique_ptr<ImageButton> btnUngroup;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointInfoControl)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

