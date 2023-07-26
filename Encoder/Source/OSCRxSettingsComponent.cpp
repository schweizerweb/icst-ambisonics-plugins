/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "OSCTargetsComponent.h"
//[/Headers]

#include "OSCRxSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCRxSettingsComponent::OSCRxSettingsComponent (EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, CustomOscRxPresetHelper* pCustomOscRxPresetHelper, OSCLogDialogManager* pOscLogManager, OSCHandlerEncoder* pOscHandler)
    : pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager), pCustomOscRxPresetHelper(pCustomOscRxPresetHelper), pOscHandler(pOscHandler)
{
    //[Constructor_pre] You can add your own custom stuff here..
    customOscTableModel.reset(new CustomOscInputTableListModel(pSettings, pOscHandler, this, this));
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
    labelOscPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
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
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                        juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
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
                                   juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0x00000000),
                                   juce::ImageCache::getFromMemory (help_png, help_pngSize), 0.400f, juce::Colour (0x6eee1010),
                                   juce::ImageCache::getFromMemory (help_png, help_pngSize), 1.000f, juce::Colour (0xc0ee1010));
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
        textEditor->setText("If activated, the Plugin listens to the standard OSC patterns, otherwise, the standard patterns are ignored and only user defined patterns are evaluated.\n\nStandard patterns (details can be found in the main help section):\n /icst/ambi/source/aed\n /icst/ambi/source/xyz\n /icst/ambi/source/gain\n /icst/ambi/group/aed\n /icst/ambi/group/xyz\n /icst/ambi/sourceindex/aed\n /icst/ambi/sourceindex/xyz\n /icst/ambi/sourceindex/gain\n /icst/ambi/group/rotate\n /icst/ambi/group/rotateorigin\n /icst/ambi/group/stretch\n /icst/ambi/distanceencoding/mode\n /icst/ambi/distanceencoding/unitcircle\n /icst/ambi/distanceencoding/dbunit\n /icst/ambi/distanceencoding/distanceattenuation\n /icst/ambi/distanceencoding/centercurve\n /icst/ambi/distanceencoding/advancedfactor\n /icst/ambi/distanceencoding/advancedexponent\n /icst/ambi/distanceencoding/standard\n /icst/ambi/distanceencoding/advanced\n /icst/ambi/distanceencoding/exponential\n /icst/ambi/distanceencoding/inverseproportional");
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
    if(message.startsWith(String(ACTION_MESSAGE_SELECT_PRESET)))
    {
        CustomOscInput* t = new CustomOscInput();
        String file = message.fromFirstOccurrenceOf(String(ACTION_MESSAGE_SELECT_PRESET), false, true);
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
    if(message.startsWith(String(ACTION_MESSAGE_SAVE_PRESET)))
    {
        int index = message.getTrailingIntValue();

        if(index < 0 || index >= pSettings->customOscInput.size())
            return;

        File* f = pCustomOscRxPresetHelper->tryCreateNewPreset();
        if(f != nullptr)
        {
            pCustomOscRxPresetHelper->writeToXmlFile(*f, pSettings->customOscInput[index]);
        }
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
                 parentClasses="public Component, public ActionListener" constructorParams="EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, CustomOscRxPresetHelper* pCustomOscRxPresetHelper, OSCLogDialogManager* pOscLogManager, OSCHandlerEncoder* pOscHandler"
                 variableInitialisers="pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager), pCustomOscRxPresetHelper(pCustomOscRxPresetHelper), pOscHandler(pOscHandler)&#10;"
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

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: help_png, 1009, "../../Resources/help.png"
static const unsigned char resource_OSCRxSettingsComponent_help_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,64,0,0,0,64,8,6,0,0,0,170,105,113,222,0,0,3,184,73,68,65,84,120,94,229,155,
193,117,163,64,12,134,201,190,220,73,7,166,131,56,21,12,91,129,237,10,22,42,88,119,96,114,200,221,91,129,73,5,11,215,92,118,232,128,237,0,119,224,169,192,59,135,255,192,35,75,132,61,154,17,137,245,158,
30,151,60,143,244,161,145,52,163,112,119,62,159,163,91,150,123,171,209,219,219,155,47,0,11,171,169,213,37,52,129,142,73,7,109,161,218,234,145,27,64,211,52,209,203,203,11,0,248,113,122,109,53,131,211,151,
72,2,77,35,8,64,148,86,43,102,24,236,0,148,213,2,198,179,9,32,238,161,26,107,52,92,0,190,49,57,254,7,198,165,145,71,1,92,141,245,148,52,128,216,234,111,194,113,175,32,176,126,44,1,96,133,100,181,142,100,
101,13,59,86,33,1,28,144,144,30,162,121,200,3,236,57,248,6,16,99,239,101,209,60,37,131,125,177,15,0,177,195,94,151,200,13,49,39,0,56,143,154,62,127,89,246,32,176,0,168,28,156,151,132,80,113,52,66,7,198,
176,215,208,214,234,201,106,55,232,236,20,158,9,128,167,142,224,83,216,159,95,11,96,229,154,240,224,232,30,106,168,22,189,247,124,237,181,213,133,131,29,25,34,161,238,3,80,74,145,91,32,70,255,237,34,37,
222,230,179,85,211,131,186,67,182,222,81,221,28,34,36,199,239,180,14,118,196,151,70,64,233,82,231,65,254,149,72,164,41,158,26,13,141,33,64,60,33,164,47,141,134,7,64,216,76,77,130,202,177,195,43,224,252,
112,63,46,25,74,87,14,96,215,116,140,106,42,128,194,193,249,214,234,243,72,2,236,136,172,189,189,32,186,78,215,189,24,26,128,114,204,250,251,17,48,134,114,112,106,104,99,59,84,87,86,5,133,11,145,81,0,
69,228,32,132,97,53,145,200,18,100,125,82,28,18,116,241,81,4,44,24,106,190,113,0,4,8,147,164,117,232,13,22,35,0,130,28,109,53,211,2,198,229,8,61,2,128,229,148,167,200,22,149,71,22,110,37,250,61,128,5,
147,113,251,15,202,89,140,68,232,32,44,32,151,74,169,197,16,64,202,124,18,91,253,39,50,244,132,61,222,77,14,99,55,73,135,157,224,50,200,73,140,118,254,56,49,252,51,6,27,95,17,1,0,32,47,165,123,169,157,
14,96,184,5,18,89,0,56,53,210,114,96,138,214,100,86,0,224,188,33,175,224,17,250,188,0,228,165,35,222,190,178,218,250,234,83,238,103,114,155,107,70,222,122,129,210,233,73,228,35,160,26,153,243,253,176,
218,250,118,94,62,2,222,59,248,136,237,144,70,144,175,12,160,28,212,253,29,113,26,253,114,91,160,26,148,55,56,47,3,160,19,0,80,227,249,83,96,220,214,17,0,4,242,128,48,128,54,48,128,22,207,71,161,38,172,
149,6,112,26,244,228,226,0,180,192,76,95,9,78,155,245,16,192,17,84,66,79,112,51,137,240,111,154,230,56,0,128,186,124,27,82,142,245,1,213,141,0,168,198,0,28,3,229,130,10,153,255,14,154,4,132,175,225,39,
49,24,241,107,192,6,176,251,224,55,129,32,20,84,43,220,128,146,68,211,179,13,0,191,241,59,28,165,229,47,49,243,11,240,246,105,0,77,136,112,148,191,119,0,0,98,4,205,45,43,98,218,227,171,235,204,46,61,14,
27,64,240,17,134,49,81,159,3,93,185,209,247,1,53,97,20,215,228,232,17,255,51,148,122,106,122,106,151,27,161,28,117,58,21,152,28,113,100,253,156,227,70,104,141,115,194,167,145,254,53,58,7,0,99,53,253,68,
16,90,216,107,56,0,12,33,232,153,3,208,132,243,4,0,26,194,247,25,159,26,75,216,103,124,15,70,114,236,175,211,76,0,156,96,79,30,114,50,84,7,61,197,209,39,203,90,98,46,96,172,110,68,114,3,246,58,214,55,
210,131,145,6,123,15,32,252,59,142,245,154,43,191,26,37,0,56,130,64,72,110,25,203,102,139,223,75,8,199,195,70,0,113,172,253,101,245,9,48,178,222,87,159,221,132,129,133,198,223,103,128,249,132,223,99,57,
46,43,165,162,190,220,252,215,227,223,162,27,151,127,227,222,244,125,176,15,252,137,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* OSCRxSettingsComponent::help_png = (const char*) resource_OSCRxSettingsComponent_help_png;
const int OSCRxSettingsComponent::help_pngSize = 1009;

// JUCER_RESOURCE: save_png, 3426, "../../Resources/save.png"
static const unsigned char resource_OSCRxSettingsComponent_save_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,2,0,0,0,2,0,8,3,0,0,0,195,166,36,200,0,0,0,156,80,76,84,69,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,141,76,222,0,0,0,51,116,82,78,83,0,175,62,92,28,80,159,220,56,17,39,251,76,61,101,97,68,226,30,109,7,189,
21,205,3,244,215,197,86,248,231,183,151,133,13,139,33,210,113,235,169,145,122,44,239,128,163,105,50,117,37,112,135,208,45,0,0,12,66,73,68,65,84,120,218,236,218,129,86,162,64,24,134,225,143,77,40,22,67,
89,65,208,208,82,84,210,204,180,255,254,239,109,1,119,79,187,157,74,166,3,145,204,247,92,194,204,59,51,231,204,12,78,176,251,131,181,17,122,219,137,208,185,113,38,211,113,96,173,250,115,124,146,29,91,
99,161,179,231,132,179,43,40,179,221,192,17,106,11,111,182,128,138,189,197,61,191,101,28,227,10,101,245,3,161,22,50,126,162,140,61,167,191,173,156,100,142,83,236,59,30,253,45,118,121,141,143,153,83,161,
54,115,30,35,188,207,191,19,106,59,99,142,247,116,110,132,218,47,28,226,109,15,158,144,14,198,111,23,112,149,10,233,97,249,86,1,251,173,144,46,110,124,188,246,204,245,175,147,29,94,177,249,236,163,151,
39,252,39,186,21,210,202,228,1,255,58,8,105,38,140,240,226,129,111,127,250,89,225,5,47,128,52,180,29,226,175,11,33,13,37,248,195,191,20,210,208,104,136,163,158,144,150,102,56,226,21,128,166,82,31,185,
190,144,166,98,228,118,66,154,50,144,241,71,66,154,154,204,1,116,133,180,101,2,88,11,105,107,13,32,20,210,86,8,248,252,5,174,49,199,199,94,72,99,123,196,66,26,139,249,19,64,111,7,36,66,26,75,96,9,105,
204,194,189,144,198,238,97,8,105,204,96,0,122,251,76,0,75,235,209,141,187,95,234,48,149,18,54,221,174,105,154,113,225,34,215,235,245,92,215,125,202,12,6,131,85,230,112,56,108,10,143,185,89,33,201,172,
11,119,71,86,110,183,219,221,23,140,163,219,92,112,116,115,20,22,150,203,229,56,119,89,240,60,111,154,75,211,116,155,25,141,70,147,140,227,56,82,137,233,166,251,174,184,183,177,66,167,246,0,150,238,16,
13,232,164,114,90,31,223,90,20,69,126,102,158,177,51,195,66,167,211,249,181,200,60,151,73,60,93,224,4,219,13,107,13,192,51,209,144,217,249,7,240,49,79,78,155,161,4,211,171,47,128,91,27,77,233,49,0,17,
23,101,216,65,93,1,4,17,202,99,0,77,5,0,63,168,39,128,177,13,21,12,160,169,0,96,79,213,3,248,238,227,203,0,20,2,64,92,71,0,1,212,48,128,230,2,136,198,53,4,96,66,13,3,104,46,0,172,170,15,96,228,67,13,3,
104,48,128,133,66,0,103,113,2,48,0,181,0,224,85,30,64,2,69,12,160,201,0,140,202,3,112,161,134,1,52,26,64,82,121,0,93,40,98,0,77,6,176,170,60,128,31,80,196,0,154,12,192,173,60,128,107,168,97,0,141,6,112,
193,29,224,204,48,0,6,192,0,24,0,3,96,0,12,128,1,48,0,6,240,155,93,58,32,1,0,0,0,16,244,255,117,59,2,221,160,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,
0,2,8,32,128,0,2,8,32,128,0,95,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,192,151,0,2,8,32,128,
0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,128,0,2,8,32,64,236,218,235,82,226,64,16,134,225,94,206,17,4,93,2,36,106,100,5,2,106,0,129,190,255,123,219,137,214,110,237,89,
171,224,199,134,239,125,47,97,234,169,154,234,158,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,213,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,111,245,172,194,1,224,120,0,125,171,112,107,0,28,13,224,201,170,219,204,1,112,52,128,137,85,183,29,0,142,7,224,133,85,182,33,0,78,0,96,107,149,237,22,0,239,2,56,231,57,160,227,0,56,5,128,180,
162,171,128,120,2,128,147,0,240,238,83,195,170,87,252,201,1,112,18,0,161,108,186,239,21,139,241,120,28,69,209,108,54,139,227,56,73,236,255,238,234,198,1,240,62,128,163,234,118,179,44,75,211,245,122,189,
90,221,135,174,175,219,161,155,80,158,79,66,183,101,163,178,105,232,83,217,99,104,187,109,133,190,148,61,133,230,243,249,67,104,19,122,14,237,247,203,229,114,56,124,121,121,233,247,7,131,65,189,94,191,
11,53,155,205,94,175,119,85,86,171,213,46,59,161,93,168,40,138,139,139,195,97,177,248,220,104,52,198,161,87,159,141,162,57,191,113,7,192,7,0,16,0,212,3,128,120,202,0,234,78,62,16,6,112,231,228,117,97,
0,61,39,111,10,3,232,56,249,165,48,128,134,147,47,132,1,88,234,242,101,137,50,128,137,203,151,155,50,128,150,203,215,146,6,48,116,249,234,210,0,118,174,94,119,44,13,32,185,118,241,166,38,13,192,150,46,
94,83,28,64,124,239,210,165,177,56,0,123,118,233,182,166,14,96,182,114,225,186,133,60,0,237,73,240,193,0,144,228,46,91,30,3,192,172,211,117,209,178,194,0,16,218,184,104,67,3,64,89,50,114,201,182,6,128,
183,162,182,11,54,138,1,240,173,133,224,58,104,18,25,0,190,119,33,183,13,200,199,6,128,31,218,137,253,13,186,141,12,0,63,117,41,37,160,21,27,0,126,169,166,115,11,164,125,51,0,252,214,98,234,26,141,14,
6,128,63,86,108,214,126,246,181,235,102,0,248,91,209,243,153,15,132,249,32,54,0,252,171,248,110,154,249,153,182,106,149,231,13,128,247,154,53,55,211,246,153,189,16,165,249,227,190,147,152,1,224,163,37,
141,162,115,46,237,14,145,189,6,0,2,0,1,128,0,64,0,32,0,16,0,8,0,4,0,2,0,1,128,0,64,0,32,0,16,0,8,0,4,0,2,0,1,128,0,64,0,32,0,168,7,0,241,0,32,30,0,196,3,128,120,0,16,15,0,226,1,64,60,0,136,7,0,241,0,
32,30,0,196,3,128,120,0,16,15,0,226,1,64,60,0,136,7,0,241,0,32,30,0,196,3,128,120,0,16,15,0,226,1,64,188,175,236,220,205,78,90,81,20,134,97,171,8,42,212,208,134,162,104,165,80,136,63,165,149,134,125,255,
247,214,116,102,26,61,164,147,195,113,127,207,123,5,123,240,236,193,90,131,5,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,
120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,0,132,7,64,120,239,27,192,231,211,251,213,162,175,197,211,114,59,158,166,1,216,
172,6,69,47,186,89,174,131,0,244,30,139,254,109,216,239,165,0,24,253,44,122,165,135,113,6,128,211,97,209,171,13,191,38,0,216,204,138,222,232,238,172,126,0,215,87,69,111,118,51,169,30,192,73,81,67,219,
234,1,220,22,53,52,168,29,192,174,168,177,179,202,1,108,139,26,251,85,57,128,231,162,198,190,85,14,192,12,176,167,121,229,0,230,69,205,77,234,6,96,11,184,175,105,213,0,38,69,123,90,87,13,224,188,104,79,
59,0,178,187,0,32,59,0,194,3,32,60,0,194,3,32,60,0,194,3,32,60,0,194,3,32,60,0,194,3,32,60,0,194,3,32,60,0,194,3,224,105,211,171,185,223,11,0,26,235,31,213,94,31,128,166,198,71,181,55,6,224,111,7,191,
82,114,176,142,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,168,61,0,0,0,0,0,0,0,0,0,0,0,0,0,32,13,192,71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,106,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,248,255,55,172,127,124,153,223,149,131,55,27,60,94,158,3,208,58,128,139,15,165,59,205,86,223,1,104,23,192,125,7,254,254,203,230,199,0,180,8,224,122,81,186,214,240,4,128,246,0,172,74,
247,26,142,0,104,11,192,168,116,177,135,30,0,237,0,152,124,42,157,236,25,128,118,0,92,150,110,54,220,1,208,10,128,219,210,209,150,0,180,1,96,90,186,218,21,0,109,0,24,151,174,246,135,189,187,93,74,27,8,
195,48,188,20,40,42,214,136,9,24,4,7,65,192,47,170,213,247,252,207,173,248,195,81,112,170,97,146,110,158,93,238,251,8,50,206,53,206,38,251,46,219,205,0,80,6,64,224,75,128,117,115,0,120,0,176,52,217,30,
0,192,127,0,0,176,6,0,192,255,125,11,232,154,104,188,5,236,249,119,128,223,0,240,2,32,49,205,186,115,0,120,1,144,117,76,178,159,236,5,236,247,110,224,0,0,158,0,184,169,233,213,61,101,30,192,27,0,119,103,
106,93,142,153,8,242,8,192,165,98,51,129,189,95,204,4,122,5,224,94,110,133,62,7,12,167,76,5,251,63,23,112,246,60,233,12,187,181,183,90,60,38,51,206,5,112,50,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,32,240,12,181,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,177,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,177,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,160,220,51,
204,198,87,253,139,78,237,45,38,211,211,28,0,222,1,12,174,135,38,83,47,205,1,224,23,64,251,198,164,90,28,1,192,39,128,123,83,107,245,3,0,254,0,164,166,215,240,15,0,124,1,56,16,250,161,240,247,58,51,0,
248,1,144,47,76,178,59,0,248,1,48,54,205,46,91,0,240,2,160,111,162,61,3,192,7,128,153,228,10,224,181,81,184,0,142,221,102,202,0,116,239,14,190,204,131,5,240,211,109,166,12,64,248,246,240,179,96,1,216,
147,219,72,25,192,210,100,123,8,23,128,165,238,99,202,0,158,76,182,121,192,0,54,5,40,3,208,188,56,246,181,110,22,50,128,13,1,202,0,90,166,218,69,184,111,1,219,2,148,1,56,209,15,129,102,215,129,3,248,32,
64,26,128,236,42,240,33,116,0,239,2,164,1,204,196,102,1,222,106,4,188,23,176,45,64,26,128,75,76,177,225,75,4,0,222,4,104,3,112,143,38,88,18,242,60,192,182,0,113,0,89,195,228,122,14,122,34,104,91,128,56,
0,151,221,153,86,195,102,216,51,129,159,4,136,3,112,238,244,220,116,234,222,206,3,159,10,254,36,64,30,192,154,192,213,133,196,198,240,112,52,61,10,254,92,192,39,1,1,0,88,151,13,90,181,119,152,199,112,
50,104,171,52,16,0,129,21,14,0,75,1,224,220,62,3,176,20,0,110,175,1,88,10,128,253,6,96,41,0,34,0,48,94,137,8,0,64,61,0,92,107,162,33,0,0,245,0,88,151,172,20,4,0,160,54,0,238,172,35,32,0,0,245,0,144,17,
0,128,122,0,200,8,0,64,61,0,100,4,0,160,30,0,50,2,0,80,15,0,25,1,0,168,7,128,140,0,0,212,3,64,70,0,0,106,7,80,74,0,0,34,0,80,74,0,0,34,0,80,74,0,0,34,0,80,74,0,0,34,0,80,74,0,0,34,0,80,74,0,0,34,0,80,
74,0,0,34,0,80,74,0,0,52,0,156,20,6,32,35,0,0,85,2,88,22,6,32,35,0,0,85,2,184,46,12,64,70,0,0,170,4,208,40,12,64,70,0,0,170,4,208,43,12,64,70,0,0,42,4,240,98,133,1,200,8,0,64,133,0,150,197,1,200,8,0,64,
117,0,242,78,113,0,50,2,0,80,29,128,166,21,7,32,35,0,0,149,1,24,220,236,0,64,70,0,0,170,2,144,245,109,7,0,50,2,6,0,168,6,192,97,223,118,1,32,35,224,192,190,173,237,98,175,93,254,218,158,227,158,149,0,
80,169,128,165,219,161,108,98,223,214,207,93,220,229,95,255,17,70,153,251,166,195,228,220,172,4,128,138,5,220,54,219,69,59,41,244,224,163,164,29,115,205,190,125,221,226,169,253,207,198,201,253,213,121,
215,172,18,0,219,2,40,214,26,14,1,127,219,165,219,148,4,194,0,140,162,55,136,193,154,34,163,151,152,25,107,102,34,21,17,191,247,191,56,65,16,196,31,110,224,62,103,13,71,109,69,6,168,237,200,0,181,145,
12,80,251,35,3,212,246,100,128,218,27,25,160,54,39,3,204,54,144,1,102,3,25,160,182,38,3,212,38,100,128,89,223,144,1,102,35,100,128,217,132,12,48,91,64,6,152,125,144,1,102,159,133,12,48,219,67,6,136,117,
133,12,48,155,66,6,136,237,32,3,196,250,23,50,64,172,158,66,6,136,29,33,3,196,42,200,0,177,167,66,6,136,13,5,50,192,107,85,32,3,188,254,27,200,0,173,246,7,200,0,173,217,9,50,64,171,93,54,64,6,72,213,213,
51,23,25,96,244,91,189,114,149,1,58,221,250,155,27,25,96,82,207,182,7,238,100,128,64,221,246,221,48,126,189,23,30,56,3,98,145,206,31,175,11,21,82,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* OSCRxSettingsComponent::save_png = (const char*) resource_OSCRxSettingsComponent_save_png;
const int OSCRxSettingsComponent::save_pngSize = 3426;


//[EndFile] You can add extra defines here...
//[/EndFile]

