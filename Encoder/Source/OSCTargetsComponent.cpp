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
//[/Headers]

#include "OSCTargetsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCTargetsComponent::OSCTargetsComponent (ChangeListener* pChangeListener, EncoderSettings* _pSettings, CustomOscTxPresetHelper* _pCustomOscTxPresetHelper)
    : pSettings(_pSettings), pCustomOscTxPresetHelper(_pCustomOscTxPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..
    customOscTableModel.reset(new CustomOscTableListModel(pSettings, this, this, pCustomOscTxPresetHelper));
    addChangeListener(pChangeListener);
    pCustomOscTxPresetHelper->addActionListener(this);
    //[/Constructor_pre]

    groupTargets.reset (new juce::GroupComponent ("groupTargets",
                                                  TRANS("Targets")));
    addAndMakeVisible (groupTargets.get());

    targetList.reset (new TableListBox());
    addAndMakeVisible (targetList.get());
    targetList->setName ("targetList");

    sliderInterval.reset (new juce::Slider ("sliderInterval"));
    addAndMakeVisible (sliderInterval.get());
    sliderInterval->setRange (10, 1000, 1);
    sliderInterval->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderInterval->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderInterval->addListener (this);

    labelInterval.reset (new juce::Label ("labelInterval",
                                          TRANS("Interval [ms]:")));
    addAndMakeVisible (labelInterval.get());
    labelInterval->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelInterval->setJustificationType (juce::Justification::centredRight);
    labelInterval->setEditable (false, false, false);
    labelInterval->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelInterval->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnAdd.reset (new juce::TextButton ("btnAdd"));
    addAndMakeVisible (btnAdd.get());
    btnAdd->setButtonText (TRANS("add"));
    btnAdd->addListener (this);

    btnDelete.reset (new juce::TextButton ("btnDelete"));
    addAndMakeVisible (btnDelete.get());
    btnDelete->setButtonText (TRANS("delete"));
    btnDelete->addListener (this);

    btnInfo.reset (new juce::ImageButton ("btnInfo"));
    addAndMakeVisible (btnInfo.get());
    btnInfo->setButtonText (TRANS("new button"));
    btnInfo->addListener (this);

    btnInfo->setImages (false, true, true,
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                        juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    toggleSendContinuous.reset (new juce::ToggleButton ("toggleSendContinuous"));
    addAndMakeVisible (toggleSendContinuous.get());
    toggleSendContinuous->setTooltip (TRANS("If enabled, positions will constantly be sent, otherwise only if the values change."));
    toggleSendContinuous->setButtonText (TRANS("Continuous"));
    toggleSendContinuous->addListener (this);

    toggleSendContinuous->setBounds (8, 8, 104, 24);

    btnManagePresets.reset (new juce::TextButton ("btnManagePresets"));
    addAndMakeVisible (btnManagePresets.get());
    btnManagePresets->setButtonText (TRANS("presets..."));
    btnManagePresets->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderInterval->setSkewFactorFromMidPoint(200.0);
    sliderInterval->setValue(pSettings->oscSendExtIntervalMs, dontSendNotification);
    toggleSendContinuous->setToggleState(pSettings->oscSendExtContinuousFlag, dontSendNotification);

    customOscTableModel->initTable(targetList.get());

    controlDimming();
    //[/Constructor]
}

OSCTargetsComponent::~OSCTargetsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pCustomOscTxPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    groupTargets = nullptr;
    targetList = nullptr;
    sliderInterval = nullptr;
    labelInterval = nullptr;
    btnAdd = nullptr;
    btnDelete = nullptr;
    btnInfo = nullptr;
    toggleSendContinuous = nullptr;
    btnManagePresets = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    customOscTableModel = nullptr;
    //[/Destructor]
}

//==============================================================================
void OSCTargetsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCTargetsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupTargets->setBounds (0, 40, getWidth() - 0, getHeight() - 40);
    targetList->setBounds (0 + 16, 40 + 24, (getWidth() - 0) - 32, (getHeight() - 40) - 65);
    sliderInterval->setBounds (getWidth() - 8 - 210, 8, 210, 24);
    labelInterval->setBounds (getWidth() - 334, 8, 104, 24);
    btnAdd->setBounds (0 + (getWidth() - 0) - 16 - 70, 40 + (getHeight() - 40) - 10 - 24, 70, 24);
    btnDelete->setBounds (0 + (getWidth() - 0) - 92 - 70, 40 + (getHeight() - 40) - 10 - 24, 70, 24);
    btnInfo->setBounds (0 + 16, 40 + (getHeight() - 40) - 10 - 24, 23, 24);
    btnManagePresets->setBounds (0 + 134 - 86, 40 + (getHeight() - 40) - 10 - 24, 86, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCTargetsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderInterval.get())
    {
        //[UserSliderCode_sliderInterval] -- add your slider handling code here..
        pSettings->oscSendExtIntervalMs = int(sliderInterval->getValue());
        //[/UserSliderCode_sliderInterval]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
    //[/UsersliderValueChanged_Post]
}

void OSCTargetsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnAdd.get())
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
        pSettings->customOscTargets.add(new CustomOscTarget());
        targetList->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
        int row = customOscTableModel->getCustomTargetIndex(targetList->getSelectedRow());
        if (row >= 0 && row < pSettings->customOscTargets.size())
            pSettings->customOscTargets.remove(row);
        targetList->updateContent();
        //[/UserButtonCode_btnDelete]
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        //[UserButtonCode_btnInfo] -- add your button handler code here..
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText(COMMON_OSC_INFO_STRING + String("\n\nJavaScript:\nEvery object can also be dynamically calculated using a JavaScript expression:\n{expr, #}\nwhere # defines the expression.\n\n The object p represents the current source point, the following methods provide access to the point's information:\n- p.getX()\n- p.getY()\n- p.getZ()\n- p.getA()\n- p.getE()\n- p.getD()\n- p.getName()\n- p.getColor()\n- p.getGain()\n- p.getIndex()\n\nExample to calculate the distance from (0,0,0) of the projection to the XY-plane:\n{expr, Math.sqrt(Math.pow(p.getX(), 2) + Math.pow(p.getY(), 2))}"));
        textEditor->setSize(390, 250);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfo->getBounds(), this);
        //[/UserButtonCode_btnInfo]
    }
    else if (buttonThatWasClicked == toggleSendContinuous.get())
    {
        //[UserButtonCode_toggleSendContinuous] -- add your button handler code here..
        pSettings->oscSendExtContinuousFlag = toggleSendContinuous->getToggleState();
        //[/UserButtonCode_toggleSendContinuous]
    }
    else if (buttonThatWasClicked == btnManagePresets.get())
    {
        //[UserButtonCode_btnManagePresets] -- add your button handler code here..
        presetManagerDialog.show(this, pCustomOscTxPresetHelper);
        //[/UserButtonCode_btnManagePresets]
    }

    //[UserbuttonClicked_Post]
    sendChangeMessage();
    controlDimming();
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCTargetsComponent::controlDimming()
{
    btnDelete->setEnabled(targetList->getSelectedRows().size() > 0 && customOscTableModel->getCustomTargetIndex(targetList->getSelectedRow()) >= 0);
}

void OSCTargetsComponent::actionListenerCallback(const String& message)
{
    controlDimming();

    if (message == ACTION_MESSAGE_DATA_CHANGED)
        sendChangeMessage();

    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        // for now, nothing to do
    }

    if(message == ACTION_MESSAGE_PRESET_CHANGED)
    {
        // for now, nothing to do
    }
    if(message.startsWith(pCustomOscTxPresetHelper->UniqueActionMessageSelectPreset()))
    {
        CustomOscTarget* t = new CustomOscTarget();
        String file = message.fromFirstOccurrenceOf(pCustomOscTxPresetHelper->UniqueActionMessageSelectPreset(), false, true);
        pCustomOscTxPresetHelper->loadFromXmlFile(file, t);

        // add to list if not yet existing
        bool existing = false;
        for(auto& e : pSettings->customOscTargets)
        {
            if(e->oscString == t->oscString && e->targetHost == t->targetHost && e->targetPort == t->targetPort){
                existing = true;
                break;
            }
        }
        if(!existing)
        {
            // always add inactive
            t->enabledFlag = false;
            pSettings->customOscTargets.add(t);
            targetList->updateContent();
        }
    }
    if(message.startsWith(pCustomOscTxPresetHelper->UniqueActionMessageSavePreset()))
    {
        int index = message.getTrailingIntValue();

        if(index < 0 || index >= pSettings->customOscTargets.size())
            return;

        pCustomOscTxPresetHelper->tryCreateNewPreset([&,index](File* f) { pCustomOscTxPresetHelper->writeToXmlFile(*f, pSettings->customOscTargets[index]); });
    }
}

void OSCTargetsComponent::textEditorTextChanged(TextEditor& /*textEditor*/)
{
    sendChangeMessage();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCTargetsComponent" componentName=""
                 parentClasses="public juce::Component, public ChangeBroadcaster, public TextEditor::Listener, public ActionListener"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* _pSettings, CustomOscTxPresetHelper* _pCustomOscTxPresetHelper"
                 variableInitialisers="pSettings(_pSettings), pCustomOscTxPresetHelper(_pCustomOscTxPresetHelper)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="groupTargets" id="5ccced30e0050e9" memberName="groupTargets"
                  virtualName="" explicitFocusOrder="0" pos="0 40 0M 40M" title="Targets"/>
  <GENERICCOMPONENT name="targetList" id="78a64bf0c7700896" memberName="targetList"
                    virtualName="" explicitFocusOrder="0" pos="16 24 32M 65M" posRelativeX="5ccced30e0050e9"
                    posRelativeY="5ccced30e0050e9" posRelativeW="5ccced30e0050e9"
                    posRelativeH="5ccced30e0050e9" class="TableListBox" params=""/>
  <SLIDER name="sliderInterval" id="d02cfad6a67a7536" memberName="sliderInterval"
          virtualName="" explicitFocusOrder="0" pos="8Rr 8 210 24" posRelativeX="4e210261f2d82b98"
          posRelativeY="4e210261f2d82b98" min="10.0" max="1000.0" int="1.0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelInterval" id="d3a3f463e2503e2f" memberName="labelInterval"
         virtualName="" explicitFocusOrder="0" pos="334R 8 104 24" posRelativeX="4e210261f2d82b98"
         posRelativeY="4e210261f2d82b98" edTextCol="ff000000" edBkgCol="0"
         labelText="Interval [ms]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="btnAdd" id="239ce64ab2ee06e6" memberName="btnAdd" virtualName=""
              explicitFocusOrder="0" pos="16Rr 10Rr 70 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="add" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnDelete" id="4ddb1b3ba4cb8ca0" memberName="btnDelete"
              virtualName="" explicitFocusOrder="0" pos="92Rr 10Rr 70 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="delete" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="btnInfo" id="9b16466da29090a6" memberName="btnInfo" virtualName=""
               explicitFocusOrder="0" pos="16 10Rr 23 24" posRelativeX="5ccced30e0050e9"
               posRelativeY="5ccced30e0050e9" buttonText="new button" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal="help_png"
               opacityNormal="1.0" colourNormal="0" resourceOver="help_png"
               opacityOver="0.4000000059604645" colourOver="6eee1010" resourceDown="help_png"
               opacityDown="1.0" colourDown="c0ee1010"/>
  <TOGGLEBUTTON name="toggleSendContinuous" id="1424e4a474921c5c" memberName="toggleSendContinuous"
                virtualName="" explicitFocusOrder="0" pos="8 8 104 24" posRelativeX="4e210261f2d82b98"
                posRelativeY="4e210261f2d82b98" tooltip="If enabled, positions will constantly be sent, otherwise only if the values change."
                buttonText="Continuous" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TEXTBUTTON name="btnManagePresets" id="e79fc007bc779712" memberName="btnManagePresets"
              virtualName="" explicitFocusOrder="0" pos="134r 10Rr 86 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="presets..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
