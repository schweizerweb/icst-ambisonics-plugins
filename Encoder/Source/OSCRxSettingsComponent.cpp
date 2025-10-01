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
#include "OSCTargetsComponent.h"
//[/Headers]

#include "OSCRxSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCRxSettingsComponent::OSCRxSettingsComponent (EncoderSettings* _pSettings, StatusMessageHandler* _pStatusMessageHandler, CustomOscRxPresetHelper* _pCustomOscRxPresetHelper, OSCLogDialogManager* _pOscLogManager, OSCHandlerEncoder* _pOscHandler)
    : pSettings(_pSettings), pStatusMessageHandler(_pStatusMessageHandler), pOscLogManager(_pOscLogManager), pCustomOscRxPresetHelper(_pCustomOscRxPresetHelper), pOscHandler(_pOscHandler)
{
    //[Constructor_pre] You can add your own custom stuff here..
    customOscTableModel.reset(new CustomOscInputTableListModel(pSettings, pOscHandler, this, this, pCustomOscRxPresetHelper));
    pCustomOscRxPresetHelper->addActionListener(this);
    //[/Constructor_pre]

    toggleReceiveOsc.reset (new juce::ToggleButton ("toggleReceiveOsc"));
    addAndMakeVisible (toggleReceiveOsc.get());
    toggleReceiveOsc->setButtonText (TRANS("Receive OSC"));
    toggleReceiveOsc->addListener (this);

    toggleReceiveOsc->setBounds (8, 10, 150, 24);

    labelOscPort.reset (new juce::Label ("labelOscPort",
                                         TRANS("Port:\n")));
    addAndMakeVisible (labelOscPort.get());
    labelOscPort->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelOscPort->setJustificationType (juce::Justification::centredRight);
    labelOscPort->setEditable (false, false, false);
    labelOscPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelOscPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelOscPort->setBounds (120, 10, 42, 24);

    sliderReceiveOscPort.reset (new juce::Slider ("sliderReceiveOscPort"));
    addAndMakeVisible (sliderReceiveOscPort.get());
    sliderReceiveOscPort->setRange (0, 65535, 1);
    sliderReceiveOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderReceiveOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderReceiveOscPort->addListener (this);

    sliderReceiveOscPort->setBounds (170, 10, 100, 24);

    groupDefinitions.reset (new juce::GroupComponent ("groupDefinitions",
                                                      TRANS("Definitions")));
    addAndMakeVisible (groupDefinitions.get());

    customOscList.reset (new TableListBox());
    addAndMakeVisible (customOscList.get());
    customOscList->setName ("customOscList");

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
    buttonShowOscLog.reset (new juce::TextButton ("buttonShowOscLog"));
    addAndMakeVisible (buttonShowOscLog.get());
    buttonShowOscLog->setButtonText (TRANS("Show OSC Log"));
    buttonShowOscLog->addListener (this);

    toggleHideWarnings.reset (new juce::ToggleButton ("toggleHideWarnings"));
    addAndMakeVisible (toggleHideWarnings.get());
    toggleHideWarnings->setButtonText (TRANS("hide warnings"));
    toggleHideWarnings->addListener (this);

    btnManagePresets.reset (new juce::TextButton ("btnManagePresets"));
    addAndMakeVisible (btnManagePresets.get());
    btnManagePresets->setButtonText (TRANS("presets..."));
    btnManagePresets->addListener (this);

    toggleReceiveStandardOsc.reset (new juce::ToggleButton ("toggleReceiveStandardOsc"));
    addAndMakeVisible (toggleReceiveStandardOsc.get());
    toggleReceiveStandardOsc->setButtonText (TRANS("Listen for Standard OSC"));
    toggleReceiveStandardOsc->addListener (this);

    toggleReceiveStandardOsc->setBounds (280, 10, 184, 24);

    btnInfoStandardOsc.reset (new juce::ImageButton ("btnInfo"));
    addAndMakeVisible (btnInfoStandardOsc.get());
    btnInfoStandardOsc->setButtonText (TRANS("new button"));
    btnInfoStandardOsc->addListener (this);

    btnInfoStandardOsc->setImages (false, true, true,
                                   juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0x00000000),
                                   juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                                   juce::ImageCache::getFromMemory (BinaryData::help_png, BinaryData::help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
    btnInfoStandardOsc->setBounds (464, 9, 23, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    sliderReceiveOscPort->setValue(pSettings->oscReceivePort, dontSendNotification);
    toggleReceiveStandardOsc->setToggleState(pSettings->oscHandleStandardFormatFlag, dontSendNotification);
    customOscTableModel->initTable(customOscList.get());
    toggleHideWarnings->setToggleState(pSettings->hideWarnings, dontSendNotification);
    controlDimming();
    //[/Constructor]
}

OSCRxSettingsComponent::~OSCRxSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pCustomOscRxPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    toggleReceiveOsc = nullptr;
    labelOscPort = nullptr;
    sliderReceiveOscPort = nullptr;
    groupDefinitions = nullptr;
    customOscList = nullptr;
    btnAdd = nullptr;
    btnDelete = nullptr;
    btnInfo = nullptr;
    buttonShowOscLog = nullptr;
    toggleHideWarnings = nullptr;
    btnManagePresets = nullptr;
    toggleReceiveStandardOsc = nullptr;
    btnInfoStandardOsc = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    customOscTableModel = nullptr;
    //[/Destructor]
}

//==============================================================================
void OSCRxSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCRxSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupDefinitions->setBounds (0, 40, getWidth() - 0, getHeight() - 44);
    customOscList->setBounds (0 + 16, 40 + 24, (getWidth() - 0) - 32, (getHeight() - 44) - 65);
    btnAdd->setBounds (0 + (getWidth() - 0) - 16 - 70, 40 + (getHeight() - 44) - 10 - 24, 70, 24);
    btnDelete->setBounds (0 + (getWidth() - 0) - 92 - 70, 40 + (getHeight() - 44) - 10 - 24, 70, 24);
    btnInfo->setBounds (0 + 16, 40 + (getHeight() - 44) - 10 - 24, 23, 24);
    buttonShowOscLog->setBounds (getWidth() - 9 - 120, 10, 120, 24);
    toggleHideWarnings->setBounds (getWidth() - 130 - 120, 10, 120, 24);
    btnManagePresets->setBounds (0 + 134 - 86, 40 + (getHeight() - 44) - 10 - 24, 86, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCRxSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleReceiveOsc.get())
    {
        //[UserButtonCode_toggleReceiveOsc] -- add your button handler code here..
        pSettings->oscReceiveFlag = toggleReceiveOsc->getToggleState();
        pOscHandler->initialize();
        //[/UserButtonCode_toggleReceiveOsc]
    }
    else if (buttonThatWasClicked == btnAdd.get())
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
        pSettings->customOscInput.add(new CustomOscInput());
        pOscHandler->initialize();
        customOscList->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
        int row = customOscTableModel->getCustomIndex(customOscList->getSelectedRow());
        if (row >= 0 && row < pSettings->customOscInput.size())
            pSettings->customOscInput.remove(row);
        pOscHandler->initialize();
        customOscList->updateContent();
        //[/UserButtonCode_btnDelete]
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        //[UserButtonCode_btnInfo] -- add your button handler code here..
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText(String(COMMON_OSC_INFO_STRING) + String("\nNote: constant values have to match, otherwise the entire message is ignored.\n\n{} = Require, but ignore this argument\n\nIf 'JS-Code' is not empty, JavaScript mode is used:\n- 'OSC-Message' defines the pattern to match incoming messages\n- 'JS-Code' defines the JavaScript code to be executed\n\nExample:\nOSC-Message: /path/to/source/*/xy\nJS-Code: s.setXYZ(s.path(4), s.arg(1), s.arg(2), s.arg(3))\nFor more information, see help in Code Editor."));
        textEditor->setSize(420, 520);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfo->getBounds(), this);
        //[/UserButtonCode_btnInfo]
    }
    else if (buttonThatWasClicked == buttonShowOscLog.get())
    {
        //[UserButtonCode_buttonShowOscLog] -- add your button handler code here..
        pOscLogManager->show(pStatusMessageHandler, this);
        //[/UserButtonCode_buttonShowOscLog]
    }
    else if (buttonThatWasClicked == toggleHideWarnings.get())
    {
        //[UserButtonCode_toggleHideWarnings] -- add your button handler code here..
        pSettings->hideWarnings = toggleHideWarnings->getToggleState();
        pOscHandler->initialize();
        //[/UserButtonCode_toggleHideWarnings]
    }
    else if (buttonThatWasClicked == btnManagePresets.get())
    {
        //[UserButtonCode_btnManagePresets] -- add your button handler code here..
        presetManagerDialog.show(this, pCustomOscRxPresetHelper);
        //[/UserButtonCode_btnManagePresets]
    }
    else if (buttonThatWasClicked == toggleReceiveStandardOsc.get())
    {
        //[UserButtonCode_toggleReceiveStandardOsc] -- add your button handler code here..
        pSettings->oscHandleStandardFormatFlag = toggleReceiveStandardOsc->getToggleState();
        pOscHandler->initialize();
        //[/UserButtonCode_toggleReceiveStandardOsc]
    }
    else if (buttonThatWasClicked == btnInfoStandardOsc.get())
    {
        //[UserButtonCode_btnInfoStandardOsc] -- add your button handler code here..
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText("If activated, the Plugin listens to the standard OSC patterns, otherwise, the standard patterns are ignored and only user defined patterns are evaluated.\n\nInformation about standard patterns can be found in the main help section.\n");
        textEditor->setSize(330, 250);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfoStandardOsc->getBounds(), this);
        //[/UserButtonCode_btnInfoStandardOsc]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
    //[/UserbuttonClicked_Post]
}

void OSCRxSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderReceiveOscPort.get())
    {
        //[UserSliderCode_sliderReceiveOscPort] -- add your slider handling code here..
        pSettings->oscReceivePort = int(sliderReceiveOscPort->getValue());
        pOscHandler->initialize();
        //[/UserSliderCode_sliderReceiveOscPort]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCRxSettingsComponent::controlDimming()
{
    btnDelete->setEnabled(customOscList->getSelectedRows().size() > 0 && customOscTableModel->getCustomIndex(customOscList->getSelectedRow()) >= 0);
}

void OSCRxSettingsComponent::actionListenerCallback(const String& message)
{
    controlDimming();
    if (message == ACTION_MESSAGE_DATA_CHANGED)
        pOscHandler->initialize();

    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        // for now, nothing to do
    }

    if(message == ACTION_MESSAGE_PRESET_CHANGED)
    {
        // for now, nothing to do
    }
    if(message.startsWith(pCustomOscRxPresetHelper->UniqueActionMessageSelectPreset()))
    {
        CustomOscInput* t = new CustomOscInput();
        String file = message.fromFirstOccurrenceOf(pCustomOscRxPresetHelper->UniqueActionMessageSelectPreset(), false, true);
        pCustomOscRxPresetHelper->loadFromXmlFile(file, t);

        // add to list if not yet existing
        bool existing = false;
        for(auto& e : pSettings->customOscInput)
        {
            if(e->oscString == t->oscString && e->commandString == t->commandString){
                existing = true;
                break;
            }
        }
        if(!existing)
        {
            // always add inactive
            t->enabledFlag = false;
            pSettings->customOscInput.add(t);
            customOscList->updateContent();
        }
    }
    if(message.startsWith(pCustomOscRxPresetHelper->UniqueActionMessageSavePreset()))
    {
        int index = message.getTrailingIntValue();

        if(index < 0 || index >= pSettings->customOscInput.size())
            return;

        pCustomOscRxPresetHelper->tryCreateNewPreset([&,index](File* f){
            if(f != nullptr)
            {
                pCustomOscRxPresetHelper->writeToXmlFile(*f, pSettings->customOscInput[index]);
            }
        });
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCRxSettingsComponent" componentName=""
                 parentClasses="public Component, public ActionListener" constructorParams="EncoderSettings* _pSettings, StatusMessageHandler* _pStatusMessageHandler, CustomOscRxPresetHelper* _pCustomOscRxPresetHelper, OSCLogDialogManager* _pOscLogManager, OSCHandlerEncoder* _pOscHandler"
                 variableInitialisers="pSettings(_pSettings), pStatusMessageHandler(_pStatusMessageHandler), pOscLogManager(_pOscLogManager), pCustomOscRxPresetHelper(_pCustomOscRxPresetHelper), pOscHandler(_pOscHandler)&#10;"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="8 10 150 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="120 10 42 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="sliderReceiveOscPort" id="591bcc850e858bff" memberName="sliderReceiveOscPort"
          virtualName="" explicitFocusOrder="0" pos="170 10 100 24" posRelativeX="64cdd18a28c39177"
          posRelativeY="64cdd18a28c39177" min="0.0" max="65535.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GROUPCOMPONENT name="groupDefinitions" id="5ccced30e0050e9" memberName="groupDefinitions"
                  virtualName="" explicitFocusOrder="0" pos="0 40 0M 44M" title="Definitions"/>
  <GENERICCOMPONENT name="customOscList" id="78a64bf0c7700896" memberName="customOscList"
                    virtualName="" explicitFocusOrder="0" pos="16 24 32M 65M" posRelativeX="5ccced30e0050e9"
                    posRelativeY="5ccced30e0050e9" posRelativeW="5ccced30e0050e9"
                    posRelativeH="5ccced30e0050e9" class="TableListBox" params=""/>
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
  <TEXTBUTTON name="buttonShowOscLog" id="680b48d522ce99b2" memberName="buttonShowOscLog"
              virtualName="" explicitFocusOrder="0" pos="9Rr 10 120 24" buttonText="Show OSC Log"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="toggleHideWarnings" id="a5f38067a572b96c" memberName="toggleHideWarnings"
                virtualName="" explicitFocusOrder="0" pos="130Rr 10 120 24" buttonText="hide warnings"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="btnManagePresets" id="e79fc007bc779712" memberName="btnManagePresets"
              virtualName="" explicitFocusOrder="0" pos="134r 10Rr 86 24" posRelativeX="5ccced30e0050e9"
              posRelativeY="5ccced30e0050e9" buttonText="presets..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="toggleReceiveStandardOsc" id="c6b5fff39ee7a27b" memberName="toggleReceiveStandardOsc"
                virtualName="" explicitFocusOrder="0" pos="280 10 184 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Listen for Standard OSC"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="btnInfo" id="9b390d63268c4528" memberName="btnInfoStandardOsc"
               virtualName="" explicitFocusOrder="0" pos="464 9 23 24" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="help_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="help_png" opacityOver="0.4000000059604645" colourOver="6eee1010"
               resourceDown="help_png" opacityDown="1.0" colourDown="c0ee1010"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]

