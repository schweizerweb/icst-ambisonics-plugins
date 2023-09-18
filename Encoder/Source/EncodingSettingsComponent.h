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
#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceDefinitionComponent.h"
#include "../../Common/PresetManagerDialog.h"
#include "../../Common/ZoomSettings.h"
#include "DistanceEncodingPresetHelper.h"
#include "ComponentArguments.h"
#include "DistanceEncodingComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EncodingSettingsComponent  : public Component,
                                   public ChangeBroadcaster,
                                   public juce::Button::Listener
{
public:
    //==============================================================================
    EncodingSettingsComponent (EncoderSettingsComponentArgs args);
    ~EncodingSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void initializePresets();
    void updateEncodingUiElements();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PresetManagerDialog presetManagerDialog;
    EncoderSettingsComponentArgs m_args;
    
    void controlDimming();
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupDistanceEncoding;
    std::unique_ptr<juce::ToggleButton> toggleDistanceEncoding;
    std::unique_ptr<juce::ToggleButton> toggleDoppler;
    std::unique_ptr<juce::TextButton> btnManageDistanceEncodingPresets;
    std::unique_ptr<DistanceEncodingComponent> distanceEncodingComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncodingSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

