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
#include "JuceHeader.h"
#include "../../Common/ZoomSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RadarSettingsComponent  : public Component,
                                public ChangeBroadcaster,
                                public ChangeListener,
                                public juce::Slider::Listener,
                                public juce::Button::Listener
{
public:
    //==============================================================================
    RadarSettingsComponent (ChangeListener* pChangeListener, ZoomSettings* pZoomSettings);
    ~RadarSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void displaySettings();
    void changeListenerCallback(ChangeBroadcaster *source) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ZoomSettings* pZoomSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupZoom;
    std::unique_ptr<juce::Slider> sliderCenterPointX;
    std::unique_ptr<juce::Label> labelCenterPointX;
    std::unique_ptr<juce::Slider> sliderCenterPointY;
    std::unique_ptr<juce::Label> labelCenterPointY;
    std::unique_ptr<juce::Slider> sliderCenterPointZ;
    std::unique_ptr<juce::Label> labelCenterPointZ;
    std::unique_ptr<juce::GroupComponent> groupDisplay;
    std::unique_ptr<juce::Slider> sliderPointScaler;
    std::unique_ptr<juce::Label> labelPointScaler;
    std::unique_ptr<juce::ToggleButton> toggleLabelInPoint;
    std::unique_ptr<juce::Slider> sliderRadius;
    std::unique_ptr<juce::Label> labelRadius;
    std::unique_ptr<juce::TextButton> btnReset;
    std::unique_ptr<juce::Slider> sliderGroupPointScaler;
    std::unique_ptr<juce::Label> labelGroupPointScaler;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RadarSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

