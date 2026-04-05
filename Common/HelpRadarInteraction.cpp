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

#include "TreeViewItemHelp.h"
#include "HelpRadarInteraction.h"

HelpRadarInteraction::HelpRadarInteraction ()
{
    treeViewRadarInteraction.reset (new juce::TreeView ("treeViewRadarInteraction"));
    addAndMakeVisible (treeViewRadarInteraction.get());
    treeViewRadarInteraction->setRootItemVisible (false);

    const String treeXmlString ((CharPointer_UTF8)BinaryData::radar_documentation_xml);
    XmlDocument parser (treeXmlString);
    treeXml = parser.getDocumentElement();
    if (treeXml != nullptr)
    {
        rootItem.reset(new TreeViewItemHelp (*treeXml));
        treeViewRadarInteraction->setRootItem(rootItem.get());
        rootItem->setOpen (true);
    }

    setSize (600, 400);
}

HelpRadarInteraction::~HelpRadarInteraction()
{
    treeViewRadarInteraction = nullptr;
}

void HelpRadarInteraction::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void HelpRadarInteraction::resized()
{
    treeViewRadarInteraction->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
}
