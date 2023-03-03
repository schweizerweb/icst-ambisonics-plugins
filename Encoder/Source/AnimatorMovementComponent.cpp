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

//[Headers] You can add your own extra header files here...
#include "../../Common/AmbiSourceSet.h"
//[/Headers]

#include "AnimatorMovementComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AnimatorMovementComponent::AnimatorMovementComponent (AmbiSourceSet* pSourceSet, AnimatorMovement* pAnimatorMovement, ActionListener* pActionListener)
    : pSourceSet(pSourceSet), pAnimatorMovement(pAnimatorMovement)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addActionListener(pActionListener);
    //[/Constructor_pre]

    group1.reset (new juce::GroupComponent ("new group",
                                            TRANS("Preset 1")));
    addAndMakeVisible (group1.get());

    buttonSet.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonSet.get());
    buttonSet->setButtonText (TRANS("Set"));
    buttonSet->addListener (this);

    sliderTime.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderTime.get());
    sliderTime->setRange (0, 999999, 0.1);
    sliderTime->setSliderStyle (juce::Slider::IncDecButtons);
    sliderTime->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 150, 20);
    sliderTime->addListener (this);

    togglePolar.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar.get());
    togglePolar->setButtonText (TRANS("Polar"));
    togglePolar->addListener (this);

    buttonGo.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo.get());
    buttonGo->setButtonText (TRANS("Go"));
    buttonGo->addListener (this);

    labelG.reset (new juce::Label ("new label",
                                   TRANS("G:")));
    addAndMakeVisible (labelG.get());
    labelG->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelG->setJustificationType (juce::Justification::centredLeft);
    labelG->setEditable (false, false, false);
    labelG->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelG->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelG->setBounds (16, 16, 32, 24);

    sliderGroup.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderGroup.get());
    sliderGroup->setRange (0, 10, 1);
    sliderGroup->setSliderStyle (juce::Slider::IncDecButtons);
    sliderGroup->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderGroup->addListener (this);

    sliderGroup->setBounds (48, 16, 136, 24);


    //[UserPreSize]
    group1->setText(pAnimatorMovement->name);
    controlDimming();
    //[/UserPreSize]

    setSize (600, 100);


    //[Constructor] You can add your own custom stuff here..
    String sliderSuffix = " s (Smoothing)";
    sliderTime->setTextValueSuffix(sliderSuffix);

    sliderGroup->setRange(1, 2, 1);
    if(pSourceSet->groupCount() > 1)
    {
        sliderGroup->setRange(1, pSourceSet->groupCount(), 1);
    }
    else
    {
        sliderGroup->setEnabled(false);
    }

    refreshControls();
    //[/Constructor]
}

AnimatorMovementComponent::~AnimatorMovementComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    removeAllActionListeners();
    //[/Destructor_pre]

    group1 = nullptr;
    buttonSet = nullptr;
    sliderTime = nullptr;
    togglePolar = nullptr;
    buttonGo = nullptr;
    labelG = nullptr;
    sliderGroup = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AnimatorMovementComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AnimatorMovementComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    group1->setBounds (0, 0, getWidth() - 0, 48);
    buttonSet->setBounds (0 + 192, 0 + 16, 55, 24);
    sliderTime->setBounds (0 + 256, 0 + 16, (getWidth() - 0) - 398, 24);
    togglePolar->setBounds (0 + (getWidth() - 0) - 71 - 63, 0 + 16, 63, 24);
    buttonGo->setBounds (0 + (getWidth() - 0) - 9 - 55, 0 + 16, 55, 24);
    //[UserResized] Add your own custom resize handling here..
    sliderTime->setTextBoxStyle(Slider::TextBoxLeft, false, jmax(0, sliderTime->getWidth() - 50), sliderTime->getHeight());

    //[/UserResized]
}

void AnimatorMovementComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSet.get())
    {
        //[UserButtonCode_buttonSet] -- add your button handler code here..
        int groupIndex = (int)sliderGroup->getValue()-1;
        pAnimatorMovement->groupPositions.clear();
        Point3D<double> p(*pSourceSet->getGroup(groupIndex)->getRawPoint());
        pAnimatorMovement->groupPositions[groupIndex] = p;
        //[/UserButtonCode_buttonSet]
    }
    else if (buttonThatWasClicked == togglePolar.get())
    {
        //[UserButtonCode_togglePolar] -- add your button handler code here..
        pAnimatorMovement->polarFlag = togglePolar->getToggleState();
        //[/UserButtonCode_togglePolar]
    }
    else if (buttonThatWasClicked == buttonGo.get())
    {
        //[UserButtonCode_buttonGo] -- add your button handler code here..
        sendActionMessage(pAnimatorMovement->name);
        //[/UserButtonCode_buttonGo]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
    //[/UserbuttonClicked_Post]
}

void AnimatorMovementComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderTime.get())
    {
        //[UserSliderCode_sliderTime] -- add your slider handling code here..
        pAnimatorMovement->timeSpanSeconds = sliderTime->getValue();
        //[/UserSliderCode_sliderTime]
    }
    else if (sliderThatWasMoved == sliderGroup.get())
    {
        //[UserSliderCode_sliderGroup] -- add your slider handling code here..
        pAnimatorMovement->groupPositions.clear();
        controlDimming();
        //[/UserSliderCode_sliderGroup]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AnimatorMovementComponent::refreshControls()
{
    for(auto p : pAnimatorMovement->groupPositions)
    {
        // temp solution because only one group allowed
        sliderGroup->setValue(p.first + 1, dontSendNotification);
        break;
    }

    sliderTime->setValue(pAnimatorMovement->timeSpanSeconds);
    togglePolar->setToggleState(pAnimatorMovement->polarFlag, dontSendNotification);
}
void AnimatorMovementComponent::controlDimming()
{
    bool en = pAnimatorMovement->groupPositions.size() > 0;
    buttonGo->setEnabled(en);
    buttonSet->setColour(TextButton::buttonColourId, en ? Colours::green : Colours::orangered);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AnimatorMovementComponent"
                 componentName="" parentClasses="public juce::Component, ActionBroadcaster"
                 constructorParams="AmbiSourceSet* pSourceSet, AnimatorMovement* pAnimatorMovement, ActionListener* pActionListener"
                 variableInitialisers="pSourceSet(pSourceSet), pAnimatorMovement(pAnimatorMovement)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="100">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="238ed80972cc4d19" memberName="group1" virtualName=""
                  explicitFocusOrder="0" pos="0 0 0M 48" title="Preset 1"/>
  <TEXTBUTTON name="new button" id="6f8d2fc9df3bb657" memberName="buttonSet"
              virtualName="" explicitFocusOrder="0" pos="192 16 55 24" posRelativeX="238ed80972cc4d19"
              posRelativeY="238ed80972cc4d19" buttonText="Set" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="1406018e4c78eb13" memberName="sliderTime"
          virtualName="" explicitFocusOrder="0" pos="256 16 398M 24" posRelativeX="238ed80972cc4d19"
          posRelativeY="238ed80972cc4d19" posRelativeW="238ed80972cc4d19"
          min="0.0" max="999999.0" int="0.1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="150" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="1c7a8ab484d0d51e" memberName="togglePolar"
                virtualName="" explicitFocusOrder="0" pos="71Rr 16 63 24" posRelativeX="238ed80972cc4d19"
                posRelativeY="238ed80972cc4d19" buttonText="Polar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="cfe28ecd7a138f26" memberName="buttonGo"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="238ed80972cc4d19"
              posRelativeY="238ed80972cc4d19" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="72ff250a95e104d5" memberName="labelG" virtualName=""
         explicitFocusOrder="0" pos="16 16 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="G:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="4e587d8ba9574cdb" memberName="sliderGroup"
          virtualName="" explicitFocusOrder="0" pos="48 16 136 24" min="0.0"
          max="10.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

