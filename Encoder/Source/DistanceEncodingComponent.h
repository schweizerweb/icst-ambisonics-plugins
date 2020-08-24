/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "DistanceEncodingParams.h"
#include "DistanceEncodingGraph.h"
#include "DistanceEncodingPresetHelper.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DistanceEncodingComponent  : public Component,
                                   ChangeListener,
                                   ActionListener,
                                   public Slider::Listener,
                                   public ComboBox::Listener,
                                   public Button::Listener
{
public:
    //==============================================================================
    DistanceEncodingComponent (DistanceEncodingParams* pParams, DistanceEncodingPresetHelper* pPresetHelper);
    ~DistanceEncodingComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updatePresetComboBox();
    void controlDimming() const;
    void setUiValues(DistanceEncodingParams *pEncodingParams);
    void changeListenerCallback(ChangeBroadcaster *source) override;
    void actionListenerCallback(const String &message) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DistanceEncodingParams* pParams;
    DistanceEncodingPresetHelper* pPresetHelper;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<DistanceEncodingGraph> distanceEncodingGraph;
    std::unique_ptr<Slider> sliderUnitCircleRadius;
    std::unique_ptr<Label> labelUnitCircleRadius;
    std::unique_ptr<Label> labelEncodingMode;
    std::unique_ptr<ComboBox> comboBoxEncodingMode;
    std::unique_ptr<Slider> sliderDbUnit;
    std::unique_ptr<Label> labelDbUnit;
    std::unique_ptr<Slider> sliderDistanceAttenuation;
    std::unique_ptr<Label> labelDistanceAttenuation;
    std::unique_ptr<Slider> sliderCenterCurve;
    std::unique_ptr<Label> labelCenterCurve;
    std::unique_ptr<Slider> sliderAdvancedFactor;
    std::unique_ptr<Label> labelAdvancedFact;
    std::unique_ptr<Slider> sliderAdvancedExponent;
    std::unique_ptr<Label> labelAdvancedExponent;
    std::unique_ptr<ComboBox> comboBoxDistanceEncodingPreset;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<TextButton> buttonSave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceEncodingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

