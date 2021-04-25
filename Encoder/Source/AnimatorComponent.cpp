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
                                            TRANS("Preset 1")));
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
                                            TRANS("Preset 2")));
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

    group3.reset (new juce::GroupComponent ("new group",
                                            TRANS("Preset 3")));
    addAndMakeVisible (group3.get());

    buttonSet3.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonSet3.get());
    buttonSet3->setButtonText (TRANS("Set"));
    buttonSet3->addListener (this);

    sliderTime3.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderTime3.get());
    sliderTime3->setRange (0, 10, 1);
    sliderTime3->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderTime3->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderTime3->addListener (this);

    togglePolar3.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar3.get());
    togglePolar3->setButtonText (TRANS("Polar"));
    togglePolar3->addListener (this);

    buttonGo3.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo3.get());
    buttonGo3->setButtonText (TRANS("Go"));
    buttonGo3->addListener (this);

    group4.reset (new juce::GroupComponent ("new group",
                                            TRANS("Preset 4")));
    addAndMakeVisible (group4.get());

    buttonSet4.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonSet4.get());
    buttonSet4->setButtonText (TRANS("Set"));
    buttonSet4->addListener (this);

    sliderTime4.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderTime4.get());
    sliderTime4->setRange (0, 10, 1);
    sliderTime4->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderTime4->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderTime4->addListener (this);

    togglePolar4.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar4.get());
    togglePolar4->setButtonText (TRANS("Polar"));
    togglePolar4->addListener (this);

    buttonGo4.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo4.get());
    buttonGo4->setButtonText (TRANS("Go"));
    buttonGo4->addListener (this);

    action1.reset (new AnimatorActionComponent (pSourceSet));
    addAndMakeVisible (action1.get());
    action1->setName ("new component");

    action2.reset (new AnimatorActionComponent (pSourceSet));
    addAndMakeVisible (action2.get());
    action2->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 300);


    //[Constructor] You can add your own custom stuff here..

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
    group3 = nullptr;
    buttonSet3 = nullptr;
    sliderTime3 = nullptr;
    togglePolar3 = nullptr;
    buttonGo3 = nullptr;
    group4 = nullptr;
    buttonSet4 = nullptr;
    sliderTime4 = nullptr;
    togglePolar4 = nullptr;
    buttonGo4 = nullptr;
    action1 = nullptr;
    action2 = nullptr;


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

    group1->setBounds (0, 0, getWidth() - 0, 48);
    buttonSet1->setBounds (0 + 16, 0 + 16, 55, 24);
    sliderTime1->setBounds (0 + 80, 0 + 16, (getWidth() - 0) - 216, 24);
    togglePolar1->setBounds (0 + (getWidth() - 0) - 73 - 63, 0 + 16, 63, 24);
    buttonGo1->setBounds (0 + (getWidth() - 0) - 9 - 55, 0 + 16, 55, 24);
    group2->setBounds (0, 48, getWidth() - 0, 48);
    buttonSet2->setBounds (0 + 16, 48 + 16, 55, 24);
    sliderTime2->setBounds (0 + 80, 48 + 16, (getWidth() - 0) - 216, 24);
    togglePolar2->setBounds (0 + (getWidth() - 0) - 73 - 63, 48 + 16, 63, 24);
    buttonGo2->setBounds (0 + (getWidth() - 0) - 9 - 55, 48 + 16, 55, 24);
    group3->setBounds (0, 96, getWidth() - 0, 48);
    buttonSet3->setBounds (0 + 16, 96 + 16, 55, 24);
    sliderTime3->setBounds (0 + 80, 96 + 16, (getWidth() - 0) - 216, 24);
    togglePolar3->setBounds (0 + (getWidth() - 0) - 73 - 63, 96 + 16, 63, 24);
    buttonGo3->setBounds (0 + (getWidth() - 0) - 9 - 55, 96 + 16, 55, 24);
    group4->setBounds (0, 144, getWidth() - 0, 48);
    buttonSet4->setBounds (0 + 16, 144 + 16, 55, 24);
    sliderTime4->setBounds (0 + 80, 144 + 16, (getWidth() - 0) - 216, 24);
    togglePolar4->setBounds (0 + (getWidth() - 0) - 73 - 63, 144 + 16, 63, 24);
    buttonGo4->setBounds (0 + (getWidth() - 0) - 9 - 55, 144 + 16, 55, 24);
    action1->setBounds (0, 200, proportionOfWidth (0.5012f), 340);
    action2->setBounds (getWidth() - proportionOfWidth (0.5012f), 200, proportionOfWidth (0.5012f), 340);
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
        setPreset(&set1);
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
        setPreset(&set2);
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
    else if (buttonThatWasClicked == buttonSet3.get())
    {
        //[UserButtonCode_buttonSet3] -- add your button handler code here..
        setPreset(&set3);
        //[/UserButtonCode_buttonSet3]
    }
    else if (buttonThatWasClicked == togglePolar3.get())
    {
        //[UserButtonCode_togglePolar3] -- add your button handler code here..
        //[/UserButtonCode_togglePolar3]
    }
    else if (buttonThatWasClicked == buttonGo3.get())
    {
        //[UserButtonCode_buttonGo3] -- add your button handler code here..
        calculateStepsTo(&set3, togglePolar3->getToggleState(), sliderTime3->getValue());
        //[/UserButtonCode_buttonGo3]
    }
    else if (buttonThatWasClicked == buttonSet4.get())
    {
        //[UserButtonCode_buttonSet4] -- add your button handler code here..
        setPreset(&set4);
        //[/UserButtonCode_buttonSet4]
    }
    else if (buttonThatWasClicked == togglePolar4.get())
    {
        //[UserButtonCode_togglePolar4] -- add your button handler code here..
        //[/UserButtonCode_togglePolar4]
    }
    else if (buttonThatWasClicked == buttonGo4.get())
    {
        //[UserButtonCode_buttonGo4] -- add your button handler code here..
        calculateStepsTo(&set4, togglePolar4->getToggleState(), sliderTime4->getValue());
        //[/UserButtonCode_buttonGo4]
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
    else if (sliderThatWasMoved == sliderTime3.get())
    {
        //[UserSliderCode_sliderTime3] -- add your slider handling code here..
        //[/UserSliderCode_sliderTime3]
    }
    else if (sliderThatWasMoved == sliderTime4.get())
    {
        //[UserSliderCode_sliderTime4] -- add your slider handling code here..
        //[/UserSliderCode_sliderTime4]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
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
    else
    {
        performAction(&action1->action);
        performAction(&action2->action);
    }
}

void AnimatorComponent::performAction(AnimatorAction *pAction)
{
    double factor = 1.0 / STEP_TIMER_INTERVAL;
    if(pAction->enabled && pAction->groupIndex >= 0 && pAction->groupIndex < pSourceSet->groupCount())
    {
        if(pAction->rotationX != 0.0 || pAction->rotationY != 0.0 || pAction->rotationZ != 0.0)
        {
            pSourceSet->rotateGroup(
                                    pAction->groupIndex,
                                    Constants::GradToRad(factor * pAction->rotationX),
                                    Constants::GradToRad(factor * pAction->rotationY),
                                    Constants::GradToRad(factor * pAction->rotationZ));
        }
        if(pAction->rotationOriginX != 0.0 || pAction->rotationOriginY != 0.0 || pAction->rotationOriginZ != 0.0)
        {
            pSourceSet->rotateGroupAroundOrigin(
                                                pAction->groupIndex,
                                                Constants::GradToRad(factor * pAction->rotationOriginX),
                                                Constants::GradToRad(factor * pAction->rotationOriginY),
                                                Constants::GradToRad(factor * pAction->rotationOriginZ),
                                                true);
        }
        if(pAction->stretch != 0.0)
        {
            pSourceSet->stretchGroup(pAction->groupIndex, factor * pAction->stretch);
        }
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

void AnimatorComponent::setPreset(OwnedArray<AmbiSource> *pSet)
{
    pSet->clear();
    for(int i = 0; i < pSourceSet->size(); i++)
        pSet->add(new AmbiSource(pSourceSet->get(i)));
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
                  explicitFocusOrder="0" pos="0 0 0M 48" title="Preset 1"/>
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
                  explicitFocusOrder="0" pos="0 48 0M 48" title="Preset 2"/>
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
  <GROUPCOMPONENT name="new group" id="e8336da307c67c03" memberName="group3" virtualName=""
                  explicitFocusOrder="0" pos="0 96 0M 48" title="Preset 3"/>
  <TEXTBUTTON name="new button" id="6514609962c325db" memberName="buttonSet3"
              virtualName="" explicitFocusOrder="0" pos="16 16 55 24" posRelativeX="e8336da307c67c03"
              posRelativeY="e8336da307c67c03" buttonText="Set" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="74aa9d1365e4e818" memberName="sliderTime3"
          virtualName="" explicitFocusOrder="0" pos="80 16 216M 24" posRelativeX="e8336da307c67c03"
          posRelativeY="e8336da307c67c03" posRelativeW="e8336da307c67c03"
          min="0.0" max="10.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="1dd6eb466e28fccb" memberName="togglePolar3"
                virtualName="" explicitFocusOrder="0" pos="73Rr 16 63 24" posRelativeX="e8336da307c67c03"
                posRelativeY="e8336da307c67c03" buttonText="Polar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="b5a1fad26134727a" memberName="buttonGo3"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="e8336da307c67c03"
              posRelativeY="e8336da307c67c03" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="new group" id="996694d54e1d1607" memberName="group4" virtualName=""
                  explicitFocusOrder="0" pos="0 144 0M 48" title="Preset 4"/>
  <TEXTBUTTON name="new button" id="16b3ac053bc2130" memberName="buttonSet4"
              virtualName="" explicitFocusOrder="0" pos="16 16 55 24" posRelativeX="996694d54e1d1607"
              posRelativeY="996694d54e1d1607" buttonText="Set" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="9bf6f40205390b0d" memberName="sliderTime4"
          virtualName="" explicitFocusOrder="0" pos="80 16 216M 24" posRelativeX="996694d54e1d1607"
          posRelativeY="996694d54e1d1607" posRelativeW="996694d54e1d1607"
          min="0.0" max="10.0" int="1.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="54313aa8ce125a33" memberName="togglePolar4"
                virtualName="" explicitFocusOrder="0" pos="73Rr 16 63 24" posRelativeX="996694d54e1d1607"
                posRelativeY="996694d54e1d1607" buttonText="Polar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="a4b18c8c85023520" memberName="buttonGo4"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="996694d54e1d1607"
              posRelativeY="996694d54e1d1607" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="99e58b8d05f9c746" memberName="action1"
                    virtualName="" explicitFocusOrder="0" pos="0 200 50.118% 340"
                    class="AnimatorActionComponent" params="pSourceSet"/>
  <GENERICCOMPONENT name="new component" id="658f4eddb6e6b401" memberName="action2"
                    virtualName="" explicitFocusOrder="0" pos="0Rr 200 50.118% 340"
                    class="AnimatorActionComponent" params="pSourceSet"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

