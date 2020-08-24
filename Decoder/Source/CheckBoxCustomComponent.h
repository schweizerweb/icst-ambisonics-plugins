/*
  ==============================================================================

    EditableTextCustomComponent.h
    Created: 9 Oct 2017 2:04:27pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"
#include "FilterSettingsComponent.h"

class CheckBoxCustomComponent : public Component, private  ToggleButton::Listener
{
public:
	CheckBoxCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
        addAndMakeVisible(toggle);
        toggle.addListener(this);
        toggle.setWantsKeyboardFocus(false);
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		toggle.setToggleState(owner.getFlag(columnId, row), dontSendNotification);
	}
    
    void resized() override
    {
        toggle.setBoundsInset(BorderSize<int>(2));
    }

    void buttonClicked(Button* b) override
    {
		CallOutBox::launchAsynchronously(new FilterSettingsComponent(owner.getFilterInfo(row), owner.getFilterSpecification(), &owner, owner.getFilterPresetHelper()), b->getScreenBounds(), nullptr);

        owner.setFlag(columnId, row, b->getToggleState());
    }
    
private:
	SpeakerSettingsComponent& owner;
	int row, columnId;
    ToggleButton toggle;
};
