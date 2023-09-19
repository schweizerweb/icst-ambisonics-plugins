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
#include "DistanceEncodingParams.h"
#include "DistanceEncodingGraph.h"
#include "DistanceEncodingPresetHelper.h"
#include "../../Common/ZoomSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DistanceEncodingComponent  : public Component,
                                   ChangeListener,
                                   ActionListener,
                                   public juce::Slider::Listener,
                                   public juce::ComboBox::Listener,
                                   public juce::Button::Listener
{
public:
    //==============================================================================
    DistanceEncodingComponent (DistanceEncodingParams* pParams, DistanceEncodingPresetHelper* pPresetHelper, ZoomSettings* pZoomSettings);
    ~DistanceEncodingComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updatePresetComboBox();
    void controlDimming() const;
    void setUiValues(DistanceEncodingParams *pEncodingParams);
    void changeListenerCallback(ChangeBroadcaster *source) override;
    void actionListenerCallback(const String &message) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DistanceEncodingParams* pParams;
    DistanceEncodingPresetHelper* pPresetHelper;
    ZoomSettings* pZoomSettings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupAirAbsorbtion;
    std::unique_ptr<juce::GroupComponent> groupAttenuation;
    std::unique_ptr<DistanceEncodingGraph> distanceEncodingGraph;
    std::unique_ptr<juce::Slider> sliderUnitCircleRadius;
    std::unique_ptr<juce::Label> labelUnitCircleRadius;
    std::unique_ptr<juce::Label> labelEncodingMode;
    std::unique_ptr<juce::ComboBox> comboBoxEncodingMode;
    std::unique_ptr<juce::Slider> sliderDbUnit;
    std::unique_ptr<juce::Label> labelDbUnit;
    std::unique_ptr<juce::Slider> sliderDistanceAttenuation;
    std::unique_ptr<juce::Label> labelDistanceAttenuation;
    std::unique_ptr<juce::Slider> sliderCenterCurve;
    std::unique_ptr<juce::Label> labelCenterCurve;
    std::unique_ptr<juce::Slider> sliderAdvancedFactor;
    std::unique_ptr<juce::Label> labelAdvancedFact;
    std::unique_ptr<juce::Slider> sliderAdvancedExponent;
    std::unique_ptr<juce::Label> labelAdvancedExponent;
    std::unique_ptr<juce::ComboBox> comboBoxDistanceEncodingPreset;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    std::unique_ptr<juce::Label> labelAirAbsorbtionMode;
    std::unique_ptr<juce::ComboBox> comboBoxAirAbsorbtionMode;
    std::unique_ptr<juce::Slider> sliderAirAbsorbtionIntensity;
    std::unique_ptr<juce::Label> labelIntensity;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceEncodingComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
