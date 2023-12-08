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
#include "GroupTableListModel.h"
#include "SourceTableListModel.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/TableColumnCallback.h"
#include "../../Common/ZoomSettings.h"
#include "EncoderSettings.h"
#include "ComponentArguments.h"
#include "EncoderPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SourceDefinitionComponent  : public Component,
                                   public ChangeListener,
                                   public ActionListener,
                                   public ChangeBroadcaster,
                                   public juce::Button::Listener,
                                   public juce::Slider::Listener,
                                   public juce::ComboBox::Listener
{
public:
    //==============================================================================
    SourceDefinitionComponent (EncoderSettingsComponentArgs args);
    ~SourceDefinitionComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void controlDimming() const;
    void refresh() const;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EncoderSettingsComponentArgs m_args;
    std::unique_ptr<GroupTableListModel> groupModel;
    std::unique_ptr<SourceTableListModel> sourceModel;
    PresetManagerDialog presetManagerDialog;

    void initializePresets();
    void updateEncodingUiElements();
    void handleAmbiOrders();
    void actionListenerCallback(const String &message) override;
    void mouseUp(const MouseEvent &event) override;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupGroups;
    std::unique_ptr<TableListBox> groupList;
    std::unique_ptr<juce::TextButton> buttonAddGroup;
    std::unique_ptr<juce::TextButton> buttonRemoveGroup;
    std::unique_ptr<juce::GroupComponent> groupSources;
    std::unique_ptr<TableListBox> sourceList;
    std::unique_ptr<juce::TextButton> buttonAdd;
    std::unique_ptr<juce::TextButton> buttonRemove;
    std::unique_ptr<juce::TextButton> buttonMoveDown;
    std::unique_ptr<juce::TextButton> buttonMoveUp;
    std::unique_ptr<juce::TextButton> buttonMoveGroupDown;
    std::unique_ptr<juce::TextButton> buttonMoveGroupUp;
    std::unique_ptr<juce::Slider> sliderDistanceScaler;
    std::unique_ptr<juce::Label> labelDistanceScaler;
    std::unique_ptr<juce::ToggleButton> toggleInfiniteDistance;
    std::unique_ptr<juce::Label> labelMasterGain;
    std::unique_ptr<juce::Slider> sliderMasterGain;
    std::unique_ptr<juce::ComboBox> comboBoxPresets;
    std::unique_ptr<juce::Label> labelPresets;
    std::unique_ptr<juce::TextButton> buttonSave;
    std::unique_ptr<juce::TextButton> buttonManagePresets;
    std::unique_ptr<juce::Component> dummyHeight;
    std::unique_ptr<juce::Label> labelAmbiOrder;
    std::unique_ptr<juce::ComboBox> comboAmbiOrder;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceDefinitionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

