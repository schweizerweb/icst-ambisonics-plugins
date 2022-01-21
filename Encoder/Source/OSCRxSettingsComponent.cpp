/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.4

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
OSCRxSettingsComponent::OSCRxSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, OSCLogDialogManager* pOscLogManager)
    : pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager)
{
    //[Constructor_pre] You can add your own custom stuff here..
    customOscTableModel.reset(new CustomOscInputTableListModel(pSettings, this, this));
    addChangeListener(pChangeListener);
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

    labelOscPort->setBounds (121, 10, 49, 24);

    sliderReceiveOscPort.reset (new juce::Slider ("sliderReceiveOscPort"));
    addAndMakeVisible (sliderReceiveOscPort.get());
    sliderReceiveOscPort->setRange (0, 65535, 1);
    sliderReceiveOscPort->setSliderStyle (juce::Slider::IncDecButtons);
    sliderReceiveOscPort->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sliderReceiveOscPort->addListener (this);

    sliderReceiveOscPort->setBounds (177, 10, 100, 24);

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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    sliderReceiveOscPort->setValue(pSettings->oscReceivePort, dontSendNotification);
    customOscTableModel->initTable(customOscList.get());

    controlDimming();
    //[/Constructor]
}

OSCRxSettingsComponent::~OSCRxSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
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
    buttonShowOscLog->setBounds (getWidth() - 9 - 150, 10, 150, 24);
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
        sendChangeMessage();
        //[/UserButtonCode_toggleReceiveOsc]
    }
    else if (buttonThatWasClicked == btnAdd.get())
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
        pSettings->customOscInput.add(new CustomOscInput());
        customOscList->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
        int row = customOscTableModel->getCustomIndex(customOscList->getSelectedRow());
        if (row >= 0 && row < pSettings->customOscInput.size())
            pSettings->customOscTargets.remove(row);
        customOscList->updateContent();
        //[/UserButtonCode_btnDelete]
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        //[UserButtonCode_btnInfo] -- add your button handler code here..
        auto label = std::make_unique<Label>("", String(COMMON_OSC_INFO_STRING) + String("\nNote: constant values have to match, otherwise entire message is ignored.\n\n{} = Ignore this argument"));
        label->setSize(370, 400);
        label->setJustificationType(Justification::left);
        CallOutBox::launchAsynchronously(std::move(label), btnInfo->getBounds(), this);
        //[/UserButtonCode_btnInfo]
    }
    else if (buttonThatWasClicked == buttonShowOscLog.get())
    {
        //[UserButtonCode_buttonShowOscLog] -- add your button handler code here..
        pOscLogManager->show(pStatusMessageHandler, this);
        //[/UserButtonCode_buttonShowOscLog]
    }

    //[UserbuttonClicked_Post]
    sendChangeMessage();
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
        //[/UserSliderCode_sliderReceiveOscPort]
    }

    //[UsersliderValueChanged_Post]
    sendChangeMessage();
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
        sendChangeMessage();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCRxSettingsComponent" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster, public ActionListener"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, StatusMessageHandler* pStatusMessageHandler, OSCLogDialogManager* pOscLogManager"
                 variableInitialisers="pSettings(pSettings), pStatusMessageHandler(pStatusMessageHandler), pOscLogManager(pOscLogManager)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="toggleReceiveOsc" id="8d9b70b5bf27a026" memberName="toggleReceiveOsc"
                virtualName="" explicitFocusOrder="0" pos="8 10 150 24" posRelativeX="64cdd18a28c39177"
                posRelativeY="64cdd18a28c39177" buttonText="Receive OSC" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelOscPort" id="646c42f30e7e37d7" memberName="labelOscPort"
         virtualName="" explicitFocusOrder="0" pos="121 10 49 24" posRelativeX="64cdd18a28c39177"
         posRelativeY="64cdd18a28c39177" edTextCol="ff000000" edBkgCol="0"
         labelText="Port:&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="sliderReceiveOscPort" id="591bcc850e858bff" memberName="sliderReceiveOscPort"
          virtualName="" explicitFocusOrder="0" pos="177 10 100 24" posRelativeX="64cdd18a28c39177"
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
              virtualName="" explicitFocusOrder="0" pos="9Rr 10 150 24" buttonText="Show OSC Log"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
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


//[EndFile] You can add extra defines here...
//[/EndFile]
