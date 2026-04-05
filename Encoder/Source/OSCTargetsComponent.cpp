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

OSCTargetsComponent::OSCTargetsComponent (ChangeListener* pChangeListener, EncoderSettings* _pSettings, CustomOscTxPresetHelper* _pCustomOscTxPresetHelper)
    : pSettings(_pSettings), pCustomOscTxPresetHelper(_pCustomOscTxPresetHelper)
{
    customOscTableModel.reset(new CustomOscTableListModel(pSettings, this, this, pCustomOscTxPresetHelper));
    addChangeListener(pChangeListener);
    pCustomOscTxPresetHelper->addActionListener(this);

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

    setSize (600, 400);

    sliderInterval->setSkewFactorFromMidPoint(200.0);
    sliderInterval->setValue(pSettings->oscSendExtIntervalMs, dontSendNotification);
    toggleSendContinuous->setToggleState(pSettings->oscSendExtContinuousFlag, dontSendNotification);

    customOscTableModel->initTable(targetList.get());

    controlDimming();
}

OSCTargetsComponent::~OSCTargetsComponent()
{
    pCustomOscTxPresetHelper->removeActionListener(this);

    groupTargets = nullptr;
    targetList = nullptr;
    sliderInterval = nullptr;
    labelInterval = nullptr;
    btnAdd = nullptr;
    btnDelete = nullptr;
    btnInfo = nullptr;
    toggleSendContinuous = nullptr;
    btnManagePresets = nullptr;

    customOscTableModel = nullptr;
}

void OSCTargetsComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void OSCTargetsComponent::resized()
{
    groupTargets->setBounds (0, 40, getWidth() - 0, getHeight() - 40);
    targetList->setBounds (0 + 16, 40 + 24, (getWidth() - 0) - 32, (getHeight() - 40) - 65);
    sliderInterval->setBounds (getWidth() - 8 - 210, 8, 210, 24);
    labelInterval->setBounds (getWidth() - 334, 8, 104, 24);
    btnAdd->setBounds (0 + (getWidth() - 0) - 16 - 70, 40 + (getHeight() - 40) - 10 - 24, 70, 24);
    btnDelete->setBounds (0 + (getWidth() - 0) - 92 - 70, 40 + (getHeight() - 40) - 10 - 24, 70, 24);
    btnInfo->setBounds (0 + 16, 40 + (getHeight() - 40) - 10 - 24, 23, 24);
    btnManagePresets->setBounds (0 + 134 - 86, 40 + (getHeight() - 40) - 10 - 24, 86, 24);
}

void OSCTargetsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderInterval.get())
    {
        pSettings->oscSendExtIntervalMs = int(sliderInterval->getValue());
    }

    sendChangeMessage();
}

void OSCTargetsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == btnAdd.get())
    {
        pSettings->customOscTargets.add(new CustomOscTarget());
        targetList->updateContent();
    }
    else if (buttonThatWasClicked == btnDelete.get())
    {
        int row = customOscTableModel->getCustomTargetIndex(targetList->getSelectedRow());
        if (row >= 0 && row < pSettings->customOscTargets.size())
            pSettings->customOscTargets.remove(row);
        targetList->updateContent();
    }
    else if (buttonThatWasClicked == btnInfo.get())
    {
        auto textEditor = std::make_unique<TextEditor>("");
        textEditor->setReadOnly(true);
        textEditor->setMultiLine(true);
        textEditor->setText(COMMON_OSC_INFO_STRING + String("\n\nJavaScript:\nEvery object can also be dynamically calculated using a JavaScript expression:\n{expr, #}\nwhere # defines the expression.\n\n The object p represents the current source point, the following methods provide access to the point's information:\n- p.getX()\n- p.getY()\n- p.getZ()\n- p.getA()\n- p.getE()\n- p.getD()\n- p.getName()\n- p.getColor()\n- p.getGain()\n- p.getIndex()\n\nExample to calculate the distance from (0,0,0) of the projection to the XY-plane:\n{expr, Math.sqrt(Math.pow(p.getX(), 2) + Math.pow(p.getY(), 2))}"));
        textEditor->setSize(390, 250);
        CallOutBox::launchAsynchronously(std::move(textEditor), btnInfo->getBounds(), this);
    }
    else if (buttonThatWasClicked == toggleSendContinuous.get())
    {
        pSettings->oscSendExtContinuousFlag = toggleSendContinuous->getToggleState();
    }
    else if (buttonThatWasClicked == btnManagePresets.get())
    {
        presetManagerDialog.show(this, pCustomOscTxPresetHelper);
    }

    sendChangeMessage();
    controlDimming();
}

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
