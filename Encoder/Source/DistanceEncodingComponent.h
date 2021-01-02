/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

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
                                   public juce::Slider::Listener,
                                   public juce::ComboBox::Listener,
                                   public juce::Button::Listener
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

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DistanceEncodingParams* pParams;
    DistanceEncodingPresetHelper* pPresetHelper;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupAirAbsorbtion;
    std::unique_ptr<juce::GroupComponent> groupAttenuation;
    std::unique_ptr<DistanceEncodingGraph> distanceEncodingGraph;
    std::unique_ptr<juce::Slider> sliderUnitCircleRadius;
    std::unique_ptr<juce::Label> labelUnitCircleRadius;
    std::unique_ptr<juce::Label> labelEncodingMode;
    std::unique_ptr<juce::ComboBox> comboBoxEncodingMode;
    std::unique_ptr<juce::Slider> sliderDbUnit;
    std::unique_ptr<juce::Label> labelDbUnit;
    std::unique_ptr<juce::Slider> sliderDistanceAttenuation;
    std::unique_ptr<juce::Label> labelDistanceAttenuation;
    std::unique_ptr<juce::Slider> sliderCenterCurve;
    std::unique_ptr<juce::Label> labelCenterCurve;
    std::unique_ptr<juce::Slider> sliderAdvancedFactor;
    std::unique_ptr<juce::Label> labelAdvancedFact;
    std::unique_ptr<juce::Slider> sliderAdvancedExponent;
    std::unique_ptr<juce::Label> labelAdvancedExponent;
    std::unique_ptr<juce::ComboBox> comboBoxDistanceEncodingPreset;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    std::unique_ptr<juce::Label> labelAirAbsorbtionMode;
    std::unique_ptr<juce::ComboBox> comboBoxAirAbsorbtionMode;
    std::unique_ptr<juce::Slider> sliderAirAbsorbtionIntensity;
    std::unique_ptr<juce::Label> labelIntensity;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceEncodingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

