/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "DistanceEncodingComponent.h"
#include "EncoderPresetHelper.h"
//[/Headers]

#include "EncodingSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EncodingSettingsComponent::EncodingSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array<AudioParameterSet>* pAudioParams, EncoderPresetHelper* pPresetHelper)
    : pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams), pPresetHelper(pPresetHelper)
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

    buttonSave.reset (new TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

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

    buttonManagePresets.reset (new TextButton ("buttonManagePresets"));
    addAndMakeVisible (buttonManagePresets.get());
    buttonManagePresets->setButtonText (TRANS("manage..."));
    buttonManagePresets->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    updateEncodingUiElements();

    labelPresets->setVisible(MULTI_ENCODER_MODE);
    comboBoxPresets->setVisible(MULTI_ENCODER_MODE);
    buttonSave->setVisible(MULTI_ENCODER_MODE);
    buttonManagePresets->setVisible(MULTI_ENCODER_MODE);

    // load stored presets
    pPresetHelper->addActionListener(this);
    initializePresets();
    controlDimming();
    //[/Constructor]
}

EncodingSettingsComponent::~EncodingSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    sourceDefinition = nullptr;
    toggleDistanceEncoding = nullptr;
    toggleDirectionFlip = nullptr;
    toggleDoppler = nullptr;
    sliderDistanceScaler = nullptr;
    labelDistanceScaler = nullptr;
    btnEditDistanceEncoding = nullptr;
    buttonManagePresets = nullptr;


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

    comboBoxPresets->setBounds (83, getHeight() - 8 - 24, getWidth() - 290, 24);
    labelPresets->setBounds (8, getHeight() - 8 - 24, 64, 24);
    buttonSave->setBounds (getWidth() - 110 - 90, getHeight() - 8 - 24, 90, 24);
    sourceDefinition->setBounds (8, 112, getWidth() - 16, getHeight() - 154);
    sliderDistanceScaler->setBounds (getWidth() - 24 - 202, 49, 202, 24);
    labelDistanceScaler->setBounds (getWidth() - 229 - 109, 49, 109, 24);
    btnEditDistanceEncoding->setBounds (getWidth() - 24 - 86, 19, 86, 24);
    buttonManagePresets->setBounds (getWidth() - 8 - 90, getHeight() - 8 - 24, 90, 24);
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
        pPresetHelper->selectPresetName(comboBoxPresets->getText());
        comboBoxPresets->setText("", dontSendNotification);
        //[/UserComboBoxCode_comboBoxPresets]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void EncodingSettingsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
                return;

        pPresetHelper->writeToXmlFile(*newFile, pSources, pEncoderSettings);
        comboBoxPresets->setText("", dontSendNotification);
        delete newFile;
        //[/UserButtonCode_buttonSave]
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
    else if (buttonThatWasClicked == buttonManagePresets.get())
    {
        //[UserButtonCode_buttonManagePresets] -- add your button handler code here..
        presetManagerComponent.reset(new PresetManagerComponent(pPresetHelper));
        DialogWindow::showDialog("Preset Manager", presetManagerComponent.get(), this, Colours::black, true);
        //[/UserButtonCode_buttonManagePresets]
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
void EncodingSettingsComponent::updateEncodingUiElements()
{
    toggleDistanceEncoding->setToggleState(pEncoderSettings->distanceEncodingFlag, dontSendNotification);
    toggleDirectionFlip->setToggleState(pEncoderSettings->getDirectionFlip(), dontSendNotification);

    toggleDoppler->setToggleState(pEncoderSettings->dopplerEncodingFlag, dontSendNotification);
    sliderDistanceScaler->setValue(pEncoderSettings->getDistanceScaler());

    // TODO: Doppler temporarily deactivated
    toggleDoppler->setToggleState(false, dontSendNotification);
    toggleDoppler->setEnabled(false);
    labelDistanceScaler->setEnabled(false);
    sliderDistanceScaler->setEnabled(false);
}

void EncodingSettingsComponent::controlDimming() const
{
    btnEditDistanceEncoding->setEnabled(pEncoderSettings->distanceEncodingFlag);
}

void EncodingSettingsComponent::initializePresets()
{
    comboBoxPresets->clear();

    int id = 1;
    for(File file : pPresetHelper->presetFiles)
    {
        comboBoxPresets->addItem(file.getFileNameWithoutExtension(), id++);
    }
}

void EncodingSettingsComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        initializePresets();
    }

    if(message == ACTION_MESSAGE_PRESET_CHANGED)
    {
        sourceDefinition->refresh();
        updateEncodingUiElements();
        controlDimming();
        sendChangeMessage();
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
                 componentName="" parentClasses="public Component, public ChangeBroadcaster, public ActionListener"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, Array&lt;AudioParameterSet&gt;* pAudioParams, EncoderPresetHelper* pPresetHelper"
                 variableInitialisers="pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams), pPresetHelper(pPresetHelper)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="comboBoxPresets" id="4b25adf5b07e9492" memberName="comboBoxPresets"
            virtualName="" explicitFocusOrder="0" pos="83 8Rr 290M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="8 8Rr 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="110Rr 8Rr 90 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
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
  <TEXTBUTTON name="buttonManagePresets" id="47314282f0cb05bc" memberName="buttonManagePresets"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 90 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

