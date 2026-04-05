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

#include "../../Common/AmbiSourceSet.h"
#include "AnimatorMovementComponent.h"

AnimatorMovementComponent::AnimatorMovementComponent (AmbiSourceSet* _pSourceSet, AnimatorMovement* _pAnimatorMovement, ActionListener* pActionListener)
    : pSourceSet(_pSourceSet), pAnimatorMovement(_pAnimatorMovement)
{
    addActionListener(pActionListener);

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
    labelG->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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

    group1->setText(pAnimatorMovement->name);
    controlDimming();

    setSize (600, 100);

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
}

AnimatorMovementComponent::~AnimatorMovementComponent()
{
    removeAllActionListeners();

    group1 = nullptr;
    buttonSet = nullptr;
    sliderTime = nullptr;
    togglePolar = nullptr;
    buttonGo = nullptr;
    labelG = nullptr;
    sliderGroup = nullptr;
}

void AnimatorMovementComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void AnimatorMovementComponent::resized()
{
    group1->setBounds (0, 0, getWidth() - 0, 48);
    buttonSet->setBounds (0 + 192, 0 + 16, 55, 24);
    sliderTime->setBounds (0 + 256, 0 + 16, (getWidth() - 0) - 398, 24);
    togglePolar->setBounds (0 + (getWidth() - 0) - 71 - 63, 0 + 16, 63, 24);
    buttonGo->setBounds (0 + (getWidth() - 0) - 9 - 55, 0 + 16, 55, 24);
    sliderTime->setTextBoxStyle(Slider::TextBoxLeft, false, jmax(0, sliderTime->getWidth() - 50), sliderTime->getHeight());
}

void AnimatorMovementComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == buttonSet.get())
    {
        int groupIndex = (int)sliderGroup->getValue()-1;
        pAnimatorMovement->groupPositions.clear();
        Point3D<double> p(*pSourceSet->getGroup(groupIndex)->getRawPoint());
        pAnimatorMovement->groupPositions[groupIndex] = p;
    }
    else if (buttonThatWasClicked == togglePolar.get())
    {
        pAnimatorMovement->polarFlag = togglePolar->getToggleState();
    }
    else if (buttonThatWasClicked == buttonGo.get())
    {
        sendActionMessage(pAnimatorMovement->name);
    }

    controlDimming();
}

void AnimatorMovementComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderTime.get())
    {
        pAnimatorMovement->timeSpanSeconds = sliderTime->getValue();
    }
    else if (sliderThatWasMoved == sliderGroup.get())
    {
        pAnimatorMovement->groupPositions.clear();
        controlDimming();
    }
}

void AnimatorMovementComponent::refreshControls()
{
    if(!pAnimatorMovement->groupPositions.empty())
    //for(auto p : pAnimatorMovement->groupPositions)
    {
        // temp solution because only one group allowed
        auto p = *pAnimatorMovement->groupPositions.begin();
    
        sliderGroup->setValue(p.first + 1, dontSendNotification);
        //break;
    }

    sliderTime->setValue(pAnimatorMovement->timeSpanSeconds);
    togglePolar->setToggleState(pAnimatorMovement->polarFlag, dontSendNotification);
    controlDimming();
}

void AnimatorMovementComponent::setData(AnimatorMovement* pMovement)
{
    pAnimatorMovement = pMovement;
    refreshControls();
}

void AnimatorMovementComponent::controlDimming()
{
    bool en = pAnimatorMovement->groupPositions.size() > 0;
    buttonGo->setEnabled(en);
    buttonSet->setColour(TextButton::buttonColourId, en ? Colours::green : Colours::orangered);
}
