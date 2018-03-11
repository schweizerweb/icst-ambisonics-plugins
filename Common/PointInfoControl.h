/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "AmbiPoint.h"
#include "PointSelection.h"
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
                          public TextEditor::Listener
{
public:
    //==============================================================================
    PointInfoControl (OwnedArray<AmbiPoint>* pEditablePointsArray, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~PointInfoControl();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateSelectedPoint(String exceptField = "");
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void textEditorTextChanged(TextEditor& source) override;
	void disableListeners();
	void enableListeners();
	void setFieldsEnabled(bool enable) const;
	double RadToGrad(double rad) const;
	double GradToRad(float grad) const;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	OwnedArray<AmbiPoint>* pEditablePointsArray;
	PointSelection* pPointSelection;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textName;
    ScopedPointer<Label> labelName;
    ScopedPointer<Label> labelX;
    ScopedPointer<TextEditor> textX;
    ScopedPointer<Label> labelY;
    ScopedPointer<TextEditor> textY;
    ScopedPointer<Label> labelZ;
    ScopedPointer<TextEditor> textZ;
    ScopedPointer<Label> labelCartesian;
    ScopedPointer<Label> labelA;
    ScopedPointer<TextEditor> textA;
    ScopedPointer<Label> labelE;
    ScopedPointer<TextEditor> textE;
    ScopedPointer<Label> labelD;
    ScopedPointer<TextEditor> textD;
    ScopedPointer<Label> labelPolar;
    ScopedPointer<Label> labelCH;
    ScopedPointer<TextEditor> textCH;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointInfoControl)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
