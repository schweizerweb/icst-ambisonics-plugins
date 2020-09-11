/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
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
class HelpRadarInteraction  : public Component
{
public:
    //==============================================================================
    HelpRadarInteraction ();
    ~HelpRadarInteraction() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* radar_documentation_xml;
    static const int radar_documentation_xmlSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<XmlElement> treeXml;
    std::unique_ptr<TreeViewItem> rootItem;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TreeView> treeViewRadarInteraction;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpRadarInteraction)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

