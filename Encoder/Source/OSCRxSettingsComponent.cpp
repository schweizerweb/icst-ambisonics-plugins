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

#include "OSCTargetsComponent.h"
#include "OSCRxSettingsComponent.h"

OSCRxSettingsComponent::OSCRxSettingsComponent (EncoderSettings* _pSettings, StatusMessageHandler* _pStatusMessageHandler, CustomOscRxPresetHelper* _pCustomOscRxPresetHelper, OSCLogDialogManager* _pOscLogManager, OSCHandlerEncoder* _pOscHandler)
    : pSettings(_pSettings), pStatusMessageHandler(_pStatusMessageHandler), pOscLogManager(_pOscLogManager), pCustomOscRxPresetHelper(_pCustomOscRxPresetHelper), pOscHandler(_pOscHandler)
{
    customOscTableModel.reset(new CustomOscInputTableListModel(pSettings, pOscHandler, this, this, pCustomOscRxPresetHelper));
    pCustomOscRxPresetHelper->addActionListener(this);

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

    setSize (600, 400);

    toggleReceiveOsc->setToggleState(pSettings->oscReceiveFlag, dontSendNotification);
    sliderReceiveOscPort->setValue(pSettings->oscReceivePort, dontSendNotification);
    toggleReceiveStandardOsc->setToggleState(pSettings->oscHandleStandardFormatFlag, dontSendNotification);
    customOscTableModel->initTable(customOscList.get());
    toggleHideWarnings->setToggleState(pSettings->hideWarnings, dontSendNotification);
    controlDimming();
}

OSCRxSettingsComponent::~OSCRxSettingsComponent()
{
    pCustomOscRxPresetHelper->removeActionListener(this);

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

    customOscTableModel = nullptr;
}

void OSCRxSettingsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void OSCRxSettingsComponent::resized()
{
    groupDefinitions->setBounds (0, 40, getWidth() - 0, getHeight() - 44);
    customOscList->setBounds (0 + 16, 40 + 24, (getWidth() - 0) - 32, (getHeight() - 44) - 65);
    btnAdd->setBounds (0 + (getWidth() - 0) - 16 - 70, 40 + (getHeight() - 44) - 10 - 24, 70, 24);
    btnDelete->setBounds (0 + (getWidth() - 0) - 92 - 70, 40 + (getHeight() - 44) - 10 - 24, 70, 24);
    btnInfo->setBounds (0 + 16, 40 + (getHeight() - 44) - 10 - 24, 23, 24);
    buttonShowOscLog->setBounds (getWidth() - 9 - 120, 10, 120, 24);
    toggleHideWarnings->setBounds (getWidth() - 130 - 120, 10, 120, 24);
    btnManagePresets->setBounds (0 + 134 - 86, 40 + (getHeight() - 44) - 10 - 24, 86, 24);
}

void OSCRxSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggleReceiveOsc.get())
    {
        pSettings->oscReceiveFlag = toggleReceiveOsc->getToggleState();
        pOscHandler->initialize();
    }
    else if (buttonThatWasClicked == btnAdd.get())
    {
        pSettings->customOscInput.add(new CustomOscInput());
        pOscHandler->initialize();
        customOscList->updateContent();
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        int row = customOscTableModel->getCustomIndex(customOscList->getSelectedRow());
        if (row >= 0 && row < pSettings->customOscInput.size())
            pSettings->customOscInput.remove(row);
        pOscHandler->initialize();
        customOscList->updateContent();
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText(String(COMMON_OSC_INFO_STRING) + String("\nNote: constant values have to match, otherwise the entire message is ignored.\n\n{} = Require, but ignore this argument\n\nIf 'JS-Code' is not empty, JavaScript mode is used:\n- 'OSC-Message' defines the pattern to match incoming messages\n- 'JS-Code' defines the JavaScript code to be executed\n\nExample:\nOSC-Message: /path/to/source/*/xy\nJS-Code: s.setXYZ(s.path(4), s.arg(1), s.arg(2), s.arg(3))\nFor more information, see help in Code Editor."));
        textEditor->setSize(420, 520);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfo->getBounds(), this);
    }
    else if (buttonThatWasClicked == buttonShowOscLog.get())
    {
        pOscLogManager->show(pStatusMessageHandler, this);
    }
    else if (buttonThatWasClicked == toggleHideWarnings.get())
    {
        pSettings->hideWarnings = toggleHideWarnings->getToggleState();
        pOscHandler->initialize();
    }
    else if (buttonThatWasClicked == btnManagePresets.get())
    {
        presetManagerDialog.show(this, pCustomOscRxPresetHelper);
    }
    else if (buttonThatWasClicked == toggleReceiveStandardOsc.get())
    {
        pSettings->oscHandleStandardFormatFlag = toggleReceiveStandardOsc->getToggleState();
        pOscHandler->initialize();
    }
    else if (buttonThatWasClicked == btnInfoStandardOsc.get())
    {
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText("If activated, the Plugin listens to the standard OSC patterns, otherwise, the standard patterns are ignored and only user defined patterns are evaluated.\n\nInformation about standard patterns can be found in the main help section.\n");
        textEditor->setSize(330, 250);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfoStandardOsc->getBounds(), this);
    }

    controlDimming();
}

void OSCRxSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderReceiveOscPort.get())
    {
        pSettings->oscReceivePort = int(sliderReceiveOscPort->getValue());
        pOscHandler->initialize();
    }
}

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
