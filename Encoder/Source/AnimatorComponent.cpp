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

#include "AnimatorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AnimatorComponent::AnimatorComponent (AmbiSourceSet* pSourceSet)
    : pSourceSet(pSourceSet)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    group1.reset (new juce::GroupComponent ("new group",
                                            TRANS("1")));
    addAndMakeVisible (group1.get());

    buttonSet1.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonSet1.get());
    buttonSet1->setButtonText (TRANS("Set"));
    buttonSet1->addListener (this);

    sliderTime1.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderTime1.get());
    sliderTime1->setRange (0, 10, 1);
    sliderTime1->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderTime1->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderTime1->addListener (this);

    togglePolar1.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar1.get());
    togglePolar1->setButtonText (TRANS("Polar"));
    togglePolar1->addListener (this);

    buttonGo1.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo1.get());
    buttonGo1->setButtonText (TRANS("Go"));
    buttonGo1->addListener (this);

    group2.reset (new juce::GroupComponent ("new group",
                                            TRANS("2")));
    addAndMakeVisible (group2.get());

    buttonSet2.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonSet2.get());
    buttonSet2->setButtonText (TRANS("Set"));
    buttonSet2->addListener (this);

    sliderTime2.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderTime2.get());
    sliderTime2->setRange (0, 10, 1);
    sliderTime2->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderTime2->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderTime2->addListener (this);

    togglePolar2.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar2.get());
    togglePolar2->setButtonText (TRANS("Polar"));
    togglePolar2->addListener (this);

    buttonGo2.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo2.get());
    buttonGo2->setButtonText (TRANS("Go"));
    buttonGo2->addListener (this);

    groupRotationOrigin.reset (new juce::GroupComponent ("new group",
                                                         TRANS("Rotation Origin")));
    addAndMakeVisible (groupRotationOrigin.get());

    groupRotationOrigin->setBounds (0, 0, 264, 160);

    comboBoxRotateOrigin.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (comboBoxRotateOrigin.get());
    comboBoxRotateOrigin->setEditableText (false);
    comboBoxRotateOrigin->setJustificationType (juce::Justification::centredLeft);
    comboBoxRotateOrigin->setTextWhenNothingSelected (juce::String());
    comboBoxRotateOrigin->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxRotateOrigin->addListener (this);

    sliderRotateOriginTime.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotateOriginTime.get());
    sliderRotateOriginTime->setRange (0, 10, 0.1);
    sliderRotateOriginTime->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderRotateOriginTime->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotateOriginTime->addListener (this);

    labelGroup.reset (new juce::Label ("new label",
                                       TRANS("Group:")));
    addAndMakeVisible (labelGroup.get());
    labelGroup->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelGroup->setJustificationType (juce::Justification::centredLeft);
    labelGroup->setEditable (false, false, false);
    labelGroup->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGroup->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelTime.reset (new juce::Label ("new label",
                                      TRANS("Time:")));
    addAndMakeVisible (labelTime.get());
    labelTime->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelTime->setJustificationType (juce::Justification::centredLeft);
    labelTime->setEditable (false, false, false);
    labelTime->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelTime->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotateOriginRepetoitions.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotateOriginRepetoitions.get());
    sliderRotateOriginRepetoitions->setRange (0, 1000, 1);
    sliderRotateOriginRepetoitions->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderRotateOriginRepetoitions->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotateOriginRepetoitions->addListener (this);

    labelRepetitions.reset (new juce::Label ("new label",
                                             TRANS("Rep:")));
    addAndMakeVisible (labelRepetitions.get());
    labelRepetitions->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelRepetitions->setJustificationType (juce::Justification::centredLeft);
    labelRepetitions->setEditable (false, false, false);
    labelRepetitions->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRepetitions->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonRotationOriginGo.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonRotationOriginGo.get());
    buttonRotationOriginGo->setButtonText (TRANS("Go"));
    buttonRotationOriginGo->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 300);


    //[Constructor] You can add your own custom stuff here..
    for(int i = 0; i < pSourceSet->groupCount(); i++)
    {
        comboBoxRotateOrigin->addItem(String(i) + " " + pSourceSet->getGroup(i)->getName(), i + 1);
    }
    
    startTimer(STEP_TIMER_ID, STEP_TIMER_INTERVAL);
    //[/Constructor]
}

AnimatorComponent::~AnimatorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    stopTimer(STEP_TIMER_ID);
    
    for(auto s : steps)
        delete(s);
    steps.clear();
    //[/Destructor_pre]

    group1 = nullptr;
    buttonSet1 = nullptr;
    sliderTime1 = nullptr;
    togglePolar1 = nullptr;
    buttonGo1 = nullptr;
    group2 = nullptr;
    buttonSet2 = nullptr;
    sliderTime2 = nullptr;
    togglePolar2 = nullptr;
    buttonGo2 = nullptr;
    groupRotationOrigin = nullptr;
    comboBoxRotateOrigin = nullptr;
    sliderRotateOriginTime = nullptr;
    labelGroup = nullptr;
    labelTime = nullptr;
    sliderRotateOriginRepetoitions = nullptr;
    labelRepetitions = nullptr;
    buttonRotationOriginGo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AnimatorComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AnimatorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    group1->setBounds (272, 0, getWidth() - 272, 48);
    buttonSet1->setBounds (272 + 16, 0 + 16, 55, 24);
    sliderTime1->setBounds (272 + 80, 0 + 16, (getWidth() - 272) - 216, 24);
    togglePolar1->setBounds (272 + (getWidth() - 272) - 73 - 63, 0 + 16, 63, 24);
    buttonGo1->setBounds (272 + (getWidth() - 272) - 9 - 55, 0 + 16, 55, 24);
    group2->setBounds (272, 48, getWidth() - 272, 48);
    buttonSet2->setBounds (272 + 16, 48 + 16, 55, 24);
    sliderTime2->setBounds (272 + 80, 48 + 16, (getWidth() - 272) - 216, 24);
    togglePolar2->setBounds (272 + (getWidth() - 272) - 73 - 63, 48 + 16, 63, 24);
    buttonGo2->setBounds (272 + (getWidth() - 272) - 9 - 55, 48 + 16, 55, 24);
    comboBoxRotateOrigin->setBounds (0 + 80, 0 + 24, 264 - 96, 24);
    sliderRotateOriginTime->setBounds (0 + 80, 0 + 56, 264 - 90, 24);
    labelGroup->setBounds (0 + 16, 0 + 24, 55, 24);
    labelTime->setBounds (0 + 16, 0 + 56, 55, 24);
    sliderRotateOriginRepetoitions->setBounds (0 + 80, 0 + 88, 264 - 90, 24);
    labelRepetitions->setBounds (0 + 16, 0 + 88, 55, 24);
    buttonRotationOriginGo->setBounds (0 + 16, 0 + 120, 264 - 32, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AnimatorComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSet1.get())
    {
        //[UserButtonCode_buttonSet1] -- add your button handler code here..
        set1.clear();
        for(int i = 0; i < pSourceSet->size(); i++)
            set1.add(new AmbiSource(pSourceSet->get(i)));
        //[/UserButtonCode_buttonSet1]
    }
    else if (buttonThatWasClicked == togglePolar1.get())
    {
        //[UserButtonCode_togglePolar1] -- add your button handler code here..
        //[/UserButtonCode_togglePolar1]
    }
    else if (buttonThatWasClicked == buttonGo1.get())
    {
        //[UserButtonCode_buttonGo1] -- add your button handler code here..
        calculateStepsTo(&set1, togglePolar1->getToggleState(), sliderTime1->getValue());
        //[/UserButtonCode_buttonGo1]
    }
    else if (buttonThatWasClicked == buttonSet2.get())
    {
        //[UserButtonCode_buttonSet2] -- add your button handler code here..
        set2.clear();
        for(int i = 0; i < pSourceSet->size(); i++)
            set2.add(new AmbiSource(pSourceSet->get(i)));
        //[/UserButtonCode_buttonSet2]
    }
    else if (buttonThatWasClicked == togglePolar2.get())
    {
        //[UserButtonCode_togglePolar2] -- add your button handler code here..
        //[/UserButtonCode_togglePolar2]
    }
    else if (buttonThatWasClicked == buttonGo2.get())
    {
        //[UserButtonCode_buttonGo2] -- add your button handler code here..
        calculateStepsTo(&set2, togglePolar2->getToggleState(), sliderTime2->getValue());
        //[/UserButtonCode_buttonGo2]
    }
    else if (buttonThatWasClicked == buttonRotationOriginGo.get())
    {
        //[UserButtonCode_buttonRotationOriginGo] -- add your button handler code here..
        //[/UserButtonCode_buttonRotationOriginGo]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AnimatorComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderTime1.get())
    {
        //[UserSliderCode_sliderTime1] -- add your slider handling code here..
        //[/UserSliderCode_sliderTime1]
    }
    else if (sliderThatWasMoved == sliderTime2.get())
    {
        //[UserSliderCode_sliderTime2] -- add your slider handling code here..
        //[/UserSliderCode_sliderTime2]
    }
    else if (sliderThatWasMoved == sliderRotateOriginTime.get())
    {
        //[UserSliderCode_sliderRotateOriginTime] -- add your slider handling code here..
        //[/UserSliderCode_sliderRotateOriginTime]
    }
    else if (sliderThatWasMoved == sliderRotateOriginRepetoitions.get())
    {
        //[UserSliderCode_sliderRotateOriginRepetoitions] -- add your slider handling code here..
        //[/UserSliderCode_sliderRotateOriginRepetoitions]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void AnimatorComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxRotateOrigin.get())
    {
        //[UserComboBoxCode_comboBoxRotateOrigin] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBoxRotateOrigin]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AnimatorComponent::timerCallback(int timerID)
{
    if(steps.size() > 0 && currentStep >= 0)
    {
        HashMap<int, OwnedArray<Point3D<float>>*>::Iterator i (steps);
 
        while (i.next())
        {
            if(currentStep >= i.getValue()->size())
            {
                currentStep = -1;
                return;
            }
            
            auto p = *i.getValue()->getUnchecked(currentStep);
            pSourceSet->setChannelXYZ(
                                    i.getKey(),
                                    p.getX(),
                                    p.getY(),
                                    p.getZ());
        }
            
        currentStep++;
    }
}

void AnimatorComponent::calculateStepsTo(OwnedArray<AmbiSource>* pPositions, bool isPolar, double timeSec)
{
    for(auto s : steps)
        delete(s);
    steps.clear();
    
    int stepsCount = jmax(1, (int)(timeSec * 1000.0 / STEP_TIMER_INTERVAL));
    
    // match points
    for(int i = 0; i < pSourceSet->size(); i++)
    {
        if(!pSourceSet->get(i)->getEnabled())
            continue;
        
        String name = pSourceSet->get(i)->getName();
        Point3D<double> origin = *(pSourceSet->get(i)->getPoint());
        int j;
        for(j = 0; j < pPositions->size(); j++)
        {
            if(pPositions->getUnchecked(j)->getName() == name)
            {
                break;
            }
        }
        
        if(j < pPositions->size() && pPositions->getUnchecked(j)->getEnabled())
        {
            // means match
            steps.set(j, new OwnedArray<Point3D<float>>());
            Point3D<double> p = *pPositions->getUnchecked(j)->getPoint();
            if(isPolar)
            {
                double da = p.getAzimuth() - origin.getAzimuth();
                double de = p.getElevation() - origin.getElevation();
                double dd = p.getDistance() - origin.getDistance();
                for(int iStep = 0; iStep < stepsCount; iStep++)
                {
                    auto newPt = new Point3D<float>();
                    newPt->setAed(
                                  origin.getAzimuth() + da * (iStep + 1) / stepsCount,
                                  origin.getElevation() + de * (iStep + 1) / stepsCount,
                                  origin.getDistance() + dd * (iStep + 1) / stepsCount);
                    steps[j]->add(newPt);
                }
            }
            else
            {
                double dx = p.getX() - origin.getX();
                double dy = p.getY() - origin.getY();
                double dz = p.getZ() - origin.getZ();
                for(int iStep = 0; iStep < stepsCount; iStep++)
                {
                    steps[j]->add(new Point3D<float>(
                                                    origin.getX() + dx * (iStep + 1) / stepsCount,
                                                    origin.getY() + dy * (iStep + 1) / stepsCount,
                                                    origin.getZ() + dz * (iStep + 1) / stepsCount));
                }
            }
        }
    }
    
    currentStep = 0;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AnimatorComponent" componentName=""
                 parentClasses="public juce::Component, public MultiTimer" constructorParams="AmbiSourceSet* pSourceSet"
                 variableInitialisers="pSourceSet(pSourceSet)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="700" initialHeight="300">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="238ed80972cc4d19" memberName="group1" virtualName=""
                  explicitFocusOrder="0" pos="272 0 272M 48" title="1"/>
  <TEXTBUTTON name="new button" id="6f8d2fc9df3bb657" memberName="buttonSet1"
              virtualName="" explicitFocusOrder="0" pos="16 16 55 24" posRelativeX="238ed80972cc4d19"
              posRelativeY="238ed80972cc4d19" buttonText="Set" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="1406018e4c78eb13" memberName="sliderTime1"
          virtualName="" explicitFocusOrder="0" pos="80 16 216M 24" posRelativeX="238ed80972cc4d19"
          posRelativeY="238ed80972cc4d19" posRelativeW="238ed80972cc4d19"
          min="0.0" max="10.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="1c7a8ab484d0d51e" memberName="togglePolar1"
                virtualName="" explicitFocusOrder="0" pos="73Rr 16 63 24" posRelativeX="238ed80972cc4d19"
                posRelativeY="238ed80972cc4d19" buttonText="Polar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="cfe28ecd7a138f26" memberName="buttonGo1"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="238ed80972cc4d19"
              posRelativeY="238ed80972cc4d19" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="new group" id="757958c6e56c2a33" memberName="group2" virtualName=""
                  explicitFocusOrder="0" pos="272 48 272M 48" title="2"/>
  <TEXTBUTTON name="new button" id="5d84d95099e73bb4" memberName="buttonSet2"
              virtualName="" explicitFocusOrder="0" pos="16 16 55 24" posRelativeX="757958c6e56c2a33"
              posRelativeY="757958c6e56c2a33" buttonText="Set" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="2bb561484b6a5dbb" memberName="sliderTime2"
          virtualName="" explicitFocusOrder="0" pos="80 16 216M 24" posRelativeX="757958c6e56c2a33"
          posRelativeY="757958c6e56c2a33" posRelativeW="757958c6e56c2a33"
          min="0.0" max="10.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="dab804a7a5be0515" memberName="togglePolar2"
                virtualName="" explicitFocusOrder="0" pos="73Rr 16 63 24" posRelativeX="757958c6e56c2a33"
                posRelativeY="757958c6e56c2a33" buttonText="Polar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="770f8258412948ef" memberName="buttonGo2"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="757958c6e56c2a33"
              posRelativeY="757958c6e56c2a33" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="new group" id="673732692faf9b89" memberName="groupRotationOrigin"
                  virtualName="" explicitFocusOrder="0" pos="0 0 264 160" title="Rotation Origin"/>
  <COMBOBOX name="new combo box" id="ed3afd4d8c1363b3" memberName="comboBoxRotateOrigin"
            virtualName="" explicitFocusOrder="0" pos="80 24 96M 24" posRelativeX="673732692faf9b89"
            posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="832fbd7caa85651a" memberName="sliderRotateOriginTime"
          virtualName="" explicitFocusOrder="0" pos="80 56 90M 24" posRelativeX="673732692faf9b89"
          posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
          min="0.0" max="10.0" int="0.1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="951754df8632ee6f" memberName="labelGroup"
         virtualName="" explicitFocusOrder="0" pos="16 24 55 24" posRelativeX="673732692faf9b89"
         posRelativeY="673732692faf9b89" edTextCol="ff000000" edBkgCol="0"
         labelText="Group:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="96c8b57f2e002997" memberName="labelTime"
         virtualName="" explicitFocusOrder="0" pos="16 56 55 24" posRelativeX="673732692faf9b89"
         posRelativeY="673732692faf9b89" edTextCol="ff000000" edBkgCol="0"
         labelText="Time:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="5778d425eb8e0dd3" memberName="sliderRotateOriginRepetoitions"
          virtualName="" explicitFocusOrder="0" pos="80 88 90M 24" posRelativeX="673732692faf9b89"
          posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
          min="0.0" max="1000.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="bdc1861864d7c7c3" memberName="labelRepetitions"
         virtualName="" explicitFocusOrder="0" pos="16 88 55 24" posRelativeX="673732692faf9b89"
         posRelativeY="673732692faf9b89" edTextCol="ff000000" edBkgCol="0"
         labelText="Rep:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="f7860822b4ee837e" memberName="buttonRotationOriginGo"
              virtualName="" explicitFocusOrder="0" pos="16 120 32M 24" posRelativeX="673732692faf9b89"
              posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
              buttonText="Go" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

