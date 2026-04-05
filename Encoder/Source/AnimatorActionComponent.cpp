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
#include "AnimatorActionComponent.h"

AnimatorActionComponent::AnimatorActionComponent (AmbiSourceSet* _pSourceSet, AnimatorAction* _pAnimatorAction)
    : pSourceSet(_pSourceSet), pAnimatorAction(_pAnimatorAction)
{
    groupMain.reset (new juce::GroupComponent ("new group",
                                               TRANS("Action 1")));
    addAndMakeVisible (groupMain.get());

    comboBoxGroup.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (comboBoxGroup.get());
    comboBoxGroup->setEditableText (false);
    comboBoxGroup->setJustificationType (juce::Justification::centredLeft);
    comboBoxGroup->setTextWhenNothingSelected (juce::String());
    comboBoxGroup->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxGroup->addListener (this);

    labelGroup.reset (new juce::Label ("new label",
                                       TRANS("Group:")));
    addAndMakeVisible (labelGroup.get());
    labelGroup->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelGroup->setJustificationType (juce::Justification::centredLeft);
    labelGroup->setEditable (false, false, false);
    labelGroup->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGroup->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    toggleEnable.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleEnable.get());
    toggleEnable->setButtonText (TRANS("Enable"));
    toggleEnable->addListener (this);

    groupRotation.reset (new juce::GroupComponent ("new group",
                                                   TRANS("Rotation around group point")));
    addAndMakeVisible (groupRotation.get());

    labelRotationX.reset (new juce::Label ("new label",
                                           TRANS("X-Axis")));
    addAndMakeVisible (labelRotationX.get());
    labelRotationX->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationX->setJustificationType (juce::Justification::centredLeft);
    labelRotationX->setEditable (false, false, false);
    labelRotationX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationX.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationX.get());
    sliderRotationX->setRange (-360, 360, 0.1);
    sliderRotationX->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationX->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationX->addListener (this);

    labelRotationY.reset (new juce::Label ("new label",
                                           TRANS("Y-Axis")));
    addAndMakeVisible (labelRotationY.get());
    labelRotationY->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationY->setJustificationType (juce::Justification::centredLeft);
    labelRotationY->setEditable (false, false, false);
    labelRotationY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationY.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationY.get());
    sliderRotationY->setRange (-360, 360, 0.1);
    sliderRotationY->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationY->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationY->addListener (this);

    labelRotationZ.reset (new juce::Label ("new label",
                                           TRANS("Z-Axis")));
    addAndMakeVisible (labelRotationZ.get());
    labelRotationZ->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationZ->setJustificationType (juce::Justification::centredLeft);
    labelRotationZ->setEditable (false, false, false);
    labelRotationZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationZ.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationZ.get());
    sliderRotationZ->setRange (-360, 360, 0.1);
    sliderRotationZ->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationZ->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationZ->addListener (this);

    groupRotationOrigin.reset (new juce::GroupComponent ("new group",
                                                         TRANS("Rotation around origin")));
    addAndMakeVisible (groupRotationOrigin.get());

    labelRotationOriginX.reset (new juce::Label ("new label",
                                                 TRANS("X-Axis")));
    addAndMakeVisible (labelRotationOriginX.get());
    labelRotationOriginX->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationOriginX->setJustificationType (juce::Justification::centredLeft);
    labelRotationOriginX->setEditable (false, false, false);
    labelRotationOriginX->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationOriginX->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationOriginX.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationOriginX.get());
    sliderRotationOriginX->setRange (-360, 360, 0.1);
    sliderRotationOriginX->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationOriginX->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationOriginX->addListener (this);

    labelRotationOriginY.reset (new juce::Label ("new label",
                                                 TRANS("Y-Axis")));
    addAndMakeVisible (labelRotationOriginY.get());
    labelRotationOriginY->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationOriginY->setJustificationType (juce::Justification::centredLeft);
    labelRotationOriginY->setEditable (false, false, false);
    labelRotationOriginY->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationOriginY->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationOriginY.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationOriginY.get());
    sliderRotationOriginY->setRange (-360, 360, 0.1);
    sliderRotationOriginY->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationOriginY->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationOriginY->addListener (this);

    labelRotationOriginZ.reset (new juce::Label ("new label",
                                                 TRANS("Z-Axis")));
    addAndMakeVisible (labelRotationOriginZ.get());
    labelRotationOriginZ->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelRotationOriginZ->setJustificationType (juce::Justification::centredLeft);
    labelRotationOriginZ->setEditable (false, false, false);
    labelRotationOriginZ->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRotationOriginZ->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderRotationOriginZ.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderRotationOriginZ.get());
    sliderRotationOriginZ->setRange (-360, 360, 0.1);
    sliderRotationOriginZ->setSliderStyle (juce::Slider::LinearBar);
    sliderRotationOriginZ->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderRotationOriginZ->addListener (this);

    groupStretch.reset (new juce::GroupComponent ("new group",
                                                  TRANS("Stretch")));
    addAndMakeVisible (groupStretch.get());

    labelStretchSimple.reset (new juce::Label ("new label",
                                               TRANS("Simple")));
    addAndMakeVisible (labelStretchSimple.get());
    labelStretchSimple->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelStretchSimple->setJustificationType (juce::Justification::centredLeft);
    labelStretchSimple->setEditable (false, false, false);
    labelStretchSimple->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelStretchSimple->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    sliderStretchSimple.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (sliderStretchSimple.get());
    sliderStretchSimple->setRange (-5, 5, 0.01);
    sliderStretchSimple->setSliderStyle (juce::Slider::LinearBar);
    sliderStretchSimple->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderStretchSimple->addListener (this);

    buttonReset.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (buttonReset.get());
    buttonReset->setButtonText (TRANS("reset"));
    buttonReset->addListener (this);

    groupMain->setText(pAnimatorAction->name);
    for(int i = 0; i < pSourceSet->groupCount(); i++)
    {
        comboBoxGroup->addItem(String(i + 1) + ": " + pSourceSet->getGroup(i)->getName(), i + 1);
    }

    String rotationUnit = CharPointer_UTF8(" [°/s]");
    sliderRotationX->setTextValueSuffix(rotationUnit);
    sliderRotationY->setTextValueSuffix(rotationUnit);
    sliderRotationZ->setTextValueSuffix(rotationUnit);
    sliderRotationOriginX->setTextValueSuffix(rotationUnit);
    sliderRotationOriginY->setTextValueSuffix(rotationUnit);
    sliderRotationOriginZ->setTextValueSuffix(rotationUnit);
    sliderStretchSimple->setTextValueSuffix(" [m/s]");

    setSize (300, 340);

    refreshControls();
}

AnimatorActionComponent::~AnimatorActionComponent()
{
    groupMain = nullptr;
    comboBoxGroup = nullptr;
    labelGroup = nullptr;
    toggleEnable = nullptr;
    groupRotation = nullptr;
    labelRotationX = nullptr;
    sliderRotationX = nullptr;
    labelRotationY = nullptr;
    sliderRotationY = nullptr;
    labelRotationZ = nullptr;
    sliderRotationZ = nullptr;
    groupRotationOrigin = nullptr;
    labelRotationOriginX = nullptr;
    sliderRotationOriginX = nullptr;
    labelRotationOriginY = nullptr;
    sliderRotationOriginY = nullptr;
    labelRotationOriginZ = nullptr;
    sliderRotationOriginZ = nullptr;
    groupStretch = nullptr;
    labelStretchSimple = nullptr;
    sliderStretchSimple = nullptr;
    buttonReset = nullptr;
}

void AnimatorActionComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void AnimatorActionComponent::resized()
{
    groupMain->setBounds (0, 0, getWidth() - 0, 336);
    comboBoxGroup->setBounds (0 + (getWidth() - 0) - 8 - ((getWidth() - 0) - 90), 0 + 24, (getWidth() - 0) - 90, 24);
    labelGroup->setBounds (0 + 8, 0 + 24, 55, 24);
    toggleEnable->setBounds (0 + 8, 0 + 336 - 8 - 24, 150, 24);
    groupRotation->setBounds (0 + 8, 0 + 56, getWidth() - 16, 96);
    labelRotationX->setBounds ((0 + 8) + 8, (0 + 56) + 16, 56, 24);
    sliderRotationX->setBounds ((0 + 8) + 72, (0 + 56) + 16, (getWidth() - 16) - 80, 24);
    labelRotationY->setBounds ((0 + 8) + 8, (0 + 56) + 40, 56, 24);
    sliderRotationY->setBounds ((0 + 8) + 72, (0 + 56) + 40, (getWidth() - 16) - 80, 24);
    labelRotationZ->setBounds ((0 + 8) + 8, (0 + 56) + 64, 56, 24);
    sliderRotationZ->setBounds ((0 + 8) + 72, (0 + 56) + 64, (getWidth() - 16) - 80, 24);
    groupRotationOrigin->setBounds (0 + 8, 0 + 152, getWidth() - 16, 96);
    labelRotationOriginX->setBounds ((0 + 8) + 8, (0 + 152) + 16, 56, 24);
    sliderRotationOriginX->setBounds ((0 + 8) + 72, (0 + 152) + 16, (getWidth() - 16) - 80, 24);
    labelRotationOriginY->setBounds ((0 + 8) + 8, (0 + 152) + 40, 56, 24);
    sliderRotationOriginY->setBounds ((0 + 8) + 72, (0 + 152) + 40, (getWidth() - 16) - 80, 24);
    labelRotationOriginZ->setBounds ((0 + 8) + 8, (0 + 152) + 64, 56, 24);
    sliderRotationOriginZ->setBounds ((0 + 8) + 72, (0 + 152) + 64, (getWidth() - 16) - 80, 24);
    groupStretch->setBounds (0 + 8, 0 + 248, (getWidth() - 0) - 16, 48);
    labelStretchSimple->setBounds ((0 + 8) + 8, (0 + 248) + 16, 56, 24);
    sliderStretchSimple->setBounds ((0 + 8) + 72, (0 + 248) + 16, ((getWidth() - 0) - 16) - 80, 24);
    buttonReset->setBounds (0 + (getWidth() - 0) - 8 - 111, 0 + 336 - 8 - 24, 111, 24);
}

void AnimatorActionComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBoxGroup.get())
    {
        pAnimatorAction->groupIndex = comboBoxGroup->getSelectedId() - 1;
    }
}

void AnimatorActionComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggleEnable.get())
    {
        pAnimatorAction->enabled = toggleEnable->getToggleState();
    }
    else if (buttonThatWasClicked == buttonReset.get())
    {
        pAnimatorAction->reset();
        refreshControls();
    }
}

void AnimatorActionComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderRotationX.get())
    {
        pAnimatorAction->rotationX = sliderRotationX->getValue();
    }
    else if (sliderThatWasMoved == sliderRotationY.get())
    {
        pAnimatorAction->rotationY = sliderRotationY->getValue();
    }
    else if (sliderThatWasMoved == sliderRotationZ.get())
    {
        pAnimatorAction->rotationZ = sliderRotationZ->getValue();
    }
    else if (sliderThatWasMoved == sliderRotationOriginX.get())
    {
        pAnimatorAction->rotationOriginX = sliderRotationOriginX->getValue();
    }
    else if (sliderThatWasMoved == sliderRotationOriginY.get())
    {
        pAnimatorAction->rotationOriginY = sliderRotationOriginY->getValue();
    }
    else if (sliderThatWasMoved == sliderRotationOriginZ.get())
    {
        pAnimatorAction->rotationOriginZ = sliderRotationOriginZ->getValue();
    }
    else if (sliderThatWasMoved == sliderStretchSimple.get())
    {
        pAnimatorAction->stretch = sliderStretchSimple->getValue();
    }
}

void AnimatorActionComponent::refreshControls()
{
    sliderRotationX->setValue(pAnimatorAction->rotationX, dontSendNotification);
    sliderRotationY->setValue(pAnimatorAction->rotationY, dontSendNotification);
    sliderRotationZ->setValue(pAnimatorAction->rotationZ, dontSendNotification);
    sliderRotationOriginX->setValue(pAnimatorAction->rotationOriginX, dontSendNotification);
    sliderRotationOriginY->setValue(pAnimatorAction->rotationOriginY, dontSendNotification);
    sliderRotationOriginZ->setValue(pAnimatorAction->rotationOriginZ, dontSendNotification);
    sliderStretchSimple->setValue(pAnimatorAction->stretch, dontSendNotification);
    comboBoxGroup->setSelectedId(pAnimatorAction->groupIndex + 1, dontSendNotification);
    toggleEnable->setToggleState(pAnimatorAction->enabled, dontSendNotification);
}

void AnimatorActionComponent::setData(AnimatorAction* pAction)
{
    this->pAnimatorAction = pAction;
    refreshControls();
}
