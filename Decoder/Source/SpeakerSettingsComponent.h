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
#include "../../Common/PointSelection.h"
#include "../../Common/AmbiSettingsCollection.h"
#include "DecoderSettings.h"
#include "../../Common/MultiSliderControl.h"
#include "../../Common/TableColumnCallback.h"
#include "../../Common/SoloMuteCallback.h"
#include "../../Common/DelayHelper.h"
#include "../../Common/TestSoundGenerator.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "FilterSettingsComponent.h"
#include "MultiDecoderComponent.h"
#include "AmbiSettingsComponent.h"
#include "SpeakerPresetHelper.h"
#include "DecodingPresetHelper.h"
#include "FilterPresetHelper.h"
#include "../../Common/PresetManagerDialog.h"
#include "../../Common/ZoomSettings.h"
#include "../../Common/ChannelLayout.h"
#include "../../Common/FilterControlCallback.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/

class SpeakerSettingsComponent  : public Component,
                                  public TableListBoxModel,
                                  public ChangeListener,
                                  public ActionBroadcaster,
                                  public ChangeBroadcaster,
                                  public TableColumnCallback,
                                  public FilterControlCallback,
                                  public SoloMuteCallback,
                                  ActionListener,
                                  public juce::Button::Listener,
                                  public juce::Slider::Listener,
                                  public ApplicationCommandTarget,
                                  public MenuBarModel
{
public:
    //==============================================================================
    SpeakerSettingsComponent (AmbiSpeakerSet* _pSpeakerSet, SpeakerPresetHelper* _pSpeakerPresetHelper, DecodingPresetHelper* _pDecodingPresetHelper, PointSelection* _pPointSelection, AmbiSettingsCollection* _pAmbiSettings, DecoderSettings* _pDecoderSettings, TestSoundGenerator* pTestSoundListener, ChangeListener* pCallback, dsp::ProcessSpec* _pFilterSpecification, ZoomSettings* _pZoomSettings, ChannelLayout* _pChannelLayout);
    ~SpeakerSettingsComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    // table overrides
	int getNumRows() override;
	void selectedRowsChanged(int lastRowSelected) override;
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;

	// table item callbacks
	String getTableText(const int columnId, const int rowNumber) override;
	void setTableText(const int columnId, const int rowNumber, const String& newText) override;
	void setValue(int columnId, int rowNumber, double newValue) override;
	double getValue(int columnId, int rowNumber) override;
    void setBypass(int rowNumber, bool newValue) override;
    bool getBypass(int rowNumber) override;
	void speakerTest(int rowNumber) const;
	TableListBox* getTable() override;
    SliderRange getSliderRange(int columnId) override;
    bool getEnabled(const int columnId, const int rowNumber) override;


	void changeListenerCallback(ChangeBroadcaster* source) override;
    void actionListenerCallback(const String &message) override;
	void updateUI() const;
	FilterBankInfo* getFilterInfo(int rowNumber) override;
	dsp::ProcessSpec* getFilterSpecification() override;
    void showFilterEditor(int rowNumber, Rectangle<int> screenBounds) override;
	void controlDimming();
    FilterPresetHelper* getFilterPresetHelper() const;
    void mouseUp(const MouseEvent &event) override;

    // menu
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override;
    void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(Array<CommandID>& c) override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    bool doBackupAllPresets();
    bool doRestoreAllPresets();

    // Inherited via SoloMuteCallback
    bool getMute(int rowNumber) override;
    void setMute(int rowNumber, bool newValue) override;
    bool getSolo(int rowNumber) override;
    void setSolo(int rowNumber, bool newValue) override;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<MenuBarComponent> menuBar;
    ApplicationCommandManager commandManager;
	AmbiSpeakerSet* pSpeakerSet;
	SpeakerPresetHelper* pSpeakerPresetHelper;
	DecodingPresetHelper* pDecodingPresetHelper;
	PointSelection* pPointSelection;
	AmbiSettingsCollection* pAmbiSettings;
	DecoderSettings* pDecoderSettings;
	DelayHelper delayHelper;
	TestSoundGenerator* pTestSoundGenerator;
	dsp::ProcessSpec* pFilterSpecification;
    PresetManagerDialog presetManagerDialog;
    std::unique_ptr<FilterPresetHelper> filterPresetHelper;
    ZoomSettings* pZoomSettings;
    ChannelLayout* pChannelLayout;

    enum CommandIDs
    {
        speakerEditMode = 1,
        speakerSavePreset,
        speakerManagePresets,
        decodingSavePreset,
        decodingManagePresets,
        filterManagePresets,
        backupAllPresets,
        restoreAllPresets,
        about
    };


    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> groupOsc;
    std::unique_ptr<juce::GroupComponent> groupAmbisonics;
    std::unique_ptr<juce::GroupComponent> groupSpeakers;
    std::unique_ptr<TableListBox> speakerList;
    std::unique_ptr<juce::TextButton> buttonAdd;
    std::unique_ptr<juce::TextButton> buttonRemove;
    std::unique_ptr<juce::TextButton> buttonMoveDown;
    std::unique_ptr<juce::TextButton> buttonMoveUp;
    std::unique_ptr<juce::Label> labelOscPort;
    std::unique_ptr<juce::Label> labelTimeout;
    std::unique_ptr<juce::ToggleButton> toggleOsc;
    std::unique_ptr<juce::TextButton> buttonSpeakerTest;
    std::unique_ptr<juce::Label> labelDevelopmentVersion;
    std::unique_ptr<juce::TextButton> buttonScaling;
    std::unique_ptr<juce::Slider> sliderPort;
    std::unique_ptr<juce::Slider> sliderTimeout;
    std::unique_ptr<juce::ToggleButton> toggleMultiDecoder;
    std::unique_ptr<MultiDecoderComponent> multiDecoderControl;
    std::unique_ptr<AmbiSettingsComponent> ambiSettingsControl;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeakerSettingsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

