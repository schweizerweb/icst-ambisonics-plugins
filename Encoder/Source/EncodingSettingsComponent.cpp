/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

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
EncodingSettingsComponent::EncodingSettingsComponent (ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, EncoderPresetHelper* pPresetHelper, ZoomSettings* pZoomSettings, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper)
    : pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams), pPresetHelper(pPresetHelper), pZoomSettings(pZoomSettings), pDistanceEncodingPresetHelper(pDistanceEncodingPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addChangeListener(pChangeListener);
    //[/Constructor_pre]

    comboBoxPresets.reset (new juce::ComboBox ("comboBoxPresets"));
    addAndMakeVisible (comboBoxPresets.get());
    comboBoxPresets->setEditableText (false);
    comboBoxPresets->setJustificationType (juce::Justification::centredLeft);
    comboBoxPresets->setTextWhenNothingSelected (TRANS("-"));
    comboBoxPresets->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxPresets->addListener (this);

    labelPresets.reset (new juce::Label ("labelPresets",
                                         TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (juce::Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPresets->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonSave.reset (new juce::TextButton ("buttonSave"));
    addAndMakeVisible (buttonSave.get());
    buttonSave->setButtonText (TRANS("save"));
    buttonSave->addListener (this);

    sourceDefinition.reset (new SourceDefinitionComponent (pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams));
    addAndMakeVisible (sourceDefinition.get());
    sourceDefinition->setName ("sourceDefinition");

    toggleDistanceEncoding.reset (new juce::ToggleButton ("toggleDistanceEncoding"));
    addAndMakeVisible (toggleDistanceEncoding.get());
    toggleDistanceEncoding->setButtonText (TRANS("Enable Distance Encoding"));
    toggleDistanceEncoding->addListener (this);

    toggleDistanceEncoding->setBounds (14, 19, 199, 24);

    toggleDoppler.reset (new juce::ToggleButton ("toggleDoppler"));
    addAndMakeVisible (toggleDoppler.get());
    toggleDoppler->setButtonText (TRANS("Enable Doppler"));
    toggleDoppler->addListener (this);

    toggleDoppler->setBounds (14, 49, 199, 24);

    sliderDistanceScaler.reset (new juce::Slider ("sliderDistanceScaler"));
    addAndMakeVisible (sliderDistanceScaler.get());
    sliderDistanceScaler->setRange (1, 1000, 0.1);
    sliderDistanceScaler->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderDistanceScaler->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderDistanceScaler->addListener (this);

    labelDistanceScaler.reset (new juce::Label ("labelDistanceScaler",
                                                TRANS("Distance Scaler:")));
    addAndMakeVisible (labelDistanceScaler.get());
    labelDistanceScaler->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceScaler->setJustificationType (juce::Justification::centredLeft);
    labelDistanceScaler->setEditable (false, false, false);
    labelDistanceScaler->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDistanceScaler->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelDistanceScaler->setBounds (14, 79, 109, 24);

    btnEditDistanceEncoding.reset (new juce::TextButton ("btnEditDistanceEncoding"));
    addAndMakeVisible (btnEditDistanceEncoding.get());
    btnEditDistanceEncoding->setButtonText (TRANS("edit..."));
    btnEditDistanceEncoding->addListener (this);

    buttonManagePresets.reset (new juce::TextButton ("buttonManagePresets"));
    addAndMakeVisible (buttonManagePresets.get());
    buttonManagePresets->setButtonText (TRANS("manage..."));
    buttonManagePresets->addListener (this);

    toggleInfiniteDistance.reset (new juce::ToggleButton ("toggleInfiniteDistance"));
    addAndMakeVisible (toggleInfiniteDistance.get());
    toggleInfiniteDistance->setButtonText (TRANS("Infinite"));
    toggleInfiniteDistance->addListener (this);

    btnManageDistanceEncodingPresets.reset (new juce::TextButton ("btnManageDistanceEncodingPresets"));
    addAndMakeVisible (btnManageDistanceEncodingPresets.get());
    btnManageDistanceEncodingPresets->setButtonText (TRANS("presets..."));
    btnManageDistanceEncodingPresets->addListener (this);

    labelMasterGain.reset (new juce::Label ("labelMasterGain",
                                            TRANS("Master Gain [dB]:")));
    addAndMakeVisible (labelMasterGain.get());
    labelMasterGain->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelMasterGain->setJustificationType (juce::Justification::centredLeft);
    labelMasterGain->setEditable (false, false, false);
    labelMasterGain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMasterGain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelMasterGain->setBounds (14, 109, 109, 24);

    sliderMasterGain.reset (new juce::Slider ("sliderMasterGain"));
    addAndMakeVisible (sliderMasterGain.get());
    sliderMasterGain->setRange (0, 36, 0.1);
    sliderMasterGain->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderMasterGain->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderMasterGain->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    sliderDistanceScaler->setSkewFactorFromMidPoint(50.0);
    updateEncodingUiElements();

    labelPresets->setVisible(MULTI_ENCODER_MODE);
    comboBoxPresets->setVisible(MULTI_ENCODER_MODE);
    buttonSave->setVisible(MULTI_ENCODER_MODE);
    buttonManagePresets->setVisible(MULTI_ENCODER_MODE);
    labelMasterGain->setVisible(MULTI_ENCODER_MODE);
    sliderMasterGain->setVisible(MULTI_ENCODER_MODE);

    sliderMasterGain->setRange(EncoderConstants::MasterGainMin, EncoderConstants::MasterGainMax, EncoderConstants::MasterGainResolution);
    sliderMasterGain->setNumDecimalPlacesToDisplay(1);
    sliderMasterGain->setTextValueSuffix(" dB");
    sliderMasterGain->setValue(pEncoderSettings->getMasterGain());
    // load stored presets
    pPresetHelper->addActionListener(this);
    pEncoderSettings->addChangeListener(this);
    initializePresets();
    controlDimming();
    //[/Constructor]
}

EncodingSettingsComponent::~EncodingSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pEncoderSettings->removeChangeListener(this);
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    comboBoxPresets = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    sourceDefinition = nullptr;
    toggleDistanceEncoding = nullptr;
    toggleDoppler = nullptr;
    sliderDistanceScaler = nullptr;
    labelDistanceScaler = nullptr;
    btnEditDistanceEncoding = nullptr;
    buttonManagePresets = nullptr;
    toggleInfiniteDistance = nullptr;
    btnManageDistanceEncodingPresets = nullptr;
    labelMasterGain = nullptr;
    sliderMasterGain = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EncodingSettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

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
    sourceDefinition->setBounds (8, 144, getWidth() - 16, getHeight() - 186);
    sliderDistanceScaler->setBounds (getWidth() - 90 - (getWidth() - 301), 79, getWidth() - 301, 24);
    btnEditDistanceEncoding->setBounds (getWidth() - 108 - 86, 19, 86, 24);
    buttonManagePresets->setBounds (getWidth() - 8 - 90, getHeight() - 8 - 24, 90, 24);
    toggleInfiniteDistance->setBounds (getWidth() - 82, 79, 72, 24);
    btnManageDistanceEncodingPresets->setBounds (getWidth() - 12 - 86, 19, 86, 24);
    sliderMasterGain->setBounds (getWidth() - 90 - (getWidth() - 301), 109, getWidth() - 301, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EncodingSettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
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

void EncodingSettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
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
        CallOutBox::launchAsynchronously(std::make_unique<DistanceEncodingComponent>(&pEncoderSettings->distanceEncodingParams, pDistanceEncodingPresetHelper), getScreenBounds(), this);
        //[/UserButtonCode_btnEditDistanceEncoding]
    }
    else if (buttonThatWasClicked == buttonManagePresets.get())
    {
        //[UserButtonCode_buttonManagePresets] -- add your button handler code here..
        presetManagerDialog.show(this, pPresetHelper);
        //[/UserButtonCode_buttonManagePresets]
    }
    else if (buttonThatWasClicked == toggleInfiniteDistance.get())
    {
        //[UserButtonCode_toggleInfiniteDistance] -- add your button handler code here..
        pEncoderSettings->setDistanceScaler(toggleInfiniteDistance->getToggleState() ? Globals::Infinite : sliderDistanceScaler->getValue());
        Globals::SetScaler(pEncoderSettings->getDistanceScaler());
        sourceDefinition->refresh();
        updateEncodingUiElements();
        //[/UserButtonCode_toggleInfiniteDistance]
    }
    else if (buttonThatWasClicked == btnManageDistanceEncodingPresets.get())
    {
        //[UserButtonCode_btnManageDistanceEncodingPresets] -- add your button handler code here..
        presetManagerDialog.show(this, pDistanceEncodingPresetHelper, false);
        //[/UserButtonCode_btnManageDistanceEncodingPresets]
    }

    //[UserbuttonClicked_Post]
    controlDimming();
    //[/UserbuttonClicked_Post]
}

void EncodingSettingsComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderDistanceScaler.get())
    {
        //[UserSliderCode_sliderDistanceScaler] -- add your slider handling code here..
        pEncoderSettings->setDistanceScaler(sliderDistanceScaler->getValue());
        Globals::SetScaler(pEncoderSettings->getDistanceScaler());
        sendChangeMessage();
        sourceDefinition->refresh();
        //[/UserSliderCode_sliderDistanceScaler]
    }
    else if (sliderThatWasMoved == sliderMasterGain.get())
    {
        //[UserSliderCode_sliderMasterGain] -- add your slider handling code here..
        pEncoderSettings->setMasterGain((float)sliderMasterGain->getValue());
        //[/UserSliderCode_sliderMasterGain]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EncodingSettingsComponent::updateEncodingUiElements()
{
    toggleDistanceEncoding->setToggleState(pEncoderSettings->distanceEncodingFlag, dontSendNotification);

    toggleDoppler->setToggleState(pEncoderSettings->dopplerEncodingFlag, dontSendNotification);

    toggleInfiniteDistance->setToggleState(pEncoderSettings->getDistanceScaler() == 0.0, dontSendNotification);
    labelDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    sliderDistanceScaler->setEnabled(!toggleInfiniteDistance->getToggleState());
    if(!toggleInfiniteDistance->getToggleState())
        sliderDistanceScaler->setValue(pEncoderSettings->getDistanceScaler());
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
        pZoomSettings->Reset();
        controlDimming();
        sendChangeMessage();
    }
}

void EncodingSettingsComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    sliderMasterGain->setValue(pEncoderSettings->getMasterGain());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EncodingSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster, public ActionListener, public ChangeListener"
                 constructorParams="ChangeListener* pChangeListener, EncoderSettings* pSettings, AmbiSourceSet* pSourceSet, PointSelection* pPointSelection, AudioParams* pAudioParams, EncoderPresetHelper* pPresetHelper, ZoomSettings* pZoomSettings, DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper"
                 variableInitialisers="pEncoderSettings(pSettings), pSources(pSourceSet), pAudioParams(pAudioParams), pPresetHelper(pPresetHelper), pZoomSettings(pZoomSettings), pDistanceEncodingPresetHelper(pDistanceEncodingPresetHelper)"
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
                    virtualName="" explicitFocusOrder="0" pos="8 144 16M 186M" class="SourceDefinitionComponent"
                    params="pChangeListener, pSettings,pSourceSet, pPointSelection, pAudioParams"/>
  <TOGGLEBUTTON name="toggleDistanceEncoding" id="c46d0c7f045490ec" memberName="toggleDistanceEncoding"
                virtualName="" explicitFocusOrder="0" pos="14 19 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Distance Encoding"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="toggleDoppler" id="8a9ea68cd17e7a8c" memberName="toggleDoppler"
                virtualName="" explicitFocusOrder="0" pos="14 49 199 24" posRelativeX="b72378bdfe4e130"
                posRelativeY="b72378bdfe4e130" buttonText="Enable Doppler" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="sliderDistanceScaler" id="86549d5794437a4a" memberName="sliderDistanceScaler"
          virtualName="" explicitFocusOrder="0" pos="90Rr 79 301M 24" posRelativeX="b72378bdfe4e130"
          posRelativeY="b72378bdfe4e130" min="1.0" max="1000.0" int="0.1"
          style="LinearHorizontal" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="labelDistanceScaler" id="3db2cd25c7d2d40f" memberName="labelDistanceScaler"
         virtualName="" explicitFocusOrder="0" pos="14 79 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Distance Scaler:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnEditDistanceEncoding" id="d37af0003751ec97" memberName="btnEditDistanceEncoding"
              virtualName="" explicitFocusOrder="0" pos="108Rr 19 86 24" posRelativeX="b72378bdfe4e130"
              posRelativeY="b72378bdfe4e130" buttonText="edit..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonManagePresets" id="47314282f0cb05bc" memberName="buttonManagePresets"
              virtualName="" explicitFocusOrder="0" pos="8Rr 8Rr 90 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="manage..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="toggleInfiniteDistance" id="6a3353481b4b5310" memberName="toggleInfiniteDistance"
                virtualName="" explicitFocusOrder="0" pos="82R 79 72 24" buttonText="Infinite"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="btnManageDistanceEncodingPresets" id="e79fc007bc779712"
              memberName="btnManageDistanceEncodingPresets" virtualName=""
              explicitFocusOrder="0" pos="12Rr 19 86 24" posRelativeX="b72378bdfe4e130"
              posRelativeY="b72378bdfe4e130" buttonText="presets..." connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelMasterGain" id="5a6c2906ed7799ee" memberName="labelMasterGain"
         virtualName="" explicitFocusOrder="0" pos="14 109 109 24" posRelativeX="b72378bdfe4e130"
         posRelativeY="b72378bdfe4e130" edTextCol="ff000000" edBkgCol="0"
         labelText="Master Gain [dB]:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderMasterGain" id="48f17ace33ebcbca" memberName="sliderMasterGain"
          virtualName="" explicitFocusOrder="0" pos="90Rr 109 301M 24"
          posRelativeX="b72378bdfe4e130" posRelativeY="b72378bdfe4e130"
          min="0.0" max="36.0" int="0.1" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

