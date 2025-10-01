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

    treeViewRadarInteraction.reset (new juce::TreeView ("treeViewRadarInteraction"));
    addAndMakeVisible (treeViewRadarInteraction.get());
    treeViewRadarInteraction->setRootItemVisible (false);


    //[UserPreSize]
    const String treeXmlString ((CharPointer_UTF8)BinaryData::radar_documentation_xml);
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
void HelpRadarInteraction::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

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

//[EndFile] You can add extra defines here...
//[/EndFile]

