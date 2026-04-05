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
#include "HelpOscSyntax.h"

HelpOscSyntax::HelpOscSyntax ()
{
    treeViewOscSyntax.reset (new juce::TreeView ("treeViewOscSyntax"));
    addAndMakeVisible (treeViewOscSyntax.get());
    treeViewOscSyntax->setRootItemVisible (false);

    const String treeXmlString ((CharPointer_UTF8)BinaryData::osc_syntax_xml);
    XmlDocument parser (treeXmlString);
    treeXml = parser.getDocumentElement();
    if (treeXml != nullptr)
    {
        rootItem.reset(new TreeViewItemHelp (*treeXml));
        treeViewOscSyntax->setRootItem(rootItem.get());
        rootItem->setOpen (true);
    }

    setSize (600, 400);
    treeViewOscSyntax->addMouseListener(this, true);
}

HelpOscSyntax::~HelpOscSyntax()
{
    treeViewOscSyntax = nullptr;
}

void HelpOscSyntax::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void HelpOscSyntax::resized()
{
    treeViewOscSyntax->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
}

void HelpOscSyntax::broughtToFront()
{
    showInfo("Right-Click on Element to copy to clipboard...");
}

void HelpOscSyntax::mouseUp (const juce::MouseEvent& e)
{
    if(e.mods.isRightButtonDown())
    {
        if(treeViewOscSyntax->getNumSelectedItems()>0)
        {
            TreeViewItem* item = treeViewOscSyntax->getSelectedItem(0);
            String copyText = ((TreeViewItemHelp*)item)->getCopyableString();
            SystemClipboard::copyTextToClipboard(copyText);
            showInfo("Copied to clipboard: " + copyText);
        }
    }
}

void HelpOscSyntax::showInfo(String info)
{
    std::unique_ptr<Label> label = std::make_unique<Label>();
    label->setSize(this->getWidth() - 40, 30);
    label->setText(info, dontSendNotification);
    label->setJustificationType(Justification::centred);
    CallOutBox::launchAsynchronously(std::move(label) , getScreenBounds(), this);
}
