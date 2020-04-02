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
#include "SourceDefinitionComponent.h"
#include "EncoderPresetHelper.h"
#include "../../Common/PresetManagerComponent.h"
#include "../../Common/ZoomSettings.h"
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
                                   public ComboBox::Listener,
                                   public Button::Listener,
                                   public Slider::Listener
{
public:
    //==============================================================================
    EncodingSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams, EncoderPresetHelper* pPresetHelper, ZoomSettings* pZoomSettings);
    ~EncodingSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void controlDimming() const;
    void initializePresets();
    void updateEncodingUiElements();
    void actionListenerCallback(const String &message) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EncoderSettings* pEncoderSettings;
    AmbiSourceSet* pSources;
    Array<AudioParameterSet>* pAudioParams;
    std::unique_ptr<PresetManagerComponent> presetManagerComponent;
    EncoderPresetHelper* pPresetHelper;
    ZoomSettings* pZoomSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> comboBoxPresets;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<TextButton> buttonSave;
    std::unique_ptr<SourceDefinitionComponent> sourceDefinition;
    std::unique_ptr<ToggleButton> toggleDistanceEncoding;
    std::unique_ptr<ToggleButton> toggleDoppler;
    std::unique_ptr<Slider> sliderDistanceScaler;
    std::unique_ptr<Label> labelDistanceScaler;
    std::unique_ptr<TextButton> btnEditDistanceEncoding;
    std::unique_ptr<TextButton> buttonManagePresets;
    std::unique_ptr<ToggleButton> toggleInfiniteDistance;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncodingSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

