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
    sliderTime1->setRange (0, 10, 0.1);
    sliderTime1->setSliderStyle (juce::Slider::LinearBar);
    sliderTime1->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);

    togglePolar1.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar1.get());
    togglePolar1->setButtonText (TRANS("Polar"));

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
    sliderTime2->setRange (0, 10, 0.1);
    sliderTime2->setSliderStyle (juce::Slider::LinearBar);
    sliderTime2->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);

    togglePolar2.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar2.get());
    togglePolar2->setButtonText (TRANS("Polar"));

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
    sliderTime3->setRange (0, 10, 0.1);
    sliderTime3->setSliderStyle (juce::Slider::LinearBar);
    sliderTime3->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);

    togglePolar3.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar3.get());
    togglePolar3->setButtonText (TRANS("Polar"));

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
    sliderTime4->setRange (0, 10, 0.1);
    sliderTime4->setSliderStyle (juce::Slider::LinearBar);
    sliderTime4->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);

    togglePolar4.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (togglePolar4.get());
    togglePolar4->setButtonText (TRANS("Polar"));

    buttonGo4.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonGo4.get());
    buttonGo4->setButtonText (TRANS("Go"));
    buttonGo4->addListener (this);

    action1.reset (new AnimatorActionComponent (pSourceSet, "Action 1"));
    addAndMakeVisible (action1.get());
    action1->setName ("new component");

    action2.reset (new AnimatorActionComponent (pSourceSet, "Action 2"));
    addAndMakeVisible (action2.get());
    action2->setName ("new component");


    //[UserPreSize]
    String sliderSuffix = " s (Smoothing)";
    sliderTime1->setTextValueSuffix(sliderSuffix);
    sliderTime2->setTextValueSuffix(sliderSuffix);
    sliderTime3->setTextValueSuffix(sliderSuffix);
    sliderTime4->setTextValueSuffix(sliderSuffix);
    controlDimming();
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
    togglePolar1->setBounds (0 + (getWidth() - 0) - 70 - 63, 0 + 16, 63, 24);
    buttonGo1->setBounds (0 + (getWidth() - 0) - 9 - 55, 0 + 16, 55, 24);
    group2->setBounds (0, 48, getWidth() - 0, 48);
    buttonSet2->setBounds (0 + 16, 48 + 16, 55, 24);
    sliderTime2->setBounds (0 + 80, 48 + 16, (getWidth() - 0) - 216, 24);
    togglePolar2->setBounds (0 + (getWidth() - 0) - 70 - 63, 48 + 16, 63, 24);
    buttonGo2->setBounds (0 + (getWidth() - 0) - 9 - 55, 48 + 16, 55, 24);
    group3->setBounds (0, 96, getWidth() - 0, 48);
    buttonSet3->setBounds (0 + 16, 96 + 16, 55, 24);
    sliderTime3->setBounds (0 + 80, 96 + 16, (getWidth() - 0) - 216, 24);
    togglePolar3->setBounds (0 + (getWidth() - 0) - 70 - 63, 96 + 16, 63, 24);
    buttonGo3->setBounds (0 + (getWidth() - 0) - 9 - 55, 96 + 16, 55, 24);
    group4->setBounds (0, 144, getWidth() - 0, 48);
    buttonSet4->setBounds (0 + 16, 144 + 16, 55, 24);
    sliderTime4->setBounds (0 + 80, 144 + 16, (getWidth() - 0) - 216, 24);
    togglePolar4->setBounds (0 + (getWidth() - 0) - 70 - 63, 144 + 16, 63, 24);
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
    else if (buttonThatWasClicked == buttonGo4.get())
    {
        //[UserButtonCode_buttonGo4] -- add your button handler code here..
        calculateStepsTo(&set4, togglePolar4->getToggleState(), sliderTime4->getValue());
        //[/UserButtonCode_buttonGo4]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AnimatorComponent::timerCallback(int /*timerID*/)
{
    if(currentStep >= 0)
    {
        bool actionsPerformed = false;
        for(int i = 0; i < JucePlugin_MaxNumInputChannels; i++)
        {
            if(currentStep >= steps[i].size())
            {
                continue;
            }

            auto p = *steps[i].getUnchecked(currentStep);
            pSourceSet->setChannelXYZ(
                                    i,
                                    p.getX(),
                                    p.getY(),
                                    p.getZ());
            actionsPerformed = true;
        }

        for(int i = 0; i < MAXIMUM_NUMBER_OF_GROUPS; i++)
        {
            if(currentStep >= groupSteps[i].size())
            {
                continue;
            }

            auto p = *groupSteps[i].getUnchecked(currentStep);
            pSourceSet->setGroupXyz(i, p.getX(), p.getY(), p.getZ(), false);

            actionsPerformed = true;
        }

        if(actionsPerformed)
        {
            currentStep++;
        }
        else
        {
            currentStep = -1;
            return;
        }
    }
    else
    {
        performAction(&action1->action);
        performAction(&action2->action);
    }
}

void AnimatorComponent::performAction(AnimatorAction *pAction)
{
    double factor = STEP_TIMER_INTERVAL / 1000.0;
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

void AnimatorComponent::calculateStepsTo(PositionSet* pPositions, bool isPolar, double timeSec)
{
    for(auto& s : steps)
        s.clear();
    for(auto& s : groupSteps)
        s.clear();

    int stepCount = jmax(1, (int)(timeSec * 1000.0 / STEP_TIMER_INTERVAL));

    for(int i = 0; i < pSourceSet->size() && i < pPositions->sources.size(); i++)
    {
        if(!pSourceSet->get(i)->getEnabled())
            continue;

        Point3D<double> origin = *(pSourceSet->get(i)->getPoint());
        Point3D<double> target = *pPositions->sources.getUnchecked(i);

        calculateStepsTo(origin, target, &steps[i], isPolar, stepCount);
    }

    for(int i = 0; i < pSourceSet->groupCount() && i < MAXIMUM_NUMBER_OF_GROUPS && i < pPositions->groups.size(); i++)
    {
        if(!pSourceSet->getGroup(i)->getEnabled())
            continue;

        Point3D<double> origin = *(pSourceSet->getGroup(i)->getPoint());
        Point3D<double> target = *pPositions->groups.getUnchecked(i);

        calculateStepsTo(origin, target, &groupSteps[i], isPolar, stepCount);
    }

    currentStep = 0;
}

void AnimatorComponent::calculateStepsTo(Point3D<double> origin, Point3D<double> target, OwnedArray<Point3D<float> > *pStepArray, bool isPolar, int stepCount)
{
    if(isPolar)
    {
        double da = target.getAzimuth() - origin.getAzimuth();
        double de = target.getElevation() - origin.getElevation();
        double dd = target.getDistance() - origin.getDistance();
        for(int iStep = 0; iStep < stepCount; iStep++)
        {
            auto newPt = new Point3D<float>();
            newPt->setAed(
                            float(origin.getAzimuth() + da * (iStep + 1) / stepCount),
                            float(origin.getElevation() + de * (iStep + 1) / stepCount),
                            float(origin.getDistance() + dd * (iStep + 1) / stepCount));
            pStepArray->add(newPt);
        }
    }
    else
    {
        double dx = target.getX() - origin.getX();
        double dy = target.getY() - origin.getY();
        double dz = target.getZ() - origin.getZ();
        for(int iStep = 0; iStep < stepCount; iStep++)
        {
            pStepArray->add(new Point3D<float>(
                                            float(origin.getX() + dx * (iStep + 1) / stepCount),
                                            float(origin.getY() + dy * (iStep + 1) / stepCount),
                                            float(origin.getZ() + dz * (iStep + 1) / stepCount)));
        }
    }
}

void AnimatorComponent::setPreset(PositionSet *pSet)
{
    pSet->sources.clear();
    pSet->groups.clear();
    for(int i = 0; i < pSourceSet->size(); i++)
    {
        pSet->sources.add(new Point3D<double>(*pSourceSet->get(i)->getPoint()));
    }

    for(int i = 0; i < pSourceSet->groupCount(); i++)
    {
        pSet->groups.add(new Point3D<double>(*pSourceSet->getGroup(i)->getPoint()));
        Logger::writeToLog(String(pSourceSet->getGroup(i)->getPoint()->getX()) + "; " + String(pSourceSet->getGroup(i)->getPoint()->getY()));
    }
}

void AnimatorComponent::controlDimming()
{
    buttonGo1->setEnabled(set1.groups.size() > 0 || set1.sources.size() > 0);
    buttonGo2->setEnabled(set2.groups.size() > 0 || set2.sources.size() > 0);
    buttonGo3->setEnabled(set3.groups.size() > 0 || set3.sources.size() > 0);
    buttonGo4->setEnabled(set4.groups.size() > 0 || set4.sources.size() > 0);
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
          min="0.0" max="10.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <TOGGLEBUTTON name="new toggle button" id="1c7a8ab484d0d51e" memberName="togglePolar1"
                virtualName="" explicitFocusOrder="0" pos="70Rr 16 63 24" posRelativeX="238ed80972cc4d19"
                posRelativeY="238ed80972cc4d19" buttonText="Polar" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
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
          min="0.0" max="10.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <TOGGLEBUTTON name="new toggle button" id="dab804a7a5be0515" memberName="togglePolar2"
                virtualName="" explicitFocusOrder="0" pos="70Rr 16 63 24" posRelativeX="757958c6e56c2a33"
                posRelativeY="757958c6e56c2a33" buttonText="Polar" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
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
          min="0.0" max="10.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <TOGGLEBUTTON name="new toggle button" id="1dd6eb466e28fccb" memberName="togglePolar3"
                virtualName="" explicitFocusOrder="0" pos="70Rr 16 63 24" posRelativeX="e8336da307c67c03"
                posRelativeY="e8336da307c67c03" buttonText="Polar" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
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
          min="0.0" max="10.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <TOGGLEBUTTON name="new toggle button" id="54313aa8ce125a33" memberName="togglePolar4"
                virtualName="" explicitFocusOrder="0" pos="70Rr 16 63 24" posRelativeX="996694d54e1d1607"
                posRelativeY="996694d54e1d1607" buttonText="Polar" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="a4b18c8c85023520" memberName="buttonGo4"
              virtualName="" explicitFocusOrder="0" pos="9Rr 16 55 24" posRelativeX="996694d54e1d1607"
              posRelativeY="996694d54e1d1607" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="99e58b8d05f9c746" memberName="action1"
                    virtualName="" explicitFocusOrder="0" pos="0 200 50.118% 340"
                    class="AnimatorActionComponent" params="pSourceSet, &quot;Action 1&quot;"/>
  <GENERICCOMPONENT name="new component" id="658f4eddb6e6b401" memberName="action2"
                    virtualName="" explicitFocusOrder="0" pos="0Rr 200 50.118% 340"
                    class="AnimatorActionComponent" params="pSourceSet, &quot;Action 2&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

