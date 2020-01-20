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
#include "../../Common/ZoomSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RadarSettingsComponent  : public Component,
                                public ChangeBroadcaster,
                                public ChangeListener,
                                public Slider::Listener,
                                public Button::Listener
{
public:
    //==============================================================================
    RadarSettingsComponent (ChangeListener* pChangeListener, ZoomSettings* pZoomSettings);
    ~RadarSettingsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void displaySettings();
    void changeListenerCallback(ChangeBroadcaster *source) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ZoomSettings* pZoomSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupZoom;
    std::unique_ptr<Slider> sliderCenterPointX;
    std::unique_ptr<Label> labelCenterPointX;
    std::unique_ptr<Slider> sliderCenterPointY;
    std::unique_ptr<Label> labelCenterPointY;
    std::unique_ptr<Slider> sliderCenterPointZ;
    std::unique_ptr<Label> labelCenterPointZ;
    std::unique_ptr<GroupComponent> groupDisplay;
    std::unique_ptr<Slider> sliderPointScaler;
    std::unique_ptr<Label> labelPointScaler;
    std::unique_ptr<Slider> sliderRadius;
    std::unique_ptr<Label> labelRadius;
    std::unique_ptr<TextButton> btnReset;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadarSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

