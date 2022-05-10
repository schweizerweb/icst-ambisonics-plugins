/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...

//[/Headers]

#include "EncoderSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
EncoderSettingsComponent::EncoderSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, ZoomSettings* pZoomSettings, StatusMessageHandler* pStatusMessageHandler, EncoderPresetHelper* pPresetHelper, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper, OSCLogDialogManager* pOscLogManager)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    tabbedComponent.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabbedComponent.get());
    tabbedComponent->setTabBarDepth (35);
    tabbedComponent->addTab (TRANS("Encoding"), juce::Colours::lightgrey, new EncodingSettingsComponent (pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams, pPresetHelper, pZoomSettings, pDistanceEncodingPresetHelper), true);
    tabbedComponent->addTab (TRANS("Radar"), juce::Colours::lightgrey, new RadarSettingsComponent (pChangeListener, pZoomSettings), true);
    tabbedComponent->addTab (TRANS("OSC In"), juce::Colours::lightgrey, new OSCRxSettingsComponent (pChangeListener, pSettings, pStatusMessageHandler, pOscLogManager), true);
    tabbedComponent->addTab (TRANS("OSC Out"), juce::Colours::lightgrey, new OSCSettingsComponent (pChangeListener, pSettings), true);
    tabbedComponent->setCurrentTabIndex (0);

    labelDevelopmentVersion.reset (new juce::Label ("labelDevelopmentVersion",
                                                    TRANS("Unofficial Pre-Release")));
    addAndMakeVisible (labelDevelopmentVersion.get());
    labelDevelopmentVersion->setFont (juce::Font (25.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDevelopmentVersion->setJustificationType (juce::Justification::centred);
    labelDevelopmentVersion->setEditable (false, false, false);
    labelDevelopmentVersion->setColour (juce::Label::backgroundColourId, juce::Colour (0xbded0d0d));
    labelDevelopmentVersion->setColour (juce::Label::textColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::Label::outlineColourId, juce::Colours::yellow);
    labelDevelopmentVersion->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDevelopmentVersion->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
	labelDevelopmentVersion->setVisible(Constants::isDevelopmentVersion());
    //[/Constructor]
}

EncoderSettingsComponent::~EncoderSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;
    labelDevelopmentVersion = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EncoderSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EncoderSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabbedComponent->setBounds (8, 8, getWidth() - 15, getHeight() - 13);
    labelDevelopmentVersion->setBounds (getWidth() - 10 - 245, 8, 245, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EncoderSettingsComponent"
                 componentName="" parentClasses="public Component, public ActionBroadcaster"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, ZoomSettings* pZoomSettings, StatusMessageHandler* pStatusMessageHandler, EncoderPresetHelper* pPresetHelper, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper, OSCLogDialogManager* pOscLogManager"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ff505050"/>
  <TABBEDCOMPONENT name="tabbedComponent" id="3a0bd97c3580beb3" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="8 8 15M 13M" orientation="top"
                   tabBarDepth="35" initialTab="0">
    <TAB name="Encoding" colour="ffd3d3d3" useJucerComp="0" contentClassName="EncodingSettingsComponent"
         constructorParams="pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams, pPresetHelper, pZoomSettings, pDistanceEncodingPresetHelper"
         jucerComponentFile=""/>
    <TAB name="Radar" colour="ffd3d3d3" useJucerComp="0" contentClassName="RadarSettingsComponent"
         constructorParams="pChangeListener, pZoomSettings" jucerComponentFile=""/>
    <TAB name="OSC" colour="ffd3d3d3" useJucerComp="0" contentClassName="OSCSettingsComponent"
         constructorParams="pChangeListener, pSettings, pStatusMessageHandler, pOscLogManager"
         jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <LABEL name="labelDevelopmentVersion" id="c41821090201078b" memberName="labelDevelopmentVersion"
         virtualName="" explicitFocusOrder="0" pos="10Rr 8 245 24" bkgCol="bded0d0d"
         textCol="ffffff00" outlineCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="Unofficial Pre-Release" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="25.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

