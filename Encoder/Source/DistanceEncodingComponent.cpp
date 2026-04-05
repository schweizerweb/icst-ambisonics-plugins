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

#include "../../Common/EncoderConstants.h"
#include "DistanceEncodingComponent.h"

DistanceEncodingComponent::DistanceEncodingComponent (DistanceEncodingParams* _pParams, DistanceEncodingPresetHelper* _pPresetHelper, ZoomSettings* _pZoomSettings)
    : pParams(_pParams), pPresetHelper(_pPresetHelper), pZoomSettings(_pZoomSettings)
{
    groupAirAbsorbtion.reset (new juce::GroupComponent ("groupAirAbsorbtion",
                                                        TRANS("Air Absorbtion")));
    addAndMakeVisible (groupAirAbsorbtion.get());

    groupAttenuation.reset (new juce::GroupComponent ("groupAttenuation",
                                                      TRANS("Attenuation")));
    addAndMakeVisible (groupAttenuation.get());

    distanceEncodingGraph.reset (new DistanceEncodingGraph (pParams, pZoomSettings));
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
    labelUnitCircleRadius->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelUnitCircleRadius->setJustificationType (juce::Justification::centredLeft);
    labelUnitCircleRadius->setEditable (false, false, false);
    labelUnitCircleRadius->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelUnitCircleRadius->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelUnitCircleRadius->setBounds (16, 48, 140, 24);

    labelEncodingMode.reset (new juce::Label ("labelEncodingMode",
                                              TRANS("Encoding Mode")));
    addAndMakeVisible (labelEncodingMode.get());
    labelEncodingMode->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelDbUnit->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelDistanceAttenuation->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelCenterCurve->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelAdvancedFact->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelAdvancedExponent->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelPresets->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelAirAbsorbtionMode->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
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
    labelIntensity->setFont (juce::Font (juce::FontOptions(15.00f, juce::Font::plain)));
    labelIntensity->setJustificationType (juce::Justification::centredLeft);
    labelIntensity->setEditable (false, false, false);
    labelIntensity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelIntensity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    buttonManagePresets.reset (new juce::TextButton ("buttonManagePresets"));
    addAndMakeVisible (buttonManagePresets.get());
    buttonManagePresets->setButtonText (TRANS("manage..."));
    buttonManagePresets->addListener (this);

    setSize (600, 500);

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
    pZoomSettings->addChangeListener(this);

    updatePresetComboBox();
    pPresetHelper->addActionListener(this);

    controlDimming();
}

DistanceEncodingComponent::~DistanceEncodingComponent()
{
    pParams->removeChangeListener(this);
    pPresetHelper->removeActionListener(this);
    pZoomSettings->removeChangeListener(this);

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
    buttonManagePresets = nullptr;
}

void DistanceEncodingComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void DistanceEncodingComponent::resized()
{
    groupAirAbsorbtion->setBounds (0, getHeight() - 99, getWidth() - 0, 64);
    groupAttenuation->setBounds (0, 0, getWidth() - 0, getHeight() - 99);
    distanceEncodingGraph->setBounds (16, 200, getWidth() - 32, getHeight() - 316);
    sliderUnitCircleRadius->setBounds (160, 48, getWidth() - 174, 24);
    comboBoxEncodingMode->setBounds (160, 24, getWidth() - 174, 24);
    sliderDbUnit->setBounds (161, 120, getWidth() - 175, 24);
    sliderDistanceAttenuation->setBounds (161, 168, getWidth() - 175, 24);
    sliderCenterCurve->setBounds (161, 144, getWidth() - 175, 24);
    sliderAdvancedFactor->setBounds (161, 72, getWidth() - 175, 24);
    sliderAdvancedExponent->setBounds (161, 96, getWidth() - 175, 24);
    comboBoxDistanceEncodingPreset->setBounds (72, getHeight() - 30, getWidth() - 278, 24);
    labelPresets->setBounds (0, getHeight() - 30, 64, 24);
    buttonSave->setBounds (getWidth() - 115 - 80, getHeight() - 30, 80, 24);
    labelAirAbsorbtionMode->setBounds (19, getHeight() - 76, 53, 24);
    comboBoxAirAbsorbtionMode->setBounds (80, getHeight() - 75, 192, 24);
    sliderAirAbsorbtionIntensity->setBounds (368, getHeight() - 75, getWidth() - 382, 24);
    labelIntensity->setBounds (288, getHeight() - 76, 72, 24);
    buttonManagePresets->setBounds (getWidth() - 6 - 96, getHeight() - 30, 96, 24);
}

void DistanceEncodingComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sliderUnitCircleRadius.get())
    {
		pParams->setUnitCircleRadius(float(sliderUnitCircleRadius->getValue()));
    }
    else if (sliderThatWasMoved == sliderDbUnit.get())
    {
        pParams->setDbUnit(float(sliderDbUnit->getValue()));
    }
    else if (sliderThatWasMoved == sliderDistanceAttenuation.get())
    {
        pParams->setInverseProportionalDistanceAttenuation(float(sliderDistanceAttenuation->getValue()));
    }
    else if (sliderThatWasMoved == sliderCenterCurve.get())
    {
        pParams->setCenterCurve(float(sliderCenterCurve->getValue()));
    }
    else if (sliderThatWasMoved == sliderAdvancedFactor.get())
    {
        pParams->setAdvancedFactor(float(sliderAdvancedFactor->getValue()));
    }
    else if (sliderThatWasMoved == sliderAdvancedExponent.get())
    {
        pParams->setAdvancedExponent(float(sliderAdvancedExponent->getValue()));
    }
    else if (sliderThatWasMoved == sliderAirAbsorbtionIntensity.get())
    {
        pParams->setAirAbsorbtionIntensity(float(sliderAirAbsorbtionIntensity->getValue()));
    }

    distanceEncodingGraph->repaint();
}

void DistanceEncodingComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == comboBoxEncodingMode.get())
    {
        pParams->setEncodingMode(EncoderConstants::EncodingMode(comboBoxEncodingMode->getSelectedId()));
        controlDimming();
        distanceEncodingGraph->repaint();
    }
    else if (comboBoxThatHasChanged == comboBoxDistanceEncodingPreset.get())
    {
        String presetName = comboBoxDistanceEncodingPreset->getText();
        pPresetHelper->selectPresetName(presetName);
        comboBoxDistanceEncodingPreset->setSelectedItemIndex(-1);
    }
    else if (comboBoxThatHasChanged == comboBoxAirAbsorbtionMode.get())
    {
        pParams->setAirAbsorbtionMode(EncoderConstants::AirAbsorbtionMode(comboBoxAirAbsorbtionMode->getSelectedId() - 1));
        controlDimming();
    }
}

void DistanceEncodingComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == buttonSave.get())
    {
        pPresetHelper->tryCreateNewPreset([&](File* newFile){
            if (newFile != nullptr)
            {
                pPresetHelper->writeToXmlFile(*newFile, pParams);
                comboBoxDistanceEncodingPreset->setText("", dontSendNotification);
            }
        });
    }
    else if (buttonThatWasClicked == buttonManagePresets.get())
    {
        presetManagerDialog.show(this, pPresetHelper, false);
    }
}

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
    else if(message.startsWith(pPresetHelper->UniqueActionMessageSelectPreset()))
    {
        File presetFile(message.substring(pPresetHelper->UniqueActionMessageSelectPreset().length()));
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
    controlDimming();
}
