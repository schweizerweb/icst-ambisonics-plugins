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



//[Headers] You can add your own extra header files here...
#include "HelpRadarInteraction.h"
#include "HelpOscSyntax.h"
#include "HelpWebBrowserComponent.h"
#include "Constants.h"
//[/Headers]

#include "HelpComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
HelpComponent::HelpComponent (bool isEncoder)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    tabHelp.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabHelp.get());
    tabHelp->setTabBarDepth (30);
    tabHelp->addTab (TRANS("Radar Interaction"), juce::Colours::lightgrey, new HelpRadarInteraction(), true);
    tabHelp->setCurrentTabIndex (0);

    label.reset (new juce::Label ("new label",
                                  juce::CharPointer_UTF8 ("\xc2\xa9 2024 Martin Neukom, Johannes Schuett & Christian Schweizer @ ICST")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (juce::FontOptions("Regular", 15.00f, juce::Font::plain)));
    label->setJustificationType (juce::Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    if(isEncoder)
    {
        tabHelp->addTab (TRANS("OSC Syntax"), Colours::lightgrey, new HelpOscSyntax(), true);
    }

    String documentationUrl = isEncoder
    ? "https://github.com/schweizerweb/icst-ambisonics-plugins/wiki/ICST-AmbiEncoder"
    : "https://github.com/schweizerweb/icst-ambisonics-plugins/wiki/ICST-AmbiDecoder";
    tabHelp->addTab(TRANS("Documentation"), Colours::lightgrey, new HelpWebBrowserComponent(documentationUrl), true);
    tabHelp->addTab(TRANS("Tutorials"), Colours::lightgrey, new HelpWebBrowserComponent("https://ambisonics.ch/page/icst-ambisonics-plugins"), true);

    label->setText(String(JucePlugin_Name).upToFirstOccurrenceOf("_", false, false) + Constants::getUiVersionString(true) + " " + String::fromUTF8("\xc2\xa9 2024 Martin Neukom, Johannes Schuett & Christian Schweizer @ ICST"), dontSendNotification);
    label->setTooltip(Constants::getBuildInfo());
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HelpComponent::~HelpComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabHelp = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HelpComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HelpComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabHelp->setBounds (0, 0, getWidth() - 0, getHeight() - 22);
    label->setBounds (0, getHeight() - 3 - 16, getWidth() - 0, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HelpComponent" componentName=""
                 parentClasses="public Component" constructorParams="bool isEncoder"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TABBEDCOMPONENT name="tabHelp" id="7fe78fa9ebb609cc" memberName="tabHelp" virtualName=""
                   explicitFocusOrder="0" pos="0 0 0M 22M" orientation="top" tabBarDepth="30"
                   initialTab="0">
    <TAB name="Radar Interaction" colour="ffd3d3d3" useJucerComp="0" contentClassName="HelpRadarInteraction"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <LABEL name="new label" id="c9e7e52a2e8c0879" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 3Rr 0M 16" edTextCol="ff000000"
         edBkgCol="0" labelText="&#169; 2024 Martin Neukom, Johannes Schuett &amp; Christian Schweizer @ ICST"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

