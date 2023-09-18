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
#include "../../Common/AmbiSourceSet.h"
//[/Headers]

#include "AnimatorActionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AnimatorActionComponent::AnimatorActionComponent (AmbiSourceSet* _pSourceSet, AnimatorAction* _pAnimatorAction)
    : pSourceSet(_pSourceSet), pAnimatorAction(_pAnimatorAction)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

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
    labelGroup->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationY->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationZ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationOriginX->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationOriginY->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelRotationOriginZ->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
    labelStretchSimple->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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


    //[UserPreSize]
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
    //[/UserPreSize]

    setSize (300, 340);


    //[Constructor] You can add your own custom stuff here..
    refreshControls();
    //[/Constructor]
}

AnimatorActionComponent::~AnimatorActionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AnimatorActionComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AnimatorActionComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

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
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AnimatorActionComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxGroup.get())
    {
        //[UserComboBoxCode_comboBoxGroup] -- add your combo box handling code here..
        pAnimatorAction->groupIndex = comboBoxGroup->getSelectedId() - 1;
        //[/UserComboBoxCode_comboBoxGroup]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void AnimatorActionComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleEnable.get())
    {
        //[UserButtonCode_toggleEnable] -- add your button handler code here..
        pAnimatorAction->enabled = toggleEnable->getToggleState();
        //[/UserButtonCode_toggleEnable]
    }
    else if (buttonThatWasClicked == buttonReset.get())
    {
        //[UserButtonCode_buttonReset] -- add your button handler code here..
        pAnimatorAction->reset();
        refreshControls();
        //[/UserButtonCode_buttonReset]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AnimatorActionComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderRotationX.get())
    {
        //[UserSliderCode_sliderRotationX] -- add your slider handling code here..
        pAnimatorAction->rotationX = sliderRotationX->getValue();
        //[/UserSliderCode_sliderRotationX]
    }
    else if (sliderThatWasMoved == sliderRotationY.get())
    {
        //[UserSliderCode_sliderRotationY] -- add your slider handling code here..
        pAnimatorAction->rotationY = sliderRotationY->getValue();
        //[/UserSliderCode_sliderRotationY]
    }
    else if (sliderThatWasMoved == sliderRotationZ.get())
    {
        //[UserSliderCode_sliderRotationZ] -- add your slider handling code here..
        pAnimatorAction->rotationZ = sliderRotationZ->getValue();
        //[/UserSliderCode_sliderRotationZ]
    }
    else if (sliderThatWasMoved == sliderRotationOriginX.get())
    {
        //[UserSliderCode_sliderRotationOriginX] -- add your slider handling code here..
        pAnimatorAction->rotationOriginX = sliderRotationOriginX->getValue();
        //[/UserSliderCode_sliderRotationOriginX]
    }
    else if (sliderThatWasMoved == sliderRotationOriginY.get())
    {
        //[UserSliderCode_sliderRotationOriginY] -- add your slider handling code here..
        pAnimatorAction->rotationOriginY = sliderRotationOriginY->getValue();
        //[/UserSliderCode_sliderRotationOriginY]
    }
    else if (sliderThatWasMoved == sliderRotationOriginZ.get())
    {
        //[UserSliderCode_sliderRotationOriginZ] -- add your slider handling code here..
        pAnimatorAction->rotationOriginZ = sliderRotationOriginZ->getValue();
        //[/UserSliderCode_sliderRotationOriginZ]
    }
    else if (sliderThatWasMoved == sliderStretchSimple.get())
    {
        //[UserSliderCode_sliderStretchSimple] -- add your slider handling code here..
        pAnimatorAction->stretch = sliderStretchSimple->getValue();
        //[/UserSliderCode_sliderStretchSimple]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AnimatorActionComponent"
                 componentName="" parentClasses="public juce::Component" constructorParams="AmbiSourceSet* _pSourceSet, AnimatorAction* _pAnimatorAction"
                 variableInitialisers="pSourceSet(_pSourceSet), pAnimatorAction(_pAnimatorAction)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="300" initialHeight="340">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="673732692faf9b89" memberName="groupMain"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 336" title="Action 1"/>
  <COMBOBOX name="new combo box" id="ed3afd4d8c1363b3" memberName="comboBoxGroup"
            virtualName="" explicitFocusOrder="0" pos="8Rr 24 90M 24" posRelativeX="673732692faf9b89"
            posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="951754df8632ee6f" memberName="labelGroup"
         virtualName="" explicitFocusOrder="0" pos="8 24 55 24" posRelativeX="673732692faf9b89"
         posRelativeY="673732692faf9b89" edTextCol="ff000000" edBkgCol="0"
         labelText="Group:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="1f874fc211e238d4" memberName="toggleEnable"
                virtualName="" explicitFocusOrder="0" pos="8 8Rr 150 24" posRelativeX="673732692faf9b89"
                posRelativeY="673732692faf9b89" buttonText="Enable" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GROUPCOMPONENT name="new group" id="c332cd434b4c2689" memberName="groupRotation"
                  virtualName="" explicitFocusOrder="0" pos="8 56 16M 96" posRelativeX="673732692faf9b89"
                  posRelativeY="673732692faf9b89" title="Rotation around group point"/>
  <LABEL name="new label" id="a738a146576388c5" memberName="labelRotationX"
         virtualName="" explicitFocusOrder="0" pos="8 16 56 24" posRelativeX="c332cd434b4c2689"
         posRelativeY="c332cd434b4c2689" edTextCol="ff000000" edBkgCol="0"
         labelText="X-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="832fbd7caa85651a" memberName="sliderRotationX"
          virtualName="" explicitFocusOrder="0" pos="72 16 80M 24" posRelativeX="c332cd434b4c2689"
          posRelativeY="c332cd434b4c2689" posRelativeW="c332cd434b4c2689"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="40706dfba362ab64" memberName="labelRotationY"
         virtualName="" explicitFocusOrder="0" pos="8 40 56 24" posRelativeX="c332cd434b4c2689"
         posRelativeY="c332cd434b4c2689" edTextCol="ff000000" edBkgCol="0"
         labelText="Y-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="9ed2979150ab453b" memberName="sliderRotationY"
          virtualName="" explicitFocusOrder="0" pos="72 40 80M 24" posRelativeX="c332cd434b4c2689"
          posRelativeY="c332cd434b4c2689" posRelativeW="c332cd434b4c2689"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="6000f1da296678d0" memberName="labelRotationZ"
         virtualName="" explicitFocusOrder="0" pos="8 64 56 24" posRelativeX="c332cd434b4c2689"
         posRelativeY="c332cd434b4c2689" edTextCol="ff000000" edBkgCol="0"
         labelText="Z-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="d1f372080e9eb8af" memberName="sliderRotationZ"
          virtualName="" explicitFocusOrder="0" pos="72 64 80M 24" posRelativeX="c332cd434b4c2689"
          posRelativeY="c332cd434b4c2689" posRelativeW="c332cd434b4c2689"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GROUPCOMPONENT name="new group" id="9c4336420a3598a" memberName="groupRotationOrigin"
                  virtualName="" explicitFocusOrder="0" pos="8 152 16M 96" posRelativeX="673732692faf9b89"
                  posRelativeY="673732692faf9b89" title="Rotation around origin"/>
  <LABEL name="new label" id="74c87b28bfb30891" memberName="labelRotationOriginX"
         virtualName="" explicitFocusOrder="0" pos="8 16 56 24" posRelativeX="9c4336420a3598a"
         posRelativeY="9c4336420a3598a" edTextCol="ff000000" edBkgCol="0"
         labelText="X-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="cb586f37c2ddbc5c" memberName="sliderRotationOriginX"
          virtualName="" explicitFocusOrder="0" pos="72 16 80M 24" posRelativeX="9c4336420a3598a"
          posRelativeY="9c4336420a3598a" posRelativeW="9c4336420a3598a"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="1bd15b5ae0fd1923" memberName="labelRotationOriginY"
         virtualName="" explicitFocusOrder="0" pos="8 40 56 24" posRelativeX="9c4336420a3598a"
         posRelativeY="9c4336420a3598a" edTextCol="ff000000" edBkgCol="0"
         labelText="Y-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="b7df5737c97e09dc" memberName="sliderRotationOriginY"
          virtualName="" explicitFocusOrder="0" pos="72 40 80M 24" posRelativeX="9c4336420a3598a"
          posRelativeY="9c4336420a3598a" posRelativeW="9c4336420a3598a"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="84ba510cb6b68a20" memberName="labelRotationOriginZ"
         virtualName="" explicitFocusOrder="0" pos="8 64 56 24" posRelativeX="9c4336420a3598a"
         posRelativeY="9c4336420a3598a" edTextCol="ff000000" edBkgCol="0"
         labelText="Z-Axis" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="a506853108f424b4" memberName="sliderRotationOriginZ"
          virtualName="" explicitFocusOrder="0" pos="72 64 80M 24" posRelativeX="9c4336420a3598a"
          posRelativeY="9c4336420a3598a" posRelativeW="9c4336420a3598a"
          min="-360.0" max="360.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GROUPCOMPONENT name="new group" id="92689b605a1ca01a" memberName="groupStretch"
                  virtualName="" explicitFocusOrder="0" pos="8 248 16M 48" posRelativeX="673732692faf9b89"
                  posRelativeY="673732692faf9b89" posRelativeW="673732692faf9b89"
                  title="Stretch"/>
  <LABEL name="new label" id="eac7d2a6ff0d4c64" memberName="labelStretchSimple"
         virtualName="" explicitFocusOrder="0" pos="8 16 56 24" posRelativeX="92689b605a1ca01a"
         posRelativeY="92689b605a1ca01a" edTextCol="ff000000" edBkgCol="0"
         labelText="Simple" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="39716a26f90ba9d8" memberName="sliderStretchSimple"
          virtualName="" explicitFocusOrder="0" pos="72 16 80M 24" posRelativeX="92689b605a1ca01a"
          posRelativeY="92689b605a1ca01a" posRelativeW="92689b605a1ca01a"
          min="-5.0" max="5.0" int="0.01" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="new button" id="51ff62138879b660" memberName="buttonReset"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 111 24" posRelativeX="673732692faf9b89"
              posRelativeY="673732692faf9b89" buttonText="reset" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

