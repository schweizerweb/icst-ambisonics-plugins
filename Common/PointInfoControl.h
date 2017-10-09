/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../Decoder/JuceLibraryCode/JuceHeader.h"
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
                          public TextEditorListener,
                          public ButtonListener
{
public:
    //==============================================================================
    PointInfoControl (Array<AmbiPoint>* pSpeakerArray, PointSelection* pPointSelection);
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
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Array<AmbiPoint>* pSpeakerArray;
	PointSelection* pPointSelection;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textName;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label3;
    ScopedPointer<TextEditor> textX;
    ScopedPointer<Label> label4;
    ScopedPointer<TextEditor> textY;
    ScopedPointer<Label> label5;
    ScopedPointer<TextEditor> textZ;
    ScopedPointer<Label> label6;
    ScopedPointer<Label> label7;
    ScopedPointer<TextEditor> textA;
    ScopedPointer<Label> label8;
    ScopedPointer<TextEditor> textE;
    ScopedPointer<Label> label9;
    ScopedPointer<TextEditor> textD;
    ScopedPointer<Label> label10;
    ScopedPointer<TextButton> buttonAdd;
    ScopedPointer<TextButton> buttonRemove;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointInfoControl)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
