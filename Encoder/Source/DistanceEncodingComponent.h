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
#include "../JuceLibraryCode/JuceHeader.h"
#include "DistanceEncodingParams.h"
#include "DistanceEncodingGraph.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DistanceEncodingComponent  : public Component,
                                   public Slider::Listener,
                                   public ComboBox::Listener
{
public:
    //==============================================================================
    DistanceEncodingComponent (DistanceEncodingParams* pParams);
    ~DistanceEncodingComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void controlDimming() const;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DistanceEncodingParams* pParams;
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
    std::unique_ptr<Slider> sliderExperimentalFactor;
    std::unique_ptr<Label> labelExperimentalFact;
    std::unique_ptr<Slider> sliderExperimentalPower;
    std::unique_ptr<Label> labelExperimentalPower;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceEncodingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

