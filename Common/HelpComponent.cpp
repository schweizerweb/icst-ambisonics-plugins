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
#include "HelpRadarInteraction.h"
#include "HelpOscSyntax.h"
#include "HelpWebBrowserComponent.h"
//[/Headers]

#include "HelpComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
HelpComponent::HelpComponent (bool isEncoder)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    tabHelp.reset (new TabbedComponent (TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabHelp.get());
    tabHelp->setTabBarDepth (30);
    tabHelp->addTab (TRANS("Radar Interaction"), Colours::lightgrey, new HelpRadarInteraction(), true);
    tabHelp->setCurrentTabIndex (0);


    //[UserPreSize]
    if(isEncoder)
    {
        tabHelp->addTab (TRANS("OSC Syntax"), Colours::lightgrey, new HelpOscSyntax(), true);
    }
    
    String documentationUrl = isEncoder
    ? "https://bitbucket.org/christian_schweizer/icst-ambisonics-plugins/wiki/Encoder_specification"
    : "https://bitbucket.org/christian_schweizer/icst-ambisonics-plugins/wiki/Decoder_specification";
    tabHelp->addTab(TRANS("Documentation"), Colours::lightgrey, new HelpWebBrowserComponent(documentationUrl), true);
    tabHelp->addTab(TRANS("Tutorials"), Colours::lightgrey, new HelpWebBrowserComponent("https://ambisonics.postach.io"), true);

    
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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HelpComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HelpComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabHelp->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
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
                   explicitFocusOrder="0" pos="0 0 0M 0M" orientation="top" tabBarDepth="30"
                   initialTab="0">
    <TAB name="Radar Interaction" colour="ffd3d3d3" useJucerComp="0" contentClassName="HelpRadarInteraction"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

