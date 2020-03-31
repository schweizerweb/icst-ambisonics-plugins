/*
  ==============================================================================

    HelpValueTreeItem.h
    Created: 29 Mar 2020 5:11:08pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class TreeViewItemHelp  : public TreeViewItem
{
public:
    TreeViewItemHelp (XmlElement& xml_)
        : xml (xml_)
    {
    }

    int getItemWidth() const
    {
        return xml.getIntAttribute ("width", -1);
    }

    String getUniqueName() const
    {
        return xml.getTagName();
    }

    bool mightContainSubItems()
    {
        return xml.getFirstChildElement() != 0;
    }

    void paintItem (Graphics& g, int width, int height)
    {
        // if this item is selected, fill it with a background colour..
        if (isSelected())
            g.fillAll (Colours::red.withAlpha (0.3f));

        // use a "colour" attribute in the xml tag for this node to set the text colour..
        if(xml.hasAttribute("colour"))
            g.setColour (Colour (xml.getStringAttribute ("colour", "ffeeeeee").getHexValue32()));
        else
        {
            if(getUniqueName() == "Syntax")
                g.setColour(Colours::lightblue);
            else if(getUniqueName() == "Example")
                g.setColour(Colours::lightgreen);
            else
                g.setColour(Colours::lightgrey);
        }

        
        g.setFont (height * 0.7f);

        // draw the xml element's tag name..
        g.drawText (xml.hasAttribute("name") ? xml.getStringAttribute("name") : getUniqueName(), 4, 0, width - 4, height, Justification::centredLeft, true);
        if(xml.hasAttribute("desc"))
            g.drawText (xml.getStringAttribute("desc"), 100, 0, width - 104, height, Justification::centredLeft, true);
        if(xml.isTextElement())
            g.drawText (xml.getText(), 100, 0, width - 104, height, Justification::centredLeft, true);
    }

    void itemOpennessChanged (bool isNowOpen)
    {
        if (isNowOpen)
        {
            // if we've not already done so, we'll now add the tree's sub-items. You could
            // also choose to delete the existing ones and refresh them if that's more suitable
            // in your app.
            if (getNumSubItems() == 0)
            {
                // create and add sub-items to this node of the tree, corresponding to
                // each sub-element in the XML..

                forEachXmlChildElement (xml, child)
                {
                    jassert (child != 0);
                    addSubItem (new TreeViewItemHelp (*child));
                }
            }
        }
        else
        {
            // in this case, we'll leave any sub-items in the tree when the node gets closed,
            // though you could choose to delete them if that's more appropriate for
            // your application.
        }
    }

    var getDragSourceDescription()
    {
        return "TreeView Items";
    }

private:
    XmlElement& xml;
};
