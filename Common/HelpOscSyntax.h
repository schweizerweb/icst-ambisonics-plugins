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



#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class HelpOscSyntax  : public Component
{
public:
    //==============================================================================
    HelpOscSyntax ();
    ~HelpOscSyntax() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void showInfo(String info);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void broughtToFront() override;
    void mouseUp (const juce::MouseEvent& e) override;

    // Binary resources:
    static const char* osc_syntax_xml;
    static const int osc_syntax_xmlSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<XmlElement> treeXml;
    std::unique_ptr<TreeViewItem> rootItem;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TreeView> treeViewOscSyntax;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpOscSyntax)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

