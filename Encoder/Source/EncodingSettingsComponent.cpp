/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "DistanceEncodingComponent.h"
//[/Headers]

#include "EncodingSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EncodingSettingsComponent::EncodingSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams)
    : pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    comboBoxPresets.reset (new ComboBox ("comboBoxPresets"));
    addAndMakeVisible (comboBoxPresets.get());
    comboBoxPresets->setEditableText (false);
    comboBoxPresets->setJustificationType (Justification::centredLeft);
    comboBoxPresets->setTextWhenNothingSelected (TRANS("-"));
    comboBoxPresets->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxPresets->addListener (this);

    labelPresets.reset (new Label ("labelPresets",
                                   TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (TextEditor::textColourId, Colours::black);
    labelPresets->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    buttonImport.reset (new TextButton ("buttonImport"));
    addAndMakeVisible (buttonImport.get());
    buttonImport->setButtonText (TRANS("import"));
    buttonImport->addListener (this);

    buttonSave.reset (new TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    buttonExport.reset (new TextButton ("buttonExport"));
    addAndMakeVisible (buttonExport.get());
    buttonExport->setButtonText (TRANS("export"));
    buttonExport->addListener (this);

    sourceDefinition.reset (new SourceDefinitionComponent (pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams));
    addAndMakeVisible (sourceDefinition.get());
    sourceDefinition->setName ("sourceDefinition");

    toggleDistanceEncoding.reset (new ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable Distance Encoding"));
    toggleDistanceEncoding->addListener (this);

    toggleDistanceEncoding->setBounds (14, 19, 199, 24);

    toggleDirectionFlip.reset (new ToggleButton ("toggleDirectionFlip"));
    addAndMakeVisible (toggleDirectionFlip.get());
    toggleDirectionFlip->setButtonText (TRANS("Flip Direction (Orientation)"));
    toggleDirectionFlip->addListener (this);

    toggleDirectionFlip->setBounds (14, 79, 207, 24);

    toggleDoppler.reset (new ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable Doppler"));
    toggleDoppler->addListener (this);

    toggleDoppler->setBounds (14, 49, 199, 24);

    sliderDistanceScaler.reset (new Slider ("sliderDistanceScaler"));
    addAndMakeVisible (sliderDistanceScaler.get());
    sliderDistanceScaler->setRange (1, 1000, 0);
    sliderDistanceScaler->setSliderStyle (Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

    labelDistanceScaler.reset (new Label ("labelDistanceScaler",
                                          TRANS("Distance Scaler:")));
    addAndMakeVisible (labelDistanceScaler.get());
    labelDistanceScaler->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (Justification::centredRight);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (TextEditor::textColourId, Colours::black);
    labelDistanceScaler->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    btnEditDistanceEncoding.reset (new TextButton ("btnEditDistanceEncoding"));
    addAndMakeVisible (btnEditDistanceEncoding.get());
    btnEditDistanceEncoding->setButtonText (TRANS("edit..."));
    btnEditDistanceEncoding->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    toggleDistanceEncoding->setToggleState(pEncoderSettings->distanceEncodingFlag, dontSendNotification);
    toggleDirectionFlip->setToggleState(pEncoderSettings->getDirectionFlip(), dontSendNotification);

    toggleDoppler->setToggleState(pEncoderSettings->dopplerEncodingFlag, dontSendNotification);
    sliderDistanceScaler->setValue(pEncoderSettings->getDistanceScaler());

        labelPresets->setVisible(MULTI_ENCODER_MODE);
    comboBoxPresets->setVisible(MULTI_ENCODER_MODE);
    buttonSave->setVisible(MULTI_ENCODER_MODE);
    buttonImport->setVisible(MULTI_ENCODER_MODE);
    buttonExport->setVisible(MULTI_ENCODER_MODE);

    // load stored presets
    initializePresets();
    controlDimming();

    // TODO: Doppler temporarily deactivated
    toggleDoppler->setToggleState(false, dontSendNotification);
    toggleDoppler->setEnabled(false);
    labelDistanceScaler->setEnabled(false);
    sliderDistanceScaler->setEnabled(false);

    //[/Constructor]
}

EncodingSettingsComponent::~EncodingSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonImport = nullptr;
    buttonSave = nullptr;
    buttonExport = nullptr;
    sourceDefinition = nullptr;
    toggleDistanceEncoding = nullptr;
    toggleDirectionFlip = nullptr;
    toggleDoppler = nullptr;
    sliderDistanceScaler = nullptr;
    labelDistanceScaler = nullptr;
    btnEditDistanceEncoding = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EncodingSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EncodingSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxPresets->setBounds (83, getHeight() - 8 - 24, getWidth() - 312, 24);
    labelPresets->setBounds (8, getHeight() - 8 - 24, 64, 24);
    buttonImport->setBounds (getWidth() - 81 - 64, getHeight() - 8 - 24, 64, 24);
    buttonSave->setBounds (getWidth() - 153 - 64, getHeight() - 8 - 24, 64, 24);
    buttonExport->setBounds (getWidth() - 9 - 64, getHeight() - 8 - 24, 64, 24);
    sourceDefinition->setBounds (8, 112, getWidth() - 16, getHeight() - 154);
    sliderDistanceScaler->setBounds (getWidth() - 24 - 202, 49, 202, 24);
    labelDistanceScaler->setBounds (getWidth() - 229 - 109, 49, 109, 24);
    btnEditDistanceEncoding->setBounds (getWidth() - 24 - 86, 19, 86, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EncodingSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxPresets.get())
    {
        //[UserComboBoxCode_comboBoxPresets] -- add your combo box handling code here..
        String presetToLoad = comboBoxPresets->getText();
        for (File preset : presets)
        {
            if (preset.getFileNameWithoutExtension() == presetToLoad)
            {
                pSources->loadFromXmlFile(preset, pAudioParams);
                sourceDefinition->refresh();
            }
        }

        //[/UserComboBoxCode_comboBoxPresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void EncodingSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonImport.get())
    {
        //[UserButtonCode_buttonImport] -- add your button handler code here..
        FileChooser chooser("Select preset XML to import...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
        if (chooser.browseForFileToOpen())
        {
            File importFile(chooser.getResult());
            AmbiSourceSet testSet;
            if(testSet.loadFromXmlFile(importFile))
            {
                importFile.copyFileTo(presetDirectory.getFullPathName() + "/" + importFile.getFileName());
                initializePresets();
                comboBoxPresets->setText(importFile.getFileNameWithoutExtension(), sendNotificationAsync);
            }
            else
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset import", "Error loading preset " + importFile.getFullPathName());
            }
        }

        //[/UserButtonCode_buttonImport]
    }
    else if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        AlertWindow alert("Save Preset", "", AlertWindow::NoIcon);
        Array<String> existingPresets;
        existingPresets.add("");
        for (File preset : presets)
            existingPresets.add(preset.getFileNameWithoutExtension());

        alert.addComboBox("existing", existingPresets, "Overwrite existing");
        alert.addTextEditor("text", "", "Or enter new name", false);
        alert.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
        alert.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));

        int returnValue = alert.runModalLoop();
        if(returnValue == 1)
        {
            String presetName = alert.getTextEditorContents("text");
            if(presetName.isEmpty())
                presetName = alert.getComboBoxComponent("existing")->getText();

            if (presetName.isEmpty() || File::createLegalFileName(presetName) != presetName)
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid preset name: " + presetName);
                return;
            }

            File newFile(presetDirectory.getFullPathName() + "/" + presetName + ".xml");

            if (newFile.existsAsFile())
            {
                AlertWindow confirm("Overwrite?", "Are you sure to overwrite preset \"" + presetName + "\"?", AlertWindow::QuestionIcon);
                confirm.addButton("No", 0, KeyPress(KeyPress::escapeKey, 0, 0));
                confirm.addButton("Yes", 1, KeyPress(KeyPress::returnKey, 0, 0));
                if (confirm.runModalLoop() == 0)
                {
                    return;
                }
            }

            pSources->writeToXmlFile(newFile);
            initializePresets();
            comboBoxPresets->setText(presetName, dontSendNotification);
        }

        //[/UserButtonCode_buttonSave]
    }
    else if (buttonThatWasClicked == buttonExport.get())
    {
        //[UserButtonCode_buttonExport] -- add your button handler code here..
        FileChooser chooser("Select filename to export...", File::getSpecialLocation(File::userHomeDirectory), "*.xml");
        if (chooser.browseForFileToSave(true))
        {
            File exportFile(chooser.getResult());
            if(!pSources->writeToXmlFile(exportFile))
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Preset export", "Error exporting preset to " + exportFile.getFullPathName());
            }
        }

        //[/UserButtonCode_buttonExport]
    }
    else if (buttonThatWasClicked == toggleDistanceEncoding.get())
    {
        //[UserButtonCode_toggleDistanceEncoding] -- add your button handler code here..
        pEncoderSettings->distanceEncodingFlag = toggleDistanceEncoding->getToggleState();
        sendChangeMessage();
        //[/UserButtonCode_toggleDistanceEncoding]
    }
    else if (buttonThatWasClicked == toggleDirectionFlip.get())
    {
        //[UserButtonCode_toggleDirectionFlip] -- add your button handler code here..
        pEncoderSettings->setDirectionFlip(toggleDirectionFlip->getToggleState());
        sendChangeMessage();

        //[/UserButtonCode_toggleDirectionFlip]
    }
    else if (buttonThatWasClicked == toggleDoppler.get())
    {
        //[UserButtonCode_toggleDoppler] -- add your button handler code here..
        pEncoderSettings->dopplerEncodingFlag = toggleDoppler->getToggleState();
        sendChangeMessage();

        //[/UserButtonCode_toggleDoppler]
    }
    else if (buttonThatWasClicked == btnEditDistanceEncoding.get())
    {
        //[UserButtonCode_btnEditDistanceEncoding] -- add your button handler code here..
        CallOutBox::launchAsynchronously(new DistanceEncodingComponent(&pEncoderSettings->distanceEncodingParams), getScreenBounds(), nullptr);

        //[/UserButtonCode_btnEditDistanceEncoding]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
    //[/UserbuttonClicked_Post]
}

void EncodingSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler.get())
    {
        //[UserSliderCode_sliderDistanceScaler] -- add your slider handling code here..
        pEncoderSettings->setDistanceScaler(sliderDistanceScaler->getValue());
        sendChangeMessage();

        //[/UserSliderCode_sliderDistanceScaler]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EncodingSettingsComponent::controlDimming() const
{
    btnEditDistanceEncoding->setEnabled(pEncoderSettings->distanceEncodingFlag);
}

void EncodingSettingsComponent::initializePresets()
{
    presetDirectory = File(File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() + "/ICST AmbiEncoder");
    if (!presetDirectory.exists())
        presetDirectory.createDirectory();

    presets.clear();
    comboBoxPresets->clear();

    int id = 1;
    DirectoryIterator iterator(presetDirectory, false, "*.xml");
    while (iterator.next())
    {
        // try to load preset
        AmbiSourceSet test;
        if (test.loadFromXmlFile(iterator.getFile()))
        {
            String name = iterator.getFile().getFileNameWithoutExtension();
            presets.add(iterator.getFile());
            comboBoxPresets->addItem(name, id++);
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

<JUCER_COMPONENT documentType="Component" className="EncodingSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array&lt;AudioParameterSet&gt;* pAudioParams"
                 variableInitialisers="pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="comboBoxPresets" id="4b25adf5b07e9492" memberName="comboBoxPresets"
            virtualName="" explicitFocusOrder="0" pos="83 8Rr 312M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="8 8Rr 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonImport" id="5a786eb91323df32" memberName="buttonImport"
              virtualName="" explicitFocusOrder="0" pos="81Rr 8Rr 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="import" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="153Rr 8Rr 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonExport" id="92cbcfe3e89f89e" memberName="buttonExport"
              virtualName="" explicitFocusOrder="0" pos="9Rr 8Rr 64 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="export" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="sourceDefinition" id="789a79909c18391b" memberName="sourceDefinition"
                    virtualName="" explicitFocusOrder="0" pos="8 112 16M 154M" class="SourceDefinitionComponent"
                    params="pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams"/>
  <TOGGLEBUTTON name="toggleDistanceEncoding" id="c46d0c7f045490ec" memberName="toggleDistanceEncoding"
                virtualName="" explicitFocusOrder="0" pos="14 19 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Distance Encoding"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleDirectionFlip" id="261d6104440c6519" memberName="toggleDirectionFlip"
                virtualName="" explicitFocusOrder="0" pos="14 79 207 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Flip Direction (Orientation)"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleDoppler" id="8a9ea68cd17e7a8c" memberName="toggleDoppler"
                virtualName="" explicitFocusOrder="0" pos="14 49 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Doppler" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="sliderDistanceScaler" id="86549d5794437a4a" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="24Rr 49 202 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="1.0" max="1000.0" int="0.0"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelDistanceScaler" id="3db2cd25c7d2d40f" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="229Rr 49 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="btnEditDistanceEncoding" id="d37af0003751ec97" memberName="btnEditDistanceEncoding"
              virtualName="" explicitFocusOrder="0" pos="24Rr 19 86 24" posRelativeX="b72378bdfe4e130"
              posRelativeY="b72378bdfe4e130" buttonText="edit..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

