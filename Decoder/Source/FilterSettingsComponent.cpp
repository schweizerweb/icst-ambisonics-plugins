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

#include "FilterSettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterSettingsComponent::FilterSettingsComponent (FilterInfo* pFilterInfo, ChangeListener* pChangeListener)
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->pFilterInfo = pFilterInfo;
    //[/Constructor_pre]

    comboBoxType.reset (new ComboBox ("comboBoxType"));
    addAndMakeVisible (comboBoxType.get());
    comboBoxType->setEditableText (false);
    comboBoxType->setJustificationType (Justification::centredLeft);
    comboBoxType->setTextWhenNothingSelected (String());
    comboBoxType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBoxType->addListener (this);

    sliderFrequency.reset (new Slider ("sliderFrequency"));
    addAndMakeVisible (sliderFrequency.get());
    sliderFrequency->setRange (20, 30000, 0);
    sliderFrequency->setSliderStyle (Slider::LinearBar);
    sliderFrequency->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderFrequency->addListener (this);

    labelFrequency.reset (new Label ("labelFrequency",
                                     TRANS("Cut-off Frequency [Hz]")));
    addAndMakeVisible (labelFrequency.get());
    labelFrequency->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelFrequency->setJustificationType (Justification::centredLeft);
    labelFrequency->setEditable (false, false, false);
    labelFrequency->setColour (TextEditor::textColourId, Colours::black);
    labelFrequency->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelFrequency->setBounds (8, 40, 152, 24);

    labelFilterType.reset (new Label ("labelFilterType",
                                      TRANS("Filter Type")));
    addAndMakeVisible (labelFilterType.get());
    labelFilterType->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelFilterType->setJustificationType (Justification::centredLeft);
    labelFilterType->setEditable (false, false, false);
    labelFilterType->setColour (TextEditor::textColourId, Colours::black);
    labelFilterType->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelFilterType->setBounds (8, 8, 150, 24);

    labelQ.reset (new Label ("labelQ",
                             TRANS("Q-Value")));
    addAndMakeVisible (labelQ.get());
    labelQ->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelQ->setJustificationType (Justification::centredLeft);
    labelQ->setEditable (false, false, false);
    labelQ->setColour (TextEditor::textColourId, Colours::black);
    labelQ->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelQ->setBounds (8, 72, 150, 24);

    sliderQ.reset (new Slider ("sliderQ"));
    addAndMakeVisible (sliderQ.get());
    sliderQ->setRange (0, 100, 0);
    sliderQ->setSliderStyle (Slider::LinearBar);
    sliderQ->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sliderQ->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 110);


    //[Constructor] You can add your own custom stuff here..
	addChangeListener(pChangeListener);
	comboBoxType->addItem("Off", 1+FilterInfo::FilterType::None);
	comboBoxType->addItem("Low-Pass", 1+FilterInfo::FilterType::LowPass);
	comboBoxType->addItem("Band-Pass", 1+FilterInfo::FilterType::BandPass);
	comboBoxType->addItem("High-Pass", 1+FilterInfo::FilterType::HighPass);
	comboBoxType->setSelectedId(1+pFilterInfo->filterType, dontSendNotification);

	sliderFrequency->setNumDecimalPlacesToDisplay(0);
	sliderFrequency->setValue(pFilterInfo->cutOffFrequencyHz);
	sliderQ->setNumDecimalPlacesToDisplay(2);
	sliderQ->setValue(pFilterInfo->qValue);
    //[/Constructor]
}

FilterSettingsComponent::~FilterSettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBoxType = nullptr;
    sliderFrequency = nullptr;
    labelFrequency = nullptr;
    labelFilterType = nullptr;
    labelQ = nullptr;
    sliderQ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterSettingsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterSettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBoxType->setBounds (168, 8, getWidth() - 176, 24);
    sliderFrequency->setBounds (168, 40, getWidth() - 176, 24);
    sliderQ->setBounds (168, 72, getWidth() - 176, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterSettingsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBoxType.get())
    {
        //[UserComboBoxCode_comboBoxType] -- add your combo box handling code here..
		pFilterInfo->filterType = FilterInfo::FilterType(comboBoxType->getSelectedId()-1);
		sendChangeMessage();
        //[/UserComboBoxCode_comboBoxType]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FilterSettingsComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderFrequency.get())
    {
        //[UserSliderCode_sliderFrequency] -- add your slider handling code here..
		pFilterInfo->cutOffFrequencyHz = float(sliderFrequency->getValue());
        //[/UserSliderCode_sliderFrequency]
    }
    else if (sliderThatWasMoved == sliderQ.get())
    {
        //[UserSliderCode_sliderQ] -- add your slider handling code here..
		pFilterInfo->qValue = float(sliderQ->getValue());
        //[/UserSliderCode_sliderQ]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterSettingsComponent"
                 componentName="" parentClasses="public Component, public ChangeBroadcaster"
                 constructorParams="FilterInfo* pFilterInfo, ChangeListener* pChangeListener"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="300" initialHeight="110">
  <BACKGROUND backgroundColour="ff505050"/>
  <COMBOBOX name="comboBoxType" id="8896d80d8503e534" memberName="comboBoxType"
            virtualName="" explicitFocusOrder="0" pos="168 8 176M 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sliderFrequency" id="f23e9b966efc9df5" memberName="sliderFrequency"
          virtualName="" explicitFocusOrder="0" pos="168 40 176M 24" min="20.0"
          max="30000.0" int="0.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="labelFrequency" id="2b4b513621db1c33" memberName="labelFrequency"
         virtualName="" explicitFocusOrder="0" pos="8 40 152 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cut-off Frequency [Hz]" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelFilterType" id="45be7377944657a2" memberName="labelFilterType"
         virtualName="" explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Filter Type" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelQ" id="c59d0be17b74b579" memberName="labelQ" virtualName=""
         explicitFocusOrder="0" pos="8 72 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Q-Value" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sliderQ" id="d75dd45147568a5" memberName="sliderQ" virtualName=""
          explicitFocusOrder="0" pos="168 72 176M 24" min="0.0" max="100.0"
          int="0.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

