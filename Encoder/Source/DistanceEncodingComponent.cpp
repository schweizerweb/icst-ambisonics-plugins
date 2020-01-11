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
//[/Headers]

#include "DistanceEncodingComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DistanceEncodingComponent::DistanceEncodingComponent (DistanceEncodingParams* pParams)
    : pParams(pParams)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    distanceEncodingGraph.reset (new DistanceEncodingGraph (pParams));
    addAndMakeVisible (distanceEncodingGraph.get());
    distanceEncodingGraph->setName ("distanceEncodingGraph");

    sliderUnitCircleRadius.reset (new Slider ("sliderUnitCircleRadius"));
    addAndMakeVisible (sliderUnitCircleRadius.get());
    sliderUnitCircleRadius->setRange (0.01, 1, 0.01);
    sliderUnitCircleRadius->setSliderStyle (Slider::LinearHorizontal);
    sliderUnitCircleRadius->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderUnitCircleRadius->addListener (this);

    labelUnitCircleRadius.reset (new Label ("labelUnitCircleRadius",
                                            TRANS("Unit Circle Radius")));
    addAndMakeVisible (labelUnitCircleRadius.get());
    labelUnitCircleRadius->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelUnitCircleRadius->setJustificationType (Justification::centredLeft);
    labelUnitCircleRadius->setEditable (false, false, false);
    labelUnitCircleRadius->setColour (TextEditor::textColourId, Colours::black);
    labelUnitCircleRadius->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelUnitCircleRadius->setBounds (8, 32, 140, 24);

    labelEncodingMode.reset (new Label ("labelEncodingMode",
                                        TRANS("Encoding Mode")));
    addAndMakeVisible (labelEncodingMode.get());
    labelEncodingMode->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelEncodingMode->setJustificationType (Justification::centredLeft);
    labelEncodingMode->setEditable (false, false, false);
    labelEncodingMode->setColour (TextEditor::textColourId, Colours::black);
    labelEncodingMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelEncodingMode->setBounds (8, 8, 140, 24);

    comboBoxEncodingMode.reset (new ComboBox ("comboBoxEncodingMode"));
    addAndMakeVisible (comboBoxEncodingMode.get());
    comboBoxEncodingMode->setEditableText (false);
    comboBoxEncodingMode->setJustificationType (Justification::centredLeft);
    comboBoxEncodingMode->setTextWhenNothingSelected (String());
    comboBoxEncodingMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxEncodingMode->addListener (this);

    sliderDbUnit.reset (new Slider ("sliderDbUnit"));
    addAndMakeVisible (sliderDbUnit.get());
    sliderDbUnit->setRange (0.01, 100, 0.01);
    sliderDbUnit->setSliderStyle (Slider::LinearHorizontal);
    sliderDbUnit->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderDbUnit->addListener (this);

    labelDbUnit.reset (new Label ("labelDbUnit",
                                  TRANS("dB Unit")));
    addAndMakeVisible (labelDbUnit.get());
    labelDbUnit->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDbUnit->setJustificationType (Justification::centredLeft);
    labelDbUnit->setEditable (false, false, false);
    labelDbUnit->setColour (TextEditor::textColourId, Colours::black);
    labelDbUnit->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelDbUnit->setBounds (8, 56, 140, 24);

    sliderDistanceAttenuation.reset (new Slider ("sliderDistanceAttenuation"));
    addAndMakeVisible (sliderDistanceAttenuation.get());
    sliderDistanceAttenuation->setRange (0.01, 20, 0.01);
    sliderDistanceAttenuation->setSliderStyle (Slider::LinearHorizontal);
    sliderDistanceAttenuation->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderDistanceAttenuation->addListener (this);

    labelDistanceAttenuation.reset (new Label ("labelDistanceAttenuation",
                                               TRANS("Distance Attenuation")));
    addAndMakeVisible (labelDistanceAttenuation.get());
    labelDistanceAttenuation->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelDistanceAttenuation->setJustificationType (Justification::centredLeft);
    labelDistanceAttenuation->setEditable (false, false, false);
    labelDistanceAttenuation->setColour (TextEditor::textColourId, Colours::black);
    labelDistanceAttenuation->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelDistanceAttenuation->setBounds (8, 104, 140, 24);

    sliderCenterCurve.reset (new Slider ("sliderCenterCurve"));
    addAndMakeVisible (sliderCenterCurve.get());
    sliderCenterCurve->setRange (0, 1, 0.0001);
    sliderCenterCurve->setSliderStyle (Slider::LinearHorizontal);
    sliderCenterCurve->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderCenterCurve->addListener (this);

    labelCenterCurve.reset (new Label ("labelCenterCurve",
                                       TRANS("Center Curve")));
    addAndMakeVisible (labelCenterCurve.get());
    labelCenterCurve->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCenterCurve->setJustificationType (Justification::centredLeft);
    labelCenterCurve->setEditable (false, false, false);
    labelCenterCurve->setColour (TextEditor::textColourId, Colours::black);
    labelCenterCurve->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCenterCurve->setBounds (8, 80, 140, 24);

    sliderCenterAttenuation.reset (new Slider ("sliderCenterAttenuation"));
    addAndMakeVisible (sliderCenterAttenuation.get());
    sliderCenterAttenuation->setRange (0, 50, 0.01);
    sliderCenterAttenuation->setSliderStyle (Slider::LinearHorizontal);
    sliderCenterAttenuation->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderCenterAttenuation->addListener (this);

    labelCenterAttenuation.reset (new Label ("labelCenterAttenuation",
                                             TRANS("Center Attenuation")));
    addAndMakeVisible (labelCenterAttenuation.get());
    labelCenterAttenuation->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCenterAttenuation->setJustificationType (Justification::centredLeft);
    labelCenterAttenuation->setEditable (false, false, false);
    labelCenterAttenuation->setColour (TextEditor::textColourId, Colours::black);
    labelCenterAttenuation->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCenterAttenuation->setBounds (8, 128, 140, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    comboBoxEncodingMode->addItem("Standard", DistanceEncodingParams::Standard);
    comboBoxEncodingMode->addItem("Exponential", DistanceEncodingParams::Exponential);
    comboBoxEncodingMode->addItem("Inverse Proportional", DistanceEncodingParams::InverseProportional);
    comboBoxEncodingMode->setSelectedId(pParams->encodingMode, dontSendNotification);
	sliderUnitCircleRadius->setValue(pParams->unitCircleRadius, dontSendNotification);
    sliderDbUnit->setSkewFactorFromMidPoint(5.0);
    sliderDbUnit->setValue(pParams->dbUnit, dontSendNotification);
    sliderDistanceAttenuation->setValue(pParams->inverseProportionalDistanceAttenuation, dontSendNotification);
    sliderCenterCurve->setSkewFactorFromMidPoint(0.1);
    sliderCenterCurve->setValue(pParams->centerCurve, dontSendNotification);
    sliderCenterAttenuation->setValue(pParams->centerAttenuation, dontSendNotification);
    controlDimming();
    //[/Constructor]
}

DistanceEncodingComponent::~DistanceEncodingComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

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
    sliderCenterAttenuation = nullptr;
    labelCenterAttenuation = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DistanceEncodingComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DistanceEncodingComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    distanceEncodingGraph->setBounds (0, 160, proportionOfWidth (1.0000f), getHeight() - 160);
    sliderUnitCircleRadius->setBounds (152, 32, getWidth() - 165, 24);
    comboBoxEncodingMode->setBounds (152, 8, getWidth() - 165, 24);
    sliderDbUnit->setBounds (153, 56, getWidth() - 165, 24);
    sliderDistanceAttenuation->setBounds (153, 104, getWidth() - 165, 24);
    sliderCenterCurve->setBounds (153, 80, getWidth() - 165, 24);
    sliderCenterAttenuation->setBounds (153, 128, getWidth() - 165, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DistanceEncodingComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderUnitCircleRadius.get())
    {
        //[UserSliderCode_sliderUnitCircleRadius] -- add your slider handling code here..
		pParams->unitCircleRadius = float(sliderUnitCircleRadius->getValue());
        //[/UserSliderCode_sliderUnitCircleRadius]
    }
    else if (sliderThatWasMoved == sliderDbUnit.get())
    {
        //[UserSliderCode_sliderDbUnit] -- add your slider handling code here..
        pParams->dbUnit = float(sliderDbUnit->getValue());
        //[/UserSliderCode_sliderDbUnit]
    }
    else if (sliderThatWasMoved == sliderDistanceAttenuation.get())
    {
        //[UserSliderCode_sliderDistanceAttenuation] -- add your slider handling code here..
        pParams->inverseProportionalDistanceAttenuation = float(sliderDistanceAttenuation->getValue());
        //[/UserSliderCode_sliderDistanceAttenuation]
    }
    else if (sliderThatWasMoved == sliderCenterCurve.get())
    {
        //[UserSliderCode_sliderCenterCurve] -- add your slider handling code here..
        pParams->centerCurve = float(sliderCenterCurve->getValue());
        //[/UserSliderCode_sliderCenterCurve]
    }
    else if (sliderThatWasMoved == sliderCenterAttenuation.get())
    {
        //[UserSliderCode_sliderCenterAttenuation] -- add your slider handling code here..
        pParams->centerAttenuation = float(sliderCenterAttenuation->getValue());
        //[/UserSliderCode_sliderCenterAttenuation]
    }

    //[UsersliderValueChanged_Post]
    distanceEncodingGraph->repaint();
    //[/UsersliderValueChanged_Post]
}

void DistanceEncodingComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxEncodingMode.get())
    {
        //[UserComboBoxCode_comboBoxEncodingMode] -- add your combo box handling code here..
        pParams->encodingMode = DistanceEncodingParams::EncodingMode(comboBoxEncodingMode->getSelectedId());
        controlDimming();
        distanceEncodingGraph->repaint();
        //[/UserComboBoxCode_comboBoxEncodingMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DistanceEncodingComponent::controlDimming() const
{
    sliderCenterCurve->setEnabled(pParams->encodingMode != DistanceEncodingParams::Standard);
    labelCenterCurve->setEnabled(pParams->encodingMode != DistanceEncodingParams::Standard);
    sliderDbUnit->setEnabled(pParams->encodingMode != DistanceEncodingParams::Standard);
    labelDbUnit->setEnabled(pParams->encodingMode != DistanceEncodingParams::Standard);
    sliderDistanceAttenuation->setEnabled(pParams->encodingMode == DistanceEncodingParams::InverseProportional);
    labelDistanceAttenuation->setEnabled(pParams->encodingMode == DistanceEncodingParams::InverseProportional);
    sliderCenterAttenuation->setEnabled(false);
    labelCenterAttenuation->setEnabled(false);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DistanceEncodingComponent"
                 componentName="" parentClasses="public Component" constructorParams="DistanceEncodingParams* pParams"
                 variableInitialisers="pParams(pParams)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="distanceEncodingGraph" id="eaba5f5be7082dad" memberName="distanceEncodingGraph"
                    virtualName="" explicitFocusOrder="0" pos="0 160 100% 160M" class="DistanceEncodingGraph"
                    params="pParams"/>
  <SLIDER name="sliderUnitCircleRadius" id="33a23e1d161c87b2" memberName="sliderUnitCircleRadius"
          virtualName="" explicitFocusOrder="0" pos="152 32 165M 24" min="0.01"
          max="1.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelUnitCircleRadius" id="1c135dae5e4bb342" memberName="labelUnitCircleRadius"
         virtualName="" explicitFocusOrder="0" pos="8 32 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Unit Circle Radius" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelEncodingMode" id="daf83410da235e44" memberName="labelEncodingMode"
         virtualName="" explicitFocusOrder="0" pos="8 8 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Encoding Mode" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="comboBoxEncodingMode" id="2662ad301936b7c0" memberName="comboBoxEncodingMode"
            virtualName="" explicitFocusOrder="0" pos="152 8 165M 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderDbUnit" id="10e905f78cc1a4e8" memberName="sliderDbUnit"
          virtualName="" explicitFocusOrder="0" pos="153 56 165M 24" min="0.01"
          max="100.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelDbUnit" id="e622db3c11547177" memberName="labelDbUnit"
         virtualName="" explicitFocusOrder="0" pos="8 56 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="dB Unit" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderDistanceAttenuation" id="670b6b956458dbf0" memberName="sliderDistanceAttenuation"
          virtualName="" explicitFocusOrder="0" pos="153 104 165M 24" min="0.01"
          max="20.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelDistanceAttenuation" id="86897181d955aad6" memberName="labelDistanceAttenuation"
         virtualName="" explicitFocusOrder="0" pos="8 104 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Distance Attenuation" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderCenterCurve" id="c8ebd57ab2ec2a1d" memberName="sliderCenterCurve"
          virtualName="" explicitFocusOrder="0" pos="153 80 165M 24" min="0.0"
          max="1.0" int="0.0001" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterCurve" id="ee0374a11fdb34bf" memberName="labelCenterCurve"
         virtualName="" explicitFocusOrder="0" pos="8 80 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Center Curve" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderCenterAttenuation" id="acaa669a372543dd" memberName="sliderCenterAttenuation"
          virtualName="" explicitFocusOrder="0" pos="153 128 165M 24" min="0.0"
          max="50.0" int="0.01" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelCenterAttenuation" id="11f4879337765cab" memberName="labelCenterAttenuation"
         virtualName="" explicitFocusOrder="0" pos="8 128 140 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Center Attenuation" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

