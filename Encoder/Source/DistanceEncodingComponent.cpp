/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "EncoderConstants.h"
//[/Headers]

#include "DistanceEncodingComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DistanceEncodingComponent::DistanceEncodingComponent (DistanceEncodingParams* pParams, DistanceEncodingPresetHelper* pPresetHelper, ScalingInfo* pScaling)
    : pParams(pParams), pPresetHelper(pPresetHelper)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupAirAbsorbtion.reset (new juce::GroupComponent ("groupAirAbsorbtion",
                                                        TRANS("Air Absorbtion")));
    addAndMakeVisible (groupAirAbsorbtion.get());

    groupAttenuation.reset (new juce::GroupComponent ("groupAttenuation",
                                                      TRANS("Attenuation")));
    addAndMakeVisible (groupAttenuation.get());

    distanceEncodingGraph.reset (new DistanceEncodingGraph (pParams, pScaling));
    addAndMakeVisible (distanceEncodingGraph.get());
    distanceEncodingGraph->setName ("distanceEncodingGraph");

    sliderUnitCircleRadius.reset (new juce::Slider ("sliderUnitCircleRadius"));
    addAndMakeVisible (sliderUnitCircleRadius.get());
    sliderUnitCircleRadius->setRange (0.01, 1, 0.01);
    sliderUnitCircleRadius->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderUnitCircleRadius->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderUnitCircleRadius->addListener (this);

    labelUnitCircleRadius.reset (new juce::Label ("labelUnitCircleRadius",
                                                  TRANS("Unit Circle Radius")));
    addAndMakeVisible (labelUnitCircleRadius.get());
    labelUnitCircleRadius->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelUnitCircleRadius->setJustificationType (juce::Justification::centredLeft);
    labelUnitCircleRadius->setEditable (false, false, false);
    labelUnitCircleRadius->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelUnitCircleRadius->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelUnitCircleRadius->setBounds (16, 48, 140, 24);

    labelEncodingMode.reset (new juce::Label ("labelEncodingMode",
                                              TRANS("Encoding Mode")));
    addAndMakeVisible (labelEncodingMode.get());
    labelEncodingMode->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelEncodingMode->setJustificationType (juce::Justification::centredLeft);
    labelEncodingMode->setEditable (false, false, false);
    labelEncodingMode->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelEncodingMode->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelEncodingMode->setBounds (16, 24, 140, 24);

    comboBoxEncodingMode.reset (new juce::ComboBox ("comboBoxEncodingMode"));
    addAndMakeVisible (comboBoxEncodingMode.get());
    comboBoxEncodingMode->setEditableText (false);
    comboBoxEncodingMode->setJustificationType (juce::Justification::centredLeft);
    comboBoxEncodingMode->setTextWhenNothingSelected (juce::String());
    comboBoxEncodingMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxEncodingMode->addListener (this);

    sliderDbUnit.reset (new juce::Slider ("sliderDbUnit"));
    addAndMakeVisible (sliderDbUnit.get());
    sliderDbUnit->setRange (0.01, 100, 0.01);
    sliderDbUnit->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderDbUnit->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderDbUnit->addListener (this);

    labelDbUnit.reset (new juce::Label ("labelDbUnit",
                                        TRANS("dB Unit")));
    addAndMakeVisible (labelDbUnit.get());
    labelDbUnit->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDbUnit->setJustificationType (juce::Justification::centredLeft);
    labelDbUnit->setEditable (false, false, false);
    labelDbUnit->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDbUnit->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelDbUnit->setBounds (16, 120, 140, 24);

    sliderDistanceAttenuation.reset (new juce::Slider ("sliderDistanceAttenuation"));
    addAndMakeVisible (sliderDistanceAttenuation.get());
    sliderDistanceAttenuation->setRange (0.01, 20, 0.01);
    sliderDistanceAttenuation->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderDistanceAttenuation->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderDistanceAttenuation->addListener (this);

    labelDistanceAttenuation.reset (new juce::Label ("labelDistanceAttenuation",
                                                     TRANS("Distance Attenuation")));
    addAndMakeVisible (labelDistanceAttenuation.get());
    labelDistanceAttenuation->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceAttenuation->setJustificationType (juce::Justification::centredLeft);
    labelDistanceAttenuation->setEditable (false, false, false);
    labelDistanceAttenuation->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelDistanceAttenuation->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelDistanceAttenuation->setBounds (16, 168, 140, 24);

    sliderCenterCurve.reset (new juce::Slider ("sliderCenterCurve"));
    addAndMakeVisible (sliderCenterCurve.get());
    sliderCenterCurve->setRange (0, 1, 0.0001);
    sliderCenterCurve->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderCenterCurve->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderCenterCurve->addListener (this);

    labelCenterCurve.reset (new juce::Label ("labelCenterCurve",
                                             TRANS("Center Curve")));
    addAndMakeVisible (labelCenterCurve.get());
    labelCenterCurve->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCenterCurve->setJustificationType (juce::Justification::centredLeft);
    labelCenterCurve->setEditable (false, false, false);
    labelCenterCurve->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCenterCurve->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCenterCurve->setBounds (16, 144, 140, 24);

    sliderAdvancedFactor.reset (new juce::Slider ("sliderAdvancedFactor"));
    addAndMakeVisible (sliderAdvancedFactor.get());
    sliderAdvancedFactor->setRange (0, 5, 0.01);
    sliderAdvancedFactor->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderAdvancedFactor->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderAdvancedFactor->addListener (this);

    labelAdvancedFact.reset (new juce::Label ("labelAdvancedFact",
                                              TRANS("Advanced Factor")));
    addAndMakeVisible (labelAdvancedFact.get());
    labelAdvancedFact->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelAdvancedFact->setJustificationType (juce::Justification::centredLeft);
    labelAdvancedFact->setEditable (false, false, false);
    labelAdvancedFact->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelAdvancedFact->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelAdvancedFact->setBounds (16, 72, 140, 24);

    sliderAdvancedExponent.reset (new juce::Slider ("sliderAdvancedExponent"));
    addAndMakeVisible (sliderAdvancedExponent.get());
    sliderAdvancedExponent->setRange (0, 20, 0.01);
    sliderAdvancedExponent->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderAdvancedExponent->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sliderAdvancedExponent->addListener (this);

    labelAdvancedExponent.reset (new juce::Label ("labelAdvancedExponent",
                                                  TRANS("Advanced Exponent")));
    addAndMakeVisible (labelAdvancedExponent.get());
    labelAdvancedExponent->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelAdvancedExponent->setJustificationType (juce::Justification::centredLeft);
    labelAdvancedExponent->setEditable (false, false, false);
    labelAdvancedExponent->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelAdvancedExponent->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelAdvancedExponent->setBounds (16, 96, 140, 24);

    comboBoxDistanceEncodingPreset.reset (new juce::ComboBox ("comboBoxDistanceEncodingPreset"));
    addAndMakeVisible (comboBoxDistanceEncodingPreset.get());
    comboBoxDistanceEncodingPreset->setEditableText (false);
    comboBoxDistanceEncodingPreset->setJustificationType (juce::Justification::centredLeft);
    comboBoxDistanceEncodingPreset->setTextWhenNothingSelected (TRANS("-"));
    comboBoxDistanceEncodingPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxDistanceEncodingPreset->addListener (this);

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

    labelAirAbsorbtionMode.reset (new juce::Label ("labelAirAbsorbtionMode",
                                                   TRANS("Mode")));
    addAndMakeVisible (labelAirAbsorbtionMode.get());
    labelAirAbsorbtionMode->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelAirAbsorbtionMode->setJustificationType (juce::Justification::centredLeft);
    labelAirAbsorbtionMode->setEditable (false, false, false);
    labelAirAbsorbtionMode->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelAirAbsorbtionMode->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    comboBoxAirAbsorbtionMode.reset (new juce::ComboBox ("comboBoxAirAbsorbtionMode"));
    addAndMakeVisible (comboBoxAirAbsorbtionMode.get());
    comboBoxAirAbsorbtionMode->setEditableText (false);
    comboBoxAirAbsorbtionMode->setJustificationType (juce::Justification::centredLeft);
    comboBoxAirAbsorbtionMode->setTextWhenNothingSelected (juce::String());
    comboBoxAirAbsorbtionMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxAirAbsorbtionMode->addListener (this);

    sliderAirAbsorbtionIntensity.reset (new juce::Slider ("sliderAirAbsorbtionIntensity"));
    addAndMakeVisible (sliderAirAbsorbtionIntensity.get());
    sliderAirAbsorbtionIntensity->setRange (0, 100, 0);
    sliderAirAbsorbtionIntensity->setSliderStyle (juce::Slider::LinearHorizontal);
    sliderAirAbsorbtionIntensity->setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    sliderAirAbsorbtionIntensity->addListener (this);

    labelIntensity.reset (new juce::Label ("labelIntensity",
                                           TRANS("Intensity")));
    addAndMakeVisible (labelIntensity.get());
    labelIntensity->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelIntensity->setJustificationType (juce::Justification::centredLeft);
    labelIntensity->setEditable (false, false, false);
    labelIntensity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelIntensity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 500);


    //[Constructor] You can add your own custom stuff here..
    comboBoxEncodingMode->addItem(EncoderConstants::encodingModeStrings[EncoderConstants::Standard], EncoderConstants::Standard);
    comboBoxEncodingMode->addItem(EncoderConstants::encodingModeStrings[EncoderConstants::Advanced], EncoderConstants::Advanced);
    comboBoxEncodingMode->addItem(EncoderConstants::encodingModeStrings[EncoderConstants::Exponential], EncoderConstants::Exponential);
    comboBoxEncodingMode->addItem(EncoderConstants::encodingModeStrings[EncoderConstants::InverseProportional], EncoderConstants::InverseProportional);

    comboBoxAirAbsorbtionMode->addItem(EncoderConstants::airAbsorbtionModeStrings[EncoderConstants::Off], EncoderConstants::Off + 1);
    comboBoxAirAbsorbtionMode->addItem(EncoderConstants::airAbsorbtionModeStrings[EncoderConstants::LowPass], EncoderConstants::LowPass + 1);
    setUiValues(pParams);

    // set slider ranges according to constants
    sliderUnitCircleRadius->setRange(EncoderConstants::UnitCircleRadiusMin, EncoderConstants::UnitCircleRadiusMax, EncoderConstants::UnitCircleRadiusResolution);
    sliderDbUnit->setRange(EncoderConstants::DbUnitMin, EncoderConstants::DbUnitMax, EncoderConstants::DbUnitResolution);
    sliderDistanceAttenuation->setRange(EncoderConstants::DistanceAttenuationMin, EncoderConstants::DistanceAttenuationMax, EncoderConstants::DistanceAttenuationResolution);
    sliderCenterCurve->setRange(EncoderConstants::CenterCurveMin, EncoderConstants::CenterCurveMax, EncoderConstants::CenterCurveResolution);
    sliderAdvancedFactor->setRange(EncoderConstants::AdvancedFactorMin, EncoderConstants::AdvancedFactorMax, EncoderConstants::AdvancedFactorResolution);
    sliderAdvancedExponent->setRange(EncoderConstants::AdvancedExponentMin, EncoderConstants::AdvancedExponentMax, EncoderConstants::AdvancedExponentResolution);
    sliderAirAbsorbtionIntensity->setRange(EncoderConstants::AirAbsorbtionIntensityMin, EncoderConstants::AirAbsorbtionIntensityMax, EncoderConstants::AirAbsorbtionIntensityResolution);
    sliderAirAbsorbtionIntensity->setSkewFactorFromMidPoint(10.0);
    pParams->addChangeListener(this);

    updatePresetComboBox();
    pPresetHelper->addActionListener(this);

    controlDimming();

    //[/Constructor]
}

DistanceEncodingComponent::~DistanceEncodingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pParams->removeChangeListener(this);
    pPresetHelper->removeActionListener(this);
    //[/Destructor_pre]

    groupAirAbsorbtion = nullptr;
    groupAttenuation = nullptr;
    distanceEncodingGraph = nullptr;
    sliderUnitCircleRadius = nullptr;
    labelUnitCircleRadius = nullptr;
    labelEncodingMode = nullptr;
    comboBoxEncodingMode = nullptr;
    sliderDbUnit = nullptr;
    labelDbUnit = nullptr;
    sliderDistanceAttenuation = nullptr;
    labelDistanceAttenuation = nullptr;
    sliderCenterCurve = nullptr;
    labelCenterCurve = nullptr;
    sliderAdvancedFactor = nullptr;
    labelAdvancedFact = nullptr;
    sliderAdvancedExponent = nullptr;
    labelAdvancedExponent = nullptr;
    comboBoxDistanceEncodingPreset = nullptr;
    labelPresets = nullptr;
    buttonSave = nullptr;
    labelAirAbsorbtionMode = nullptr;
    comboBoxAirAbsorbtionMode = nullptr;
    sliderAirAbsorbtionIntensity = nullptr;
    labelIntensity = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DistanceEncodingComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DistanceEncodingComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupAirAbsorbtion->setBounds (0, getHeight() - 99, getWidth() - 0, 64);
    groupAttenuation->setBounds (0, 0, getWidth() - 0, getHeight() - 99);
    distanceEncodingGraph->setBounds (16, 200, proportionOfWidth (0.9674f), getHeight() - 315);
    sliderUnitCircleRadius->setBounds (160, 48, getWidth() - 174, 24);
    comboBoxEncodingMode->setBounds (160, 24, getWidth() - 174, 24);
    sliderDbUnit->setBounds (161, 120, getWidth() - 175, 24);
    sliderDistanceAttenuation->setBounds (161, 168, getWidth() - 175, 24);
    sliderCenterCurve->setBounds (161, 144, getWidth() - 175, 24);
    sliderAdvancedFactor->setBounds (161, 72, getWidth() - 175, 24);
    sliderAdvancedExponent->setBounds (161, 96, getWidth() - 175, 24);
    comboBoxDistanceEncodingPreset->setBounds (72, getHeight() - 30, getWidth() - 171, 24);
    labelPresets->setBounds (0, getHeight() - 30, 64, 24);
    buttonSave->setBounds (getWidth() - 6 - 80, getHeight() - 30, 80, 24);
    labelAirAbsorbtionMode->setBounds (19, getHeight() - 76, 53, 24);
    comboBoxAirAbsorbtionMode->setBounds (80, getHeight() - 75, 192, 24);
    sliderAirAbsorbtionIntensity->setBounds (368, getHeight() - 75, getWidth() - 382, 24);
    labelIntensity->setBounds (288, getHeight() - 76, 72, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DistanceEncodingComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderUnitCircleRadius.get())
    {
        //[UserSliderCode_sliderUnitCircleRadius] -- add your slider handling code here..
		pParams->setUnitCircleRadius(float(sliderUnitCircleRadius->getValue()));
        //[/UserSliderCode_sliderUnitCircleRadius]
    }
    else if (sliderThatWasMoved == sliderDbUnit.get())
    {
        //[UserSliderCode_sliderDbUnit] -- add your slider handling code here..
        pParams->setDbUnit(float(sliderDbUnit->getValue()));
        //[/UserSliderCode_sliderDbUnit]
    }
    else if (sliderThatWasMoved == sliderDistanceAttenuation.get())
    {
        //[UserSliderCode_sliderDistanceAttenuation] -- add your slider handling code here..
        pParams->setInverseProportionalDistanceAttenuation(float(sliderDistanceAttenuation->getValue()));
        //[/UserSliderCode_sliderDistanceAttenuation]
    }
    else if (sliderThatWasMoved == sliderCenterCurve.get())
    {
        //[UserSliderCode_sliderCenterCurve] -- add your slider handling code here..
        pParams->setCenterCurve(float(sliderCenterCurve->getValue()));
        //[/UserSliderCode_sliderCenterCurve]
    }
    else if (sliderThatWasMoved == sliderAdvancedFactor.get())
    {
        //[UserSliderCode_sliderAdvancedFactor] -- add your slider handling code here..
        pParams->setAdvancedFactor(float(sliderAdvancedFactor->getValue()));
        //[/UserSliderCode_sliderAdvancedFactor]
    }
    else if (sliderThatWasMoved == sliderAdvancedExponent.get())
    {
        //[UserSliderCode_sliderAdvancedExponent] -- add your slider handling code here..
        pParams->setAdvancedExponent(float(sliderAdvancedExponent->getValue()));
        //[/UserSliderCode_sliderAdvancedExponent]
    }
    else if (sliderThatWasMoved == sliderAirAbsorbtionIntensity.get())
    {
        //[UserSliderCode_sliderAirAbsorbtionIntensity] -- add your slider handling code here..
        pParams->setAirAbsorbtionIntensity(float(sliderAirAbsorbtionIntensity->getValue()));
        //[/UserSliderCode_sliderAirAbsorbtionIntensity]
    }

    //[UsersliderValueChanged_Post]
    distanceEncodingGraph->repaint();
    //[/UsersliderValueChanged_Post]
}

void DistanceEncodingComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxEncodingMode.get())
    {
        //[UserComboBoxCode_comboBoxEncodingMode] -- add your combo box handling code here..
        pParams->setEncodingMode(EncoderConstants::EncodingMode(comboBoxEncodingMode->getSelectedId()));
        controlDimming();
        distanceEncodingGraph->repaint();
        //[/UserComboBoxCode_comboBoxEncodingMode]
    }
    else if (comboBoxThatHasChanged == comboBoxDistanceEncodingPreset.get())
    {
        //[UserComboBoxCode_comboBoxDistanceEncodingPreset] -- add your combo box handling code here..
        String presetName = comboBoxDistanceEncodingPreset->getText();
        pPresetHelper->selectPresetName(presetName);
        comboBoxDistanceEncodingPreset->setSelectedItemIndex(-1);
        //[/UserComboBoxCode_comboBoxDistanceEncodingPreset]
    }
    else if (comboBoxThatHasChanged == comboBoxAirAbsorbtionMode.get())
    {
        //[UserComboBoxCode_comboBoxAirAbsorbtionMode] -- add your combo box handling code here..
        pParams->setAirAbsorbtionMode(EncoderConstants::AirAbsorbtionMode(comboBoxAirAbsorbtionMode->getSelectedId() - 1));
        controlDimming();
        //[/UserComboBoxCode_comboBoxAirAbsorbtionMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void DistanceEncodingComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSave.get())
    {
        //[UserButtonCode_buttonSave] -- add your button handler code here..
        File* newFile = pPresetHelper->tryCreateNewPreset();
        if(newFile == nullptr)
                return;

        pPresetHelper->writeToXmlFile(*newFile, pParams);
        comboBoxDistanceEncodingPreset->setText("", dontSendNotification);
        delete newFile;
        //[/UserButtonCode_buttonSave]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DistanceEncodingComponent::updatePresetComboBox()
{
    comboBoxDistanceEncodingPreset->clear();
    int i = 1;
    for (File file : pPresetHelper->presetFiles)
    {
        comboBoxDistanceEncodingPreset->addItem(file.getFileNameWithoutExtension(), i++);
    }
}
void DistanceEncodingComponent::actionListenerCallback(const String &message)
{
    if(message == ACTION_MESSAGE_PRESET_LIST_CHANGED)
    {
        updatePresetComboBox();
    }
    else if(message.startsWith(ACTION_MESSAGE_SELECT_PRESET))
    {
        File presetFile(message.substring(String(ACTION_MESSAGE_SELECT_PRESET).length()));
        pPresetHelper->loadFromXmlFile(presetFile, pParams);
        pPresetHelper->notifyPresetChanged();
        setUiValues(pParams);
    }
}
void DistanceEncodingComponent::controlDimming() const
{
    EncoderConstants::EncodingMode mode = pParams->getEncodingMode();
    bool inverseProportionalOrExponential = mode == EncoderConstants::Exponential || mode == EncoderConstants::InverseProportional;
    sliderCenterCurve->setEnabled(inverseProportionalOrExponential);
    labelCenterCurve->setEnabled(inverseProportionalOrExponential);
    sliderDbUnit->setEnabled(inverseProportionalOrExponential);
    labelDbUnit->setEnabled(inverseProportionalOrExponential);
    sliderDistanceAttenuation->setEnabled(mode == EncoderConstants::InverseProportional);
    labelDistanceAttenuation->setEnabled(mode == EncoderConstants::InverseProportional);
    sliderAdvancedFactor->setEnabled(mode == EncoderConstants::Advanced);
    labelAdvancedFact->setEnabled(mode == EncoderConstants::Advanced);
    sliderAdvancedExponent->setEnabled(mode == EncoderConstants::Advanced);
    labelAdvancedExponent->setEnabled(mode == EncoderConstants::Advanced);

    sliderAirAbsorbtionIntensity->setEnabled(pParams->getAirAbsorbtionMode() != EncoderConstants::Off);
}

void DistanceEncodingComponent::setUiValues(DistanceEncodingParams *pEncodingParams) {
    comboBoxEncodingMode->setSelectedId(pEncodingParams->getEncodingMode(), dontSendNotification);
    sliderUnitCircleRadius->setValue(pEncodingParams->getUnitCircleRadius(), dontSendNotification);
    sliderDbUnit->setSkewFactorFromMidPoint(5.0);
    sliderDbUnit->setValue(pEncodingParams->getDbUnit(), dontSendNotification);
    sliderDistanceAttenuation->setValue(pEncodingParams->getInverseProportionalDistanceAttenuation(), dontSendNotification);
    sliderCenterCurve->setSkewFactorFromMidPoint(0.1);
    sliderCenterCurve->setValue(pEncodingParams->getCenterCurve(), dontSendNotification);
    sliderAdvancedFactor->setValue(pEncodingParams->getAdvancedFactor(), dontSendNotification);
    sliderAdvancedExponent->setValue(pEncodingParams->getAdvancedExponent(), dontSendNotification);

    comboBoxAirAbsorbtionMode->setSelectedId(pEncodingParams->getAirAbsorbtionMode() + 1, dontSendNotification);
    sliderAirAbsorbtionIntensity->setValue(pEncodingParams->getAirAbsorbtionIntensity(), dontSendNotification);

    controlDimming();
    distanceEncodingGraph->repaint();
}

void DistanceEncodingComponent::changeListenerCallback(ChangeBroadcaster* /*source*/)
{
    setUiValues(pParams);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DistanceEncodingComponent"
                 componentName="" parentClasses="public Component, ChangeListener, ActionListener"
                 constructorParams="DistanceEncodingParams* pParams, DistanceEncodingPresetHelper* pPresetHelper, ScalingInfo* pScaling"
                 variableInitialisers="pParams(pParams), pPresetHelper(pPresetHelper)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="500">
  <BACKGROUND backgroundColour="ff505050"/>
  <GROUPCOMPONENT name="groupAirAbsorbtion" id="9e077e85238f4bfb" memberName="groupAirAbsorbtion"
                  virtualName="" explicitFocusOrder="0" pos="0 99R 0M 64" title="Air Absorbtion"/>
  <GROUPCOMPONENT name="groupAttenuation" id="874eb788ffea8f71" memberName="groupAttenuation"
                  virtualName="" explicitFocusOrder="0" pos="0 0 0M 99M" title="Attenuation"/>
  <GENERICCOMPONENT name="distanceEncodingGraph" id="eaba5f5be7082dad" memberName="distanceEncodingGraph"
                    virtualName="" explicitFocusOrder="0" pos="16 200 96.742% 315M"
                    class="DistanceEncodingGraph" params="pParams, pScaling"/>
  <SLIDER name="sliderUnitCircleRadius" id="33a23e1d161c87b2" memberName="sliderUnitCircleRadius"
          virtualName="" explicitFocusOrder="0" pos="160 48 174M 24" min="0.01"
          max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelUnitCircleRadius" id="1c135dae5e4bb342" memberName="labelUnitCircleRadius"
         virtualName="" explicitFocusOrder="0" pos="16 48 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Unit Circle Radius" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelEncodingMode" id="daf83410da235e44" memberName="labelEncodingMode"
         virtualName="" explicitFocusOrder="0" pos="16 24 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Encoding Mode" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxEncodingMode" id="2662ad301936b7c0" memberName="comboBoxEncodingMode"
            virtualName="" explicitFocusOrder="0" pos="160 24 174M 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderDbUnit" id="10e905f78cc1a4e8" memberName="sliderDbUnit"
          virtualName="" explicitFocusOrder="0" pos="161 120 175M 24" min="0.01"
          max="100.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelDbUnit" id="e622db3c11547177" memberName="labelDbUnit"
         virtualName="" explicitFocusOrder="0" pos="16 120 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="dB Unit" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderDistanceAttenuation" id="670b6b956458dbf0" memberName="sliderDistanceAttenuation"
          virtualName="" explicitFocusOrder="0" pos="161 168 175M 24" min="0.01"
          max="20.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelDistanceAttenuation" id="86897181d955aad6" memberName="labelDistanceAttenuation"
         virtualName="" explicitFocusOrder="0" pos="16 168 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Distance Attenuation" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderCenterCurve" id="c8ebd57ab2ec2a1d" memberName="sliderCenterCurve"
          virtualName="" explicitFocusOrder="0" pos="161 144 175M 24" min="0.0"
          max="1.0" int="0.0001" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterCurve" id="ee0374a11fdb34bf" memberName="labelCenterCurve"
         virtualName="" explicitFocusOrder="0" pos="16 144 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Center Curve" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderAdvancedFactor" id="acaa669a372543dd" memberName="sliderAdvancedFactor"
          virtualName="" explicitFocusOrder="0" pos="161 72 175M 24" min="0.0"
          max="5.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelAdvancedFact" id="11f4879337765cab" memberName="labelAdvancedFact"
         virtualName="" explicitFocusOrder="0" pos="16 72 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Advanced Factor" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderAdvancedExponent" id="848239971373225d" memberName="sliderAdvancedExponent"
          virtualName="" explicitFocusOrder="0" pos="161 96 175M 24" min="0.0"
          max="20.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelAdvancedExponent" id="5f0682ceb4e79610" memberName="labelAdvancedExponent"
         virtualName="" explicitFocusOrder="0" pos="16 96 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Advanced Exponent" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxDistanceEncodingPreset" id="4b25adf5b07e9492" memberName="comboBoxDistanceEncodingPreset"
            virtualName="" explicitFocusOrder="0" pos="72 30R 171M 24" posRelativeX="450188aa0f332e78"
            posRelativeY="450188aa0f332e78" editable="0" layout="33" items=""
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="107b43efebb2a5c8" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="0 30R 64 24" posRelativeY="450188aa0f332e78"
         edTextCol="ff000000" edBkgCol="0" labelText="Presets:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSave" id="80fd69347fffe9b6" memberName="buttonSave"
              virtualName="" explicitFocusOrder="0" pos="6Rr 30R 80 24" posRelativeX="450188aa0f332e78"
              posRelativeY="450188aa0f332e78" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="labelAirAbsorbtionMode" id="21b94baa2315f138" memberName="labelAirAbsorbtionMode"
         virtualName="" explicitFocusOrder="0" pos="19 76R 53 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxAirAbsorbtionMode" id="ee61081979986e59" memberName="comboBoxAirAbsorbtionMode"
            virtualName="" explicitFocusOrder="0" pos="80 75R 192 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderAirAbsorbtionIntensity" id="d2648c461e85094" memberName="sliderAirAbsorbtionIntensity"
          virtualName="" explicitFocusOrder="0" pos="368 75R 382M 24" min="0.0"
          max="100.0" int="0.0" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelIntensity" id="474bf2082d99f6e7" memberName="labelIntensity"
         virtualName="" explicitFocusOrder="0" pos="288 76R 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Intensity" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

