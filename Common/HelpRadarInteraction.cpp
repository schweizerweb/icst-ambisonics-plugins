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
#include "TreeViewItemHelp.h"
//[/Headers]

#include "HelpRadarInteraction.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
HelpRadarInteraction::HelpRadarInteraction ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    treeViewRadarInteraction.reset (new TreeView ("treeViewRadarInteraction"));
    addAndMakeVisible (treeViewRadarInteraction.get());
    treeViewRadarInteraction->setRootItemVisible (false);


    //[UserPreSize]
    const String treeXmlString ((CharPointer_UTF8)HelpRadarInteraction::radar_documentation_xml);
    XmlDocument parser (treeXmlString);
    treeXml = parser.getDocumentElement();
    if (treeXml != nullptr)
    {
        rootItem.reset(new TreeViewItemHelp (*treeXml));
        treeViewRadarInteraction->setRootItem(rootItem.get());
        rootItem->setOpen (true);
    }
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

HelpRadarInteraction::~HelpRadarInteraction()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    treeViewRadarInteraction = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void HelpRadarInteraction::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void HelpRadarInteraction::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    treeViewRadarInteraction->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
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

<JUCER_COMPONENT documentType="Component" className="HelpRadarInteraction" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TREEVIEW name="treeViewRadarInteraction" id="e91e7a8989c6152f" memberName="treeViewRadarInteraction"
            virtualName="" explicitFocusOrder="0" pos="0 0 0M 0M" rootVisible="0"
            openByDefault="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: radar_documentation_xml, 1149, "../Resources/Radar_Documentation.xml"
static const unsigned char resource_HelpRadarInteraction_radar_documentation_xml[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
70,45,56,34,63,62,10,60,68,111,99,117,109,101,110,116,97,116,105,111,110,62,10,60,82,97,100,97,114,62,10,60,77,111,118,101,62,82,105,103,104,116,32,77,111,117,115,101,32,66,117,116,116,111,110,32,68,114,
97,103,45,68,114,111,112,60,47,77,111,118,101,62,10,60,90,111,111,109,73,110,32,110,97,109,101,61,34,90,111,111,109,32,73,110,34,62,82,105,103,104,116,32,77,111,117,115,101,32,66,117,116,116,111,110,32,
67,108,105,99,107,32,47,32,77,111,117,115,101,87,104,101,101,108,60,47,90,111,111,109,73,110,62,10,60,90,111,111,109,79,117,116,32,110,97,109,101,61,34,90,111,111,109,32,79,117,116,34,62,83,104,105,102,
116,32,43,32,82,105,103,104,116,32,77,111,117,115,101,32,66,117,116,116,111,110,32,67,108,105,99,107,32,47,32,77,111,117,115,101,87,104,101,101,108,60,47,90,111,111,109,79,117,116,62,10,60,82,101,115,
101,116,90,111,111,109,32,110,97,109,101,61,34,82,101,115,101,116,32,90,111,111,109,34,62,72,111,109,101,45,66,117,116,116,111,110,32,40,79,83,88,58,32,67,109,100,32,43,32,76,101,102,116,32,65,114,114,
111,119,41,60,47,82,101,115,101,116,90,111,111,109,62,10,60,47,82,97,100,97,114,62,10,60,80,111,105,110,116,62,10,60,83,101,108,101,99,116,80,111,105,110,116,32,110,97,109,101,61,34,83,101,108,101,99,
116,32,80,111,105,110,116,34,62,67,108,105,99,107,32,80,111,105,110,116,60,47,83,101,108,101,99,116,80,111,105,110,116,62,10,60,83,101,108,101,99,116,77,117,108,116,105,112,108,101,80,111,105,110,116,
115,32,110,97,109,101,32,61,32,34,83,101,108,101,99,116,32,77,117,108,116,105,112,108,101,32,80,111,105,110,116,115,34,62,83,104,105,102,116,32,43,32,67,108,105,99,107,32,77,117,108,116,105,112,108,101,
32,80,111,105,110,116,115,32,111,114,32,83,101,108,101,99,116,32,114,101,99,116,97,110,103,117,108,97,114,32,82,101,103,105,111,110,32,117,115,105,110,103,32,68,114,97,103,45,68,114,111,112,60,47,83,101,
108,101,99,116,77,117,108,116,105,112,108,101,80,111,105,110,116,115,62,10,60,77,111,118,101,80,111,105,110,116,32,110,97,109,101,61,34,77,111,118,101,32,80,111,105,110,116,40,115,41,34,62,68,114,97,103,
45,68,114,111,112,32,83,101,108,101,99,116,101,100,32,80,111,105,110,116,40,115,41,60,47,77,111,118,101,80,111,105,110,116,62,10,60,65,100,100,78,101,119,32,110,97,109,101,61,34,65,100,100,32,78,101,119,
34,62,68,111,117,98,108,101,99,108,105,99,107,32,111,110,32,100,101,115,105,114,101,100,32,80,111,115,105,116,105,111,110,32,40,111,110,108,121,32,97,118,97,105,108,97,98,108,101,32,105,102,32,109,97,
120,105,109,117,109,32,110,111,116,32,121,101,116,32,114,101,97,99,104,101,100,41,60,47,65,100,100,78,101,119,62,10,60,47,80,111,105,110,116,62,10,60,71,114,111,117,112,62,10,60,83,101,108,101,99,116,
71,114,111,117,112,32,110,97,109,101,61,34,83,101,108,101,99,116,32,71,114,111,117,112,34,62,67,108,105,99,107,32,71,114,111,117,112,32,80,111,105,110,116,60,47,83,101,108,101,99,116,71,114,111,117,112,
62,10,60,83,101,108,101,99,116,77,117,108,116,105,112,108,101,71,114,111,117,112,115,32,110,97,109,101,32,61,32,34,83,101,108,101,99,116,32,77,117,108,116,105,112,108,101,32,71,114,111,117,112,115,34,
62,83,104,105,102,116,32,43,32,67,108,105,99,107,32,77,117,108,116,105,112,108,101,32,71,114,111,117,112,32,80,111,105,110,116,115,60,47,83,101,108,101,99,116,77,117,108,116,105,112,108,101,71,114,111,
117,112,115,62,10,60,77,111,118,101,71,114,111,117,112,32,110,97,109,101,61,34,77,111,118,101,32,71,114,111,117,112,40,115,41,34,62,68,114,97,103,45,68,114,111,112,32,83,101,108,101,99,116,101,100,32,
71,114,111,117,112,32,80,111,105,110,116,40,115,41,60,47,77,111,118,101,71,114,111,117,112,62,10,60,71,114,111,117,112,77,111,118,101,65,101,100,32,110,97,109,101,61,34,77,111,118,101,32,71,114,111,117,
112,40,115,41,32,114,101,108,97,116,105,118,101,32,116,111,32,65,69,68,45,83,112,97,99,101,34,62,65,108,116,32,43,32,68,114,97,103,45,68,114,111,112,32,83,101,108,101,99,116,101,100,32,71,114,111,117,
112,32,80,111,105,110,116,115,60,47,71,114,111,117,112,77,111,118,101,65,101,100,62,10,60,47,71,114,111,117,112,62,10,60,47,68,111,99,117,109,101,110,116,97,116,105,111,110,62,0,0};

const char* HelpRadarInteraction::radar_documentation_xml = (const char*) resource_HelpRadarInteraction_radar_documentation_xml;
const int HelpRadarInteraction::radar_documentation_xmlSize = 1149;


//[EndFile] You can add extra defines here...
//[/EndFile]

