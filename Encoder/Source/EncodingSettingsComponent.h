/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceDefinitionComponent.h"
#include "EncoderPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"
#include "../../Common/ZoomSettings.h"
#include "DistanceEncodingPresetHelper.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncodingSettingsComponent  : public Component,
                                   public ChangeBroadcaster,
                                   public ActionListener,
                                   public ChangeListener,
                                   public juce::ComboBox::Listener,
                                   public juce::Button::Listener,
                                   public juce::Slider::Listener
{
public:
    //==============================================================================
    EncodingSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, EncoderPresetHelper* pPresetHelper, ZoomSettings* pZoomSettings, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper);
    ~EncodingSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void controlDimming() const;
    void initializePresets();
    void updateEncodingUiElements();
    void actionListenerCallback(const String &message) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EncoderSettings* pEncoderSettings;
    AmbiSourceSet* pSources;
    AudioParams* pAudioParams;
    PresetManagerDialog presetManagerDialog;
    EncoderPresetHelper* pPresetHelper;
    ZoomSettings* pZoomSettings;
    DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> comboBoxPresets;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    std::unique_ptr<SourceDefinitionComponent> sourceDefinition;
    std::unique_ptr<juce::ToggleButton> toggleDistanceEncoding;
    std::unique_ptr<juce::ToggleButton> toggleDoppler;
    std::unique_ptr<juce::Slider> sliderDistanceScaler;
    std::unique_ptr<juce::Label> labelDistanceScaler;
    std::unique_ptr<juce::TextButton> btnEditDistanceEncoding;
    std::unique_ptr<juce::TextButton> buttonManagePresets;
    std::unique_ptr<juce::ToggleButton> toggleInfiniteDistance;
    std::unique_ptr<juce::TextButton> btnManageDistanceEncodingPresets;
    std::unique_ptr<juce::Label> labelMasterGain;
    std::unique_ptr<juce::Slider> sliderMasterGain;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncodingSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

