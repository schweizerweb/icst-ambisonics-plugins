/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



//[Headers] You can add your own extra header files here...
#include "EncoderPresetHelper.h"
//[/Headers]

#include "EncodingSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EncodingSettingsComponent::EncodingSettingsComponent (EncoderSettingsComponentArgs args)
    : m_args(args)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(m_args.pChangeListener);
    //[/Constructor_pre]

    groupDistanceEncoding.reset (new juce::GroupComponent ("groupDistanceEncoding",
                                                           TRANS("Distance Encoding")));
    addAndMakeVisible (groupDistanceEncoding.get());

    toggleDistanceEncoding.reset (new juce::ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable"));
    toggleDistanceEncoding->addListener (this);

    toggleDoppler.reset (new juce::ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable Doppler"));
    toggleDoppler->addListener (this);

    toggleDoppler->setBounds (6, 8, 199, 24);

    distanceEncodingComponent.reset (new DistanceEncodingComponent (&m_args.pSettings->distanceEncodingParams, m_args.pDistanceEncodingPresetHelper, m_args.pZoomSettings));
    addAndMakeVisible (distanceEncodingComponent.get());
    distanceEncodingComponent->setName ("distanceEncodingComponent");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    updateEncodingUiElements();
    //[/Constructor]
}

EncodingSettingsComponent::~EncodingSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupDistanceEncoding = nullptr;
    toggleDistanceEncoding = nullptr;
    toggleDoppler = nullptr;
    distanceEncodingComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EncodingSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EncodingSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupDistanceEncoding->setBounds (8, 40, getWidth() - 14, getHeight() - 46);
    toggleDistanceEncoding->setBounds (8 + 14, 40 + 24, 199, 24);
    distanceEncodingComponent->setBounds (8 + 14, 40 + 56, (getWidth() - 14) - 28, (getHeight() - 46) - 70);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EncodingSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleDistanceEncoding.get())
    {
        //[UserButtonCode_toggleDistanceEncoding] -- add your button handler code here..
        m_args.pSettings->distanceEncodingFlag = toggleDistanceEncoding->getToggleState();
        sendChangeMessage();
        controlDimming();
        //[/UserButtonCode_toggleDistanceEncoding]
    }
    else if (buttonThatWasClicked == toggleDoppler.get())
    {
        //[UserButtonCode_toggleDoppler] -- add your button handler code here..
        m_args.pSettings->dopplerEncodingFlag = toggleDoppler->getToggleState();
        sendChangeMessage();
        //[/UserButtonCode_toggleDoppler]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EncodingSettingsComponent::updateEncodingUiElements()
{
    toggleDistanceEncoding->setToggleState(m_args.pSettings->distanceEncodingFlag, dontSendNotification);

    toggleDoppler->setToggleState(m_args.pSettings->dopplerEncodingFlag, dontSendNotification);
    
    controlDimming();
}

void EncodingSettingsComponent::controlDimming()
{
    distanceEncodingComponent->setEnabled(toggleDistanceEncoding->getToggleState());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EncodingSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster"
                 constructorParams="EncoderSettingsComponentArgs args" variableInitialisers="m_args(args)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupDistanceEncoding" id="c7f86e9b07311c49" memberName="groupDistanceEncoding"
                  virtualName="" explicitFocusOrder="0" pos="8 40 14M 46M" title="Distance Encoding"/>
  <TOGGLEBUTTON name="toggleDistanceEncoding" id="c46d0c7f045490ec" memberName="toggleDistanceEncoding"
                virtualName="" explicitFocusOrder="0" pos="14 24 199 24" posRelativeX="c7f86e9b07311c49"
                posRelativeY="c7f86e9b07311c49" buttonText="Enable" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleDoppler" id="8a9ea68cd17e7a8c" memberName="toggleDoppler"
                virtualName="" explicitFocusOrder="0" pos="6 8 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Doppler" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="distanceEncodingComponent" id="1a603f1700c46471" memberName="distanceEncodingComponent"
                    virtualName="" explicitFocusOrder="0" pos="14 56 28M 70M" posRelativeX="c7f86e9b07311c49"
                    posRelativeY="c7f86e9b07311c49" posRelativeW="c7f86e9b07311c49"
                    posRelativeH="c7f86e9b07311c49" class="DistanceEncodingComponent"
                    params="&amp;m_args.pSettings-&gt;distanceEncodingParams, m_args.pDistanceEncodingPresetHelper, m_args.pZoomSettings"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

