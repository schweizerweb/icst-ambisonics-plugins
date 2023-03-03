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
//[/Headers]

#include "AnimatorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AnimatorComponent::AnimatorComponent (AmbiSourceSet* pSourceSet, AnimatorDataset* pAnimatorDataset)
    : pSourceSet(pSourceSet), pAnimatorDataset(pAnimatorDataset)
{
    //[Constructor_pre] You can add your own custom stuff here..
    while(pAnimatorDataset->actions.size() < 2)
    {
        auto a = new AnimatorAction();
        a->name = "Action " + String(pAnimatorDataset->actions.size() + 1);
        pAnimatorDataset->actions.add(a);
    }
    while(pAnimatorDataset->movements.size() < 4)
    {
        auto a = new AnimatorMovement();
        a->name = "Movement " + String(pAnimatorDataset->movements.size() + 1);
        pAnimatorDataset->movements.add(a);
    }
    //[/Constructor_pre]

    action1.reset (new AnimatorActionComponent (pSourceSet, pAnimatorDataset->actions[0]));
    addAndMakeVisible (action1.get());
    action1->setName ("new component");

    action2.reset (new AnimatorActionComponent (pSourceSet, pAnimatorDataset->actions[1]));
    addAndMakeVisible (action2.get());
    action2->setName ("new component");

    preset1.reset (new AnimatorMovementComponent (pSourceSet, pAnimatorDataset->movements[0], this));
    addAndMakeVisible (preset1.get());
    preset1->setName ("preset1");

    preset2.reset (new AnimatorMovementComponent (pSourceSet, pAnimatorDataset->movements[1], this));
    addAndMakeVisible (preset2.get());
    preset2->setName ("preset2");

    preset3.reset (new AnimatorMovementComponent (pSourceSet, pAnimatorDataset->movements[2], this));
    addAndMakeVisible (preset3.get());
    preset3->setName ("preset3");

    preset4.reset (new AnimatorMovementComponent (pSourceSet, pAnimatorDataset->movements[3], this));
    addAndMakeVisible (preset4.get());
    preset4->setName ("preset4");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 300);


    //[Constructor] You can add your own custom stuff here..
    refreshControls();
    startTimer(STEP_TIMER_ID, STEP_TIMER_INTERVAL);
    //[/Constructor]
}

AnimatorComponent::~AnimatorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    stopTimer(STEP_TIMER_ID);
    //[/Destructor_pre]

    action1 = nullptr;
    action2 = nullptr;
    preset1 = nullptr;
    preset2 = nullptr;
    preset3 = nullptr;
    preset4 = nullptr;


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

    action1->setBounds (0, 200, proportionOfWidth (0.5000f), 340);
    action2->setBounds (getWidth() - proportionOfWidth (0.5000f), 200, proportionOfWidth (0.5000f), 340);
    preset1->setBounds (0, 0, getWidth() - 0, 48);
    preset2->setBounds (0, 48, getWidth() - 0, 48);
    preset3->setBounds (0, 96, getWidth() - 0, 48);
    preset4->setBounds (0, 144, getWidth() - 0, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AnimatorComponent::refreshControls()
{
    // nothing to do
}

void AnimatorComponent::actionListenerCallback(const juce::String &message)
{
#if MULTI_ENCODER_MODE
    AnimatorMovement* pMov = nullptr;
    for(auto& m : pAnimatorDataset->movements)
    {
        if(m->name == message)
        {
            pMov = m;
            break;
        }
    }

    if(pMov != nullptr)
    {
        int stepCount = jmax(1, (int)(pMov->timeSpanSeconds * 1000.0 / STEP_TIMER_INTERVAL));

        for(auto& p : pMov->groupPositions)
        {
            int groupIndex = p.first;

            // clear existing steps
            groupSteps[groupIndex].clear();

            auto group = pSourceSet->getGroup(groupIndex);
            if(group != nullptr && group->getEnabled())
            {
                Point3D<double> origin = *(group->getRawPoint());
                Point3D<double> target = p.second;
                calculateStepsTo(origin, target, &groupSteps[groupIndex], pMov->polarFlag, stepCount);
                currentStep[groupIndex] = 0;
            }
        }
    }
#endif
}

void AnimatorComponent::timerCallback(int /*timerID*/)
{
#if MULTI_ENCODER_MODE
    for(int i = 0; i < MAXIMUM_NUMBER_OF_GROUPS; i++)
    {
        if(currentStep[i] >= 0 && currentStep[i] < groupSteps[i].size())
        {
            auto p = *groupSteps[i].getUnchecked(currentStep[i]);
            pSourceSet->setGroupXyz(i, p.getX(), p.getY(), p.getZ(), true);
            currentStep[i]++;
        }
        else
            currentStep[i] = -1;
    }
#endif

    // actions
    for(auto& a : pAnimatorDataset->actions)
        performAction(a);
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

void AnimatorComponent::setPreset(PositionSet *pSet, int groupIndex)
{
    pSet->sources.clear();
    pSet->groups.clear();

    /*
    for(int i = 0; i < pSourceSet->size(); i++)
    {
        pSet->sources.add(new Point3D<double>(*pSourceSet->get(i)->getPoint()));
    }

    for(int i = 0; i < pSourceSet->groupCount(); i++)
    {
        pSet->groups.add(new Point3D<double>(*pSourceSet->getGroup(i)->getPoint()));
        Logger::writeToLog(String(pSourceSet->getGroup(i)->getPoint()->getX()) + "; " + String(pSourceSet->getGroup(i)->getPoint()->getY()));
    }
     */
    pSet->groups.add(new Point3D<double>(*pSourceSet->getGroup(groupIndex)->getRawPoint()));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AnimatorComponent" componentName=""
                 parentClasses="public juce::Component, public MultiTimer, public ActionListener"
                 constructorParams="AmbiSourceSet* pSourceSet, AnimatorDataset* pAnimatorDataset"
                 variableInitialisers="pSourceSet(pSourceSet), pAnimatorDataset(pAnimatorDataset)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="700" initialHeight="300">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="new component" id="99e58b8d05f9c746" memberName="action1"
                    virtualName="" explicitFocusOrder="0" pos="0 200 50% 340" class="AnimatorActionComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;actions[0]"/>
  <GENERICCOMPONENT name="new component" id="658f4eddb6e6b401" memberName="action2"
                    virtualName="" explicitFocusOrder="0" pos="0Rr 200 50% 340" class="AnimatorActionComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;actions[1]"/>
  <GENERICCOMPONENT name="preset1" id="4b1fefe0b208f633" memberName="preset1" virtualName=""
                    explicitFocusOrder="0" pos="0 0 0M 48" class="AnimatorMovementComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;movements[0], this"/>
  <GENERICCOMPONENT name="preset2" id="cbf853ccee5a58c5" memberName="preset2" virtualName=""
                    explicitFocusOrder="0" pos="0 48 0M 48" class="AnimatorMovementComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;movements[1], this"/>
  <GENERICCOMPONENT name="preset3" id="de661489792c3777" memberName="preset3" virtualName=""
                    explicitFocusOrder="0" pos="0 96 0M 48" class="AnimatorMovementComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;movements[2], this"/>
  <GENERICCOMPONENT name="preset4" id="8250d85fe0faebe7" memberName="preset4" virtualName=""
                    explicitFocusOrder="0" pos="0 144 0M 48" class="AnimatorMovementComponent"
                    params="pSourceSet, pAnimatorDataset-&gt;movements[3], this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

